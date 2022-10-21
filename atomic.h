#ifndef ATOMIC_H
#define ATOMIC_H

/* enum definition */
enum {
	ATOMIC_IS_IDLE = 0,
	ATOMIC_IS_CHECKING = 1,
	ATOMIC_MAX,
};

/* function prototype declaration */
int try_check_password(void);
void done_check_password(void);

#endif /* ATOMIC_H */
