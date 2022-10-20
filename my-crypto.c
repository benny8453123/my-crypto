#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include "my-crypto.h"
#include "retval.h"
#include "tty.h"
#include "proc.h"
#include "sha256.h"
#include "proc.h"

/* Global password hash */
char password_buf[PASSWORD_LENGTH] = {0};
char password_hash[PASSWORD_HASH_LENGTH] = {0};

/* Module param init */
static char *mod_password = "password";
module_param(mod_password, charp, S_IRUSR | S_IWUSR);
MODULE_PARM_DESC(mod_password, "The password of encrypt_test_module.ko");

static int __init my_crypto_init(void)
{
	int ret = -RET_VAL_FAIL;
	int str_len = 0;
	
	print_string("Enter my_crypto_init, hello");

	/* Check environment variable which is password */
	str_len = strlen(mod_password);
	if (str_len > PASSWORD_LENGTH - 1) {
		print_string("Error! Password too long!!");
		goto out;
	}
	if (str_len <= 0) {
		print_string("Error! Password is NULL!!");
		goto out;
	}
	strncpy(password_buf, mod_password, PASSWORD_LENGTH);
	
	/* Create entry in proc */
	ret = create_my_proc_data_entry();
	if (!ret)
		print_string("Please printf the password in /proc/guess_password (do not using echo!)");
	else 
		goto out;

	/* Print password hash */
	crypto_sha256(password_buf, password_hash);
	print_string("The sha256 of the password is:");
	// For debugging
	//print_buf_string("password: %s, strlen: %d", password_buf, (int)strlen(password_buf)); //Debug
	print_string(password_hash);

	ret = RET_VAL_SUCCESS;

out:
	return ret; 
}

static void __exit my_crypto_exit(void)
{
	print_string("Enter my_crypto_exit, bye bye");

	remove_my_proc_data_entry();
}

module_init(my_crypto_init);
module_exit(my_crypto_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ben Chang");
MODULE_DESCRIPTION("Testing for encrypt and multiple api");
