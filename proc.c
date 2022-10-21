#include <linux/module.h>
#include <linux/proc_fs.h>
#include "proc.h"
#include "retval.h"
/* For using print_string() & print_buf_string() */
#include "tty.h"
/* For using try_check_password() */
#include "atomic.h"
/* For using start_checking_work() & done_checking_work() */
#include "workqueue.h"

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

	/* Clean buf first */
	memset(guessword_buf, 0, sizeof(char) * PASSWORD_LENGTH);
	memset(guessword_hash, 0, sizeof(char) * PASSWORD_HASH_LENGTH);

	if (copy_from_user(guessword_buf, buf, count))
		return -EFAULT;

	/* For debugging */
	//print_buf_string("guess: %s, strlen: %d", guessword_buf, (int)strlen(guessword_buf));

	crypto_sha256(guessword_buf, guessword_hash);

	/* Start checking workqueue */
	start_checking_work();

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
