/* Copyright 1989-2018, Joseph Pasquale, University of California, San Diego
 *
 *	UMIX kernel-related major constants and interfaces
 *
 *	Relevant only to kernel code (i.e., NOT to user processes).
 *
 *	The constants should NOT to be changed, unless explicitly stated
 *	otherwise, as other already-compiled parts of the kernel rely on
 *	these constants.
 */

#include <setjmp.h>

/*	Processes
 */
#define MAXPROCS	10	// max processes: DO NOT CHANGE

/*	Shared Memory
 */
#define MAXSHM		1024	// max shared memory size (bytes): DO NOT CHANGE

/*	Semaphores
 */
#define MAXSEMS		100	// max semaphores: DO NOT CHANGE

/*	Scheduling
 */
#define NOSCHEDPOLICY	0
#define ARBITRARY	1	// choose arbitrarily (any process is OK)
#define FIFO		2
#define LIFO		3
#define ROUNDROBIN	4
#define PROPORTIONAL	5

/*	Kernel procedures: can only be called from within the kernel
 */

/*	PA1: Context switching
 */

typedef struct {
        int pid;		// process ID
        jmp_buf env;		// jmp_buf
} CONTEXT;

void SaveContext (CONTEXT *c);		// save context of current process in c
void RestoreContext (CONTEXT *c);	// restore context from c
int SwitchContext (int p);		// working version of switch context 
int GetCurProc ();			// returns current process's ID

/*	PA2: Scheduling
 */
void SetTimer (int t);		// timer to interrupt after t ticks
int GetTimer ();		// returns the current value of timer
int SetSchedPolicy (int p);	// set the scheduling policy to p
int GetSchedPolicy ();		// return the scheduling policy
void DoSched ();		// make sched decision at next opportune time

/*	PA3: Synchronization
 */
int Block (int p);		// make proc p ineligible for sched
int Unblock (int p);		// make proc p eligible for sched

/*	Other
 */
void Panic (char *s);		// causes abnormal exit from kernel, print s
