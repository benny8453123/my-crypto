#ifndef PROC_H
#define PROC_H

/* For using PASSWORD_LENGTH macro */
#include "my-crypto.h"
/* For using PASSWORD_HASH_LENGTH macro */
#include "sha256.h"

/* function prototype declaration */
/* create and remove proc entry */
int create_my_proc_data_entry(void);
void remove_my_proc_data_entry(void);

/* global variable extern */
extern char guessword_buf[PASSWORD_LENGTH];
extern char guessword_hash[PASSWORD_HASH_LENGTH];

#endif /* PROC_H */
