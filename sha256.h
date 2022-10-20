#ifndef SHA256_H
#define SHA256_H

/* macro definition*/
#define SHA256_LENGTH			32
#define PASSWORD_HASH_LENGTH	(SHA256_LENGTH * 2 + 1)

/* function prototype declaration */
/* caculate sha256 hash */
int crypto_sha256(char *plaintext, char *hash);

#endif /* SHA256_H */
