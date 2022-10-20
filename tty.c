#include <linux/tty.h>
#include <linux/sched.h>
#include "tty.h"

/* Global log_buf */
char log_buf[LOG_BUF_SIZE] = {0};

void print_string(char *str)
{
	static int warning = 0;
	struct tty_struct *my_tty = get_current_tty();

	if (!str)
		return;

	if (my_tty) {
		const struct tty_operations *ttyops = my_tty->driver->ops;
		const char *header = "[my_crypto]: ";

		/* print header */
		(ttyops->write)(my_tty, header, strlen(header));
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
