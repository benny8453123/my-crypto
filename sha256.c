#include <crypto/internal/hash.h>
#include <linux/slab.h>
#include "sha256.h"
#include "retval.h"

static void sha256_to_readablehash(char *hash_sha256, char *hash) 
{ 
    int i; 
 
    for (i = 0; i < SHA256_LENGTH; i++) 
        sprintf(&hash[i * 2], "%02x", (unsigned char)hash_sha256[i]); 

    hash[i * 2] = 0; 

	return;
}

int crypto_sha256(char *plaintext, char *hash)
{
	int ret = -RET_VAL_FAIL;
	char hash_sha256[SHA256_LENGTH];
	struct crypto_shash *sha256;
	struct shash_desc *shash;

	sha256 = crypto_alloc_shash("sha256", 0, 0);

	if (IS_ERR(sha256))
		goto out;

	shash = kmalloc(sizeof(struct shash_desc) + crypto_shash_descsize(sha256), 
                    GFP_KERNEL);

	if (!shash) {
        ret = -ENOMEM;
		goto out;
	}

	shash->tfm = sha256;

	if (crypto_shash_init(shash))
		goto out;

	if (crypto_shash_update(shash, plaintext, strlen(plaintext)))
		goto out;
		
	if (crypto_shash_final(shash, hash_sha256))
		goto out;

	kfree(shash);
    crypto_free_shash(sha256);

	sha256_to_readablehash(hash_sha256, hash);
	ret = RET_VAL_SUCCESS;

out:
	return ret;
}
