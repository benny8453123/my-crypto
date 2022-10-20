#ifndef MY_CRYPTO_H
#define MY_CRYPTO_H

#include "sha256.h"

/* macro definition*/
#define PASSWORD_LENGTH			65

/* function prototype declaration */

/* global variable extern */
extern char password_buf[PASSWORD_LENGTH];
extern char password_hash[PASSWORD_HASH_LENGTH];

#endif /* MY_CRYPTO_H */
