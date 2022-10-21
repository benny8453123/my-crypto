#include <linux/tty.h>
#include <linux/sched.h>
#include <linux/delay.h>
#include "tty.h"

#define LOG_TAG				"[my_crypto]: "
#define DOT_PRINT_TIMES		4
#define DOT_DELAY_TIME		1000

/* Global log_buf */
char log_buf[LOG_BUF_SIZE] = {0};

void print_dot(void)
{
	int i;

	for (i = 0; i< DOT_PRINT_TIMES; ++i) {
		mdelay(DOT_DELAY_TIME);
		print_string(".");
	}
}

void print_string(char *str)
{
	static int warning = 0;
	struct tty_struct *my_tty = get_current_tty();

	if (!str)
		return;

	if (my_tty) {
		const struct tty_operations *ttyops = my_tty->driver->ops;

		/* print header */
		(ttyops->write)(my_tty, LOG_TAG, strlen(LOG_TAG));
		/* print string */
		(ttyops->write)(my_tty, str, strlen(str));
		/* print \n */
		(ttyops->write)(my_tty, "\015\012", 2);
	} else {
		if (!warning) {
			warning = 1;
			my_crypto_err("Warning: Cannot get current tty\n");
		}
		my_crypto_info("%s\n", str);
	}

	return;
}
