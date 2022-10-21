#ifndef ATOMIC_H
#define ATOMIC_H

/* enum definition */
enum {
	ATOMIC_IS_IDLE = 0,
	ATOMIC_IS_CHECKING = 1,
	ATOMIC_STATUS_MAX,
};

enum {
	ATOMIC_IS_MATCH = 0,
	ATOMIC_NOT_MATCH = 1,
	ATOMIC_MATCH_MAX,
};

/* function prototype declaration */
/* check state */
int try_check_password(void);
void done_check_password(void);
/* match state */
void set_hash_match(int match);
int get_hash_match(void);
void set_buf_match(int match);
int get_buf_match(void);
void init_match(void);

#endif /* ATOMIC_H */
