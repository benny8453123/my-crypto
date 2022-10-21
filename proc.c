#include <linux/module.h>
#include <linux/proc_fs.h>
#include "proc.h"
#include "retval.h"
/* For using print_string() and print_buf_string() */
#include "tty.h"
/* For using try_check_password(), init_match(),
 * get_hash_match(), get_buf_match() and ATOMIC_IS_MATCH macro
 */
#include "atomic.h"
/* For using start_checking_work() and done_checking_work() */
#include "workqueue.h"
/* For using init_check_completion(), wait_buf_check_completion()
 * and wait_hash_check_completion()
 */
#include "completion.h"

#define MY_PROC_DATA_FILENAME	"guess_password"

char guessword_buf[PASSWORD_LENGTH] = {0};
char guessword_hash[PASSWORD_HASH_LENGTH] = {0};
struct proc_dir_entry *my_proc_data_entry = NULL;

ssize_t my_proc_data_write(struct file *file, const char __user *buf, size_t count, loff_t *lof)
{
	if (count > PASSWORD_LENGTH - 1) {
		print_string("Error! password length error!");
		return -EINVAL;
	}

	/* Check is checking or not */
	if (try_check_password() == ATOMIC_IS_CHECKING) {
		/* Is checking just leave */
		print_string("Try it later again! previous guessword is checking!");
		return -EBUSY;
	}

	/* Init state:
	 * 	1. Clean buf
	 * 	2. Reset result-atomic of buf and hash
	 * 	3. Reset checking-completion of buf and hash
	 */
	memset(guessword_buf, 0, sizeof(char) * PASSWORD_LENGTH);
	memset(guessword_hash, 0, sizeof(char) * PASSWORD_HASH_LENGTH);
	init_match();
	init_check_completion();

	if (copy_from_user(guessword_buf, buf, count))
		return -EFAULT;

	/* For debugging */
	//print_buf_string("guess: %s, strlen: %d", guessword_buf, (int)strlen(guessword_buf));

	crypto_sha256(guessword_buf, guessword_hash);

	/* Start checking workqueue */
	start_hash_checking_work();
	start_buf_checking_work();

	/* Wait for both checking completion */
	wait_buf_check_completion();
	wait_hash_check_completion();

	/* Print result */
	print_string("Checking sha256...");
	print_dot();
	if (get_hash_match() == ATOMIC_IS_MATCH) {
		print_string("WoW! Sha256 comapare match! Checking password next...");
		print_dot();

		if(get_buf_match() == ATOMIC_IS_MATCH)
			print_string("Congratulations! Password match!");
		else {
			print_string("Oh no! Password not match!");
			print_string("This is such a rare sitiuation.");
		}
	} else {
		print_string("Sorry! Sha256 comapare not match!");
		print_string("Ths sha256 of your answer is: ");
		print_string(guessword_hash);
	}

	/* Release checking state atomic */
	done_check_password();

	return count;
}

static int my_proc_data_open(struct inode *inode, struct file *file)
{
	try_module_get(THIS_MODULE);

	return 0;
}

static int my_proc_data_release(struct inode *inode, struct file *file)
{
	module_put(THIS_MODULE);

	return 0;
}

static const struct proc_ops my_proc_data_ops = {
	.proc_write = my_proc_data_write,
	//.proc_read = my_proc_data_read,
	.proc_open = my_proc_data_open,
	.proc_release = my_proc_data_release,
};

int create_my_proc_data_entry(void)
{
	int ret = -RET_VAL_FAIL;
	
	/* Create proc entry */
	my_proc_data_entry = proc_create_data(MY_PROC_DATA_FILENAME, 0774, NULL, &my_proc_data_ops, NULL);
	if (!my_proc_data_entry) {
		print_string("Error! Failed to create entry in /proc!");
		ret = -ENOMEM;
		goto out;
	}

	/* All done update return val goto out */
	ret = RET_VAL_SUCCESS;

out:
	return ret;
}

void remove_my_proc_data_entry(void)
{
	/* Release resource of workqueue */
	done_checking_work();

	/* Release proc entry resource */
	remove_proc_entry(MY_PROC_DATA_FILENAME, NULL);

	return;
}
