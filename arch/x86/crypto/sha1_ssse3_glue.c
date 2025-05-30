// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Cryptographic API.
 *
 * Glue code for the SHA1 Secure Hash Algorithm assembler implementations
 * using SSSE3, AVX, AVX2, and SHA-NI instructions.
 *
 * This file is based on sha1_generic.c
 *
 * Copyright (c) Alan Smithee.
 * Copyright (c) Andrew McDonald <andrew@mcdonald.org.uk>
 * Copyright (c) Jean-Francois Dive <jef@linuxbe.org>
 * Copyright (c) Mathias Krause <minipli@googlemail.com>
 * Copyright (c) Chandramouli Narayanan <mouli@linux.intel.com>
 */

#define pr_fmt(fmt)	KBUILD_MODNAME ": " fmt

#include <asm/cpu_device_id.h>
#include <asm/simd.h>
#include <crypto/internal/hash.h>
#include <crypto/sha1.h>
#include <crypto/sha1_base.h>
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/module.h>

static const struct x86_cpu_id module_cpu_ids[] = {
	X86_MATCH_FEATURE(X86_FEATURE_SHA_NI, NULL),
	X86_MATCH_FEATURE(X86_FEATURE_AVX2, NULL),
	X86_MATCH_FEATURE(X86_FEATURE_AVX, NULL),
	X86_MATCH_FEATURE(X86_FEATURE_SSSE3, NULL),
	{}
};
MODULE_DEVICE_TABLE(x86cpu, module_cpu_ids);

static inline int sha1_update(struct shash_desc *desc, const u8 *data,
			      unsigned int len, sha1_block_fn *sha1_xform)
{
	int remain;

	/*
	 * Make sure struct sha1_state begins directly with the SHA1
	 * 160-bit internal state, as this is what the asm functions expect.
	 */
	BUILD_BUG_ON(offsetof(struct sha1_state, state) != 0);

	kernel_fpu_begin();
	remain = sha1_base_do_update_blocks(desc, data, len, sha1_xform);
	kernel_fpu_end();

	return remain;
}

static inline int sha1_finup(struct shash_desc *desc, const u8 *data,
			     unsigned int len, u8 *out,
			     sha1_block_fn *sha1_xform)
{
	kernel_fpu_begin();
	sha1_base_do_finup(desc, data, len, sha1_xform);
	kernel_fpu_end();

	return sha1_base_finish(desc, out);
}

asmlinkage void sha1_transform_ssse3(struct sha1_state *state,
				     const u8 *data, int blocks);

static int sha1_ssse3_update(struct shash_desc *desc, const u8 *data,
			     unsigned int len)
{
	return sha1_update(desc, data, len, sha1_transform_ssse3);
}

static int sha1_ssse3_finup(struct shash_desc *desc, const u8 *data,
			      unsigned int len, u8 *out)
{
	return sha1_finup(desc, data, len, out, sha1_transform_ssse3);
}

static struct shash_alg sha1_ssse3_alg = {
	.digestsize	=	SHA1_DIGEST_SIZE,
	.init		=	sha1_base_init,
	.update		=	sha1_ssse3_update,
	.finup		=	sha1_ssse3_finup,
	.descsize	=	SHA1_STATE_SIZE,
	.base		=	{
		.cra_name	=	"sha1",
		.cra_driver_name =	"sha1-ssse3",
		.cra_priority	=	150,
		.cra_flags	=	CRYPTO_AHASH_ALG_BLOCK_ONLY,
		.cra_blocksize	=	SHA1_BLOCK_SIZE,
		.cra_module	=	THIS_MODULE,
	}
};

static int register_sha1_ssse3(void)
{
	if (boot_cpu_has(X86_FEATURE_SSSE3))
		return crypto_register_shash(&sha1_ssse3_alg);
	return 0;
}

static void unregister_sha1_ssse3(void)
{
	if (boot_cpu_has(X86_FEATURE_SSSE3))
		crypto_unregister_shash(&sha1_ssse3_alg);
}

asmlinkage void sha1_transform_avx(struct sha1_state *state,
				   const u8 *data, int blocks);

static int sha1_avx_update(struct shash_desc *desc, const u8 *data,
			     unsigned int len)
{
	return sha1_update(desc, data, len, sha1_transform_avx);
}

static int sha1_avx_finup(struct shash_desc *desc, const u8 *data,
			      unsigned int len, u8 *out)
{
	return sha1_finup(desc, data, len, out, sha1_transform_avx);
}

static struct shash_alg sha1_avx_alg = {
	.digestsize	=	SHA1_DIGEST_SIZE,
	.init		=	sha1_base_init,
	.update		=	sha1_avx_update,
	.finup		=	sha1_avx_finup,
	.descsize	=	SHA1_STATE_SIZE,
	.base		=	{
		.cra_name	=	"sha1",
		.cra_driver_name =	"sha1-avx",
		.cra_priority	=	160,
		.cra_flags	=	CRYPTO_AHASH_ALG_BLOCK_ONLY,
		.cra_blocksize	=	SHA1_BLOCK_SIZE,
		.cra_module	=	THIS_MODULE,
	}
};

static bool avx_usable(void)
{
	if (!cpu_has_xfeatures(XFEATURE_MASK_SSE | XFEATURE_MASK_YMM, NULL)) {
		if (boot_cpu_has(X86_FEATURE_AVX))
			pr_info("AVX detected but unusable.\n");
		return false;
	}

	return true;
}

static int register_sha1_avx(void)
{
	if (avx_usable())
		return crypto_register_shash(&sha1_avx_alg);
	return 0;
}

static void unregister_sha1_avx(void)
{
	if (avx_usable())
		crypto_unregister_shash(&sha1_avx_alg);
}

#define SHA1_AVX2_BLOCK_OPTSIZE	4	/* optimal 4*64 bytes of SHA1 blocks */

asmlinkage void sha1_transform_avx2(struct sha1_state *state,
				    const u8 *data, int blocks);

static bool avx2_usable(void)
{
	if (avx_usable() && boot_cpu_has(X86_FEATURE_AVX2)
		&& boot_cpu_has(X86_FEATURE_BMI1)
		&& boot_cpu_has(X86_FEATURE_BMI2))
		return true;

	return false;
}

static inline void sha1_apply_transform_avx2(struct sha1_state *state,
					     const u8 *data, int blocks)
{
	/* Select the optimal transform based on data block size */
	if (blocks >= SHA1_AVX2_BLOCK_OPTSIZE)
		sha1_transform_avx2(state, data, blocks);
	else
		sha1_transform_avx(state, data, blocks);
}

static int sha1_avx2_update(struct shash_desc *desc, const u8 *data,
			     unsigned int len)
{
	return sha1_update(desc, data, len, sha1_apply_transform_avx2);
}

static int sha1_avx2_finup(struct shash_desc *desc, const u8 *data,
			      unsigned int len, u8 *out)
{
	return sha1_finup(desc, data, len, out, sha1_apply_transform_avx2);
}

static struct shash_alg sha1_avx2_alg = {
	.digestsize	=	SHA1_DIGEST_SIZE,
	.init		=	sha1_base_init,
	.update		=	sha1_avx2_update,
	.finup		=	sha1_avx2_finup,
	.descsize	=	SHA1_STATE_SIZE,
	.base		=	{
		.cra_name	=	"sha1",
		.cra_driver_name =	"sha1-avx2",
		.cra_priority	=	170,
		.cra_flags	=	CRYPTO_AHASH_ALG_BLOCK_ONLY,
		.cra_blocksize	=	SHA1_BLOCK_SIZE,
		.cra_module	=	THIS_MODULE,
	}
};

static int register_sha1_avx2(void)
{
	if (avx2_usable())
		return crypto_register_shash(&sha1_avx2_alg);
	return 0;
}

static void unregister_sha1_avx2(void)
{
	if (avx2_usable())
		crypto_unregister_shash(&sha1_avx2_alg);
}

asmlinkage void sha1_ni_transform(struct sha1_state *digest, const u8 *data,
				  int rounds);

static int sha1_ni_update(struct shash_desc *desc, const u8 *data,
			     unsigned int len)
{
	return sha1_update(desc, data, len, sha1_ni_transform);
}

static int sha1_ni_finup(struct shash_desc *desc, const u8 *data,
			      unsigned int len, u8 *out)
{
	return sha1_finup(desc, data, len, out, sha1_ni_transform);
}

static struct shash_alg sha1_ni_alg = {
	.digestsize	=	SHA1_DIGEST_SIZE,
	.init		=	sha1_base_init,
	.update		=	sha1_ni_update,
	.finup		=	sha1_ni_finup,
	.descsize	=	SHA1_STATE_SIZE,
	.base		=	{
		.cra_name	=	"sha1",
		.cra_driver_name =	"sha1-ni",
		.cra_priority	=	250,
		.cra_flags	=	CRYPTO_AHASH_ALG_BLOCK_ONLY,
		.cra_blocksize	=	SHA1_BLOCK_SIZE,
		.cra_module	=	THIS_MODULE,
	}
};

static int register_sha1_ni(void)
{
	if (boot_cpu_has(X86_FEATURE_SHA_NI))
		return crypto_register_shash(&sha1_ni_alg);
	return 0;
}

static void unregister_sha1_ni(void)
{
	if (boot_cpu_has(X86_FEATURE_SHA_NI))
		crypto_unregister_shash(&sha1_ni_alg);
}

static int __init sha1_ssse3_mod_init(void)
{
	if (!x86_match_cpu(module_cpu_ids))
		return -ENODEV;

	if (register_sha1_ssse3())
		goto fail;

	if (register_sha1_avx()) {
		unregister_sha1_ssse3();
		goto fail;
	}

	if (register_sha1_avx2()) {
		unregister_sha1_avx();
		unregister_sha1_ssse3();
		goto fail;
	}

	if (register_sha1_ni()) {
		unregister_sha1_avx2();
		unregister_sha1_avx();
		unregister_sha1_ssse3();
		goto fail;
	}

	return 0;
fail:
	return -ENODEV;
}

static void __exit sha1_ssse3_mod_fini(void)
{
	unregister_sha1_ni();
	unregister_sha1_avx2();
	unregister_sha1_avx();
	unregister_sha1_ssse3();
}

module_init(sha1_ssse3_mod_init);
module_exit(sha1_ssse3_mod_fini);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("SHA1 Secure Hash Algorithm, Supplemental SSE3 accelerated");

MODULE_ALIAS_CRYPTO("sha1");
MODULE_ALIAS_CRYPTO("sha1-ssse3");
MODULE_ALIAS_CRYPTO("sha1-avx");
MODULE_ALIAS_CRYPTO("sha1-avx2");
MODULE_ALIAS_CRYPTO("sha1-ni");
