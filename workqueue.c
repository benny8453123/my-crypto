#include <linux/workqueue.h>
#include <linux/delay.h>
#include "workqueue.h"
/* For using print_string() */
#include "tty.h"
/* For using password_buf & buf */
#include "my-crypto.h"
/* For using guessword_buf & hash */
#include "proc.h"
/* For using done_check_password() */
#include "atomic.h"

#define QUEUE_NAME			"CHECKING_QUEUE"
#define DOT_PRINT_TIMES		4
#define DOT_DELAY_TIME		1000

static short init_work_queue = 0;
static struct workqueue_struct *queue = NULL;
static struct work_struct work;

static inline void print_dot(void)
{
	int i;
	
	for (i = 0; i< DOT_PRINT_TIMES; ++i) {
		mdelay(DOT_DELAY_TIME);
		print_string(".");
	}
}

static void checking_work(struct work_struct *work)
{
	/* Checking work */
	print_string("Checking sha256...");
	print_dot();
	if (!strncmp(password_hash, guessword_hash, PASSWORD_HASH_LENGTH))
	{
		print_string("WoW! Sha256 comapare match!");
		print_string("Checking password next...");

		print_dot();

		if (!strncmp(password_buf, guessword_buf, PASSWORD_LENGTH))
			print_string("Congratulations! Password match!");
	} else {
		print_string("Sorry! Sha256 comapare not match!");
		print_string("Ths sha256 of your answer is: ");
		print_string(guessword_hash);
	}
	/* Release checking state */
	done_check_password();

	return;
}


void start_checking_work(void)
{
	if (!init_work_queue) {
		queue = alloc_workqueue(QUEUE_NAME, WQ_UNBOUND, 1);
		INIT_WORK(&work, checking_work);
	}

	schedule_work(&work);

	return;
}

void done_checking_work(void)
{
	destroy_workqueue(queue);
}
