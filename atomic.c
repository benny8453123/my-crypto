#include <asm/atomic.h>
#include "atomic.h"

static atomic_t is_checking = ATOMIC_INIT(0);

/* Return idle that user can keep doing c.s work */
int try_check_password(void)
{
	int ret = ATOMIC_IS_CHECKING;

	if (!arch_atomic_cmpxchg(&is_checking, 0, 1))
		ret = ATOMIC_IS_IDLE;

	return ret;
}

/* Only task in c.s can call this function */
void done_check_password(void)
{
	int ret = ATOMIC_IS_CHECKING;

	if (arch_atomic_cmpxchg(&is_checking, 1, 0))
		ret = ATOMIC_IS_IDLE;

	return;
}
