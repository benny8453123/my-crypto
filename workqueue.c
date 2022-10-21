#include <linux/workqueue.h>
#include <linux/delay.h>
#include "workqueue.h"
/* For using password_buf and hash */
#include "my-crypto.h"
/* For using guessword_buf and hash */
#include "proc.h"
/* For using set_hash_match(), ATOMIC_IS_MATCH marco
 * and ATOMIC_NOT_MATCH macro
 */
#include "atomic.h"
/* For using set_hash_check_completion(), set_buf_check_completion()
 * and wait_hash_check_completion()
 */
#include "completion.h"

#define QUEUE_NAME			"CHECKING_QUEUE"
#define MAX_ACTIVATE			2

static short init_work_queue = 0;
static short init_hash_check_work = 0;
static short init_buf_check_work = 0;
static struct workqueue_struct *queue = NULL;
static struct work_struct check_hash_work, check_buf_work;

static void checking_hash_work(struct work_struct *work)
{
	if (!strncmp(password_hash, guessword_hash, PASSWORD_HASH_LENGTH))
		set_hash_match(ATOMIC_IS_MATCH);
	else
		set_hash_match(ATOMIC_NOT_MATCH);

	set_hash_check_completion();
}

static void checking_buf_work(struct work_struct *work)
{
	/* Wait for hash checking conplete */
	wait_hash_check_completion();

	if (!strncmp(password_buf, guessword_buf, PASSWORD_LENGTH))
		set_buf_match(ATOMIC_IS_MATCH);
	else
		set_buf_match(ATOMIC_NOT_MATCH);

	set_buf_check_completion();
}

void start_hash_checking_work(void)
{
	if (!init_work_queue) {
		queue = alloc_workqueue(QUEUE_NAME, WQ_UNBOUND, MAX_ACTIVATE);
		init_work_queue = 1;
	}
	if (!init_hash_check_work) {
		INIT_WORK(&check_hash_work, checking_hash_work);
		init_hash_check_work = 1;
	}

	queue_work(queue, &check_hash_work);

	return;
}

void start_buf_checking_work(void)
{
	if (!init_work_queue) {
		queue = alloc_workqueue(QUEUE_NAME, WQ_UNBOUND, MAX_ACTIVATE);
		init_work_queue = 1;
	}
	if (!init_buf_check_work) {
		INIT_WORK(&check_buf_work, checking_buf_work);
		init_buf_check_work = 1;
	}

	queue_work(queue, &check_buf_work);

	return;
}

void done_checking_work(void)
{
	if (init_work_queue)
		destroy_workqueue(queue);
}
