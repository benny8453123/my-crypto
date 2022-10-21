#include <asm/atomic.h>
#include "atomic.h"

/* Set idle at init */
static atomic_t is_checking = ATOMIC_INIT(ATOMIC_IS_IDLE);

/* Notice:
 * 		arch_atomic_cmpxchg(atomic_t, old, new) will return
 * 		the origin value of atomic_t whatever atomic_t is change
 * 		or not.
 */

/* Description:
 * 		Return idle that user can keep doing work in c.s
 */
int try_check_password(void)
{
	int ret = ATOMIC_IS_CHECKING;

	/* If if idle, set checking state. Then tell caller the origin state is idle */
	if (arch_atomic_cmpxchg(&is_checking, ATOMIC_IS_IDLE, ATOMIC_IS_CHECKING) == ATOMIC_IS_IDLE)
		ret = ATOMIC_IS_IDLE;

	return ret;
}

/* Notice:
 * 		Only task in c.s can call this function.
 * Description:
 * 		If in checking state, change it to idle state.
 */
void done_check_password(void)
{
	arch_atomic_cmpxchg(&is_checking, ATOMIC_IS_CHECKING, ATOMIC_IS_IDLE);

	return;
}
