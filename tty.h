#ifndef TTY_H
#define TTY_H

#include <linux/printk.h>
#include <linux/kernel.h>

/* macro definition*/
#define LOG_BUF_SIZE			128

#define my_crypto_info(fmt, ...) \
	pr_info("[my_crypto]: "fmt, ##__VA_ARGS__)

#define my_crypto_err(fmt, ...) \
	pr_err("[my_crypto]: "fmt, ##__VA_ARGS__)

#define my_crypto_debug(fmt, ...) \
	pr_debug("[my_crypto]: "fmt, ##__VA_ARGS__)

#define print_buf_string(fmt, ...) \
	memset(log_buf, 0, LOG_BUF_SIZE); \
	snprintf(log_buf, LOG_BUF_SIZE, fmt, ##__VA_ARGS__); \
	print_string(log_buf);

/* function prototype declaration */
void print_string(char *str);
void print_dot(void);

/* global variable extern */
extern char log_buf[LOG_BUF_SIZE];;

#endif /* TTY_H */
