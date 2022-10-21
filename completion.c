#include <linux/completion.h>
#include "completion.h"

static struct completion check_buf_completion;
static struct completion check_hash_completion;

void init_check_completion(void)
{
	init_completion(&check_hash_completion);
	init_completion(&check_buf_completion);
}


/* Notice:
 *	complete_and_exit() will cause exit than work thread
 *	will no longer running cause system hang.
 */
void set_buf_check_completion(void)
{
	complete_all(&check_buf_completion);
	//complete_and_exit(&check_buf_completion, 0);
}

void set_hash_check_completion(void)
{
	complete_all(&check_hash_completion);
	//complete_and_exit(&check_hash_completion, 0);
}

void wait_buf_check_completion(void)
{
	wait_for_completion(&check_buf_completion);
}

void wait_hash_check_completion(void)
{
	wait_for_completion(&check_hash_completion);
}
