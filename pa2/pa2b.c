/* Programming Assignment 2: Exercise B
 *
 * In this second set of exercises, you will learn about what mechanisms
 * are available to you to modify the kernel's scheduler.  Study the file
 * mycode2.c.  It contains a rudimentary process table data structure,
 * "proctab[]", and a set of procedures THAT ARE CALLED BY OTHER PARTS OF
 * THE KERNEL (the source of which you don't have nor require access to).
 * Your ability to modify the scheduler is via these procedures, and what
 * you choose to put in them.  You may also modify proctab[], or create
 * any data structures you wish.  The only constraint is that you not
 * change the interfaces to the procedures, as the rest of the kernel
 * depends on them.  Also, your system must support up to MAXPROCS existing
 * processes at any single point in time.  In fact, more than MAXPROCS
 * processes may be created (which you must allow), but for any created
 * beyond MAXPROCS, there will have been that same number that have exited
 * the system (i.e., so that only MAXPROCS processes actually exist at any
 * single point in time).
 *
 * Let's look at the procedures:
 *
 * InitSched () is called at system start-up time.  Here, the process table
 * is initialized (all the entries are marked invalid).  It also calls
 * SetTimer (t), which will be discussed in Exercise C.  Anything that you
 * want done before the system settles into its normal operation should be
 * placed in this procedure.
 *
 * StartingProc (int p) is called by the kernel when process p starts up.
 * Specifically, whenever Fork () is called by a process, which causes entry
 * into the kernel, StartingProc (p) will be called from within the kernel
 * with parameter p, where p is the PID (process identifier) of the process
 * being created.  Notice how a free entry is found in the process table,
 * and the PID is recorded.
 *
 * EndingProc (int p) is called by kernel when process p is ending.
 * Whenever a process calls Exit () (implicitly if there is no explicit
 * call), which causes entry into the kernel, EndingProc (p) will be called
 * from within the kernel with parameter p which is the PID of the exiting
 * process.  Notice how the process table is updated.
 *
 * SchedProc () is called by the kernel when it needs a decision for which
 * process to run next.  It determines which scheduling policy is in effect
 * by calling the kernel function GetSchedPolicy (), which will return one
 * of the following: ARBITRARY, FIFO, LIFO, ROUNDROBIN, and PROPORTIONAL
 * (these constants are defined in sys.h).  The scheduling policy can be
 * set by calling the kernel function SetSchedPolicy (p), where p is the
 * policy ID (one of the above constants), which is called in InitSched ().
 * SchedProc () should return a PID, or 0 if there are no processes to run.
 * This is where you implement the core of the various scheduling policies.
 * The current code for SchedProc implements ARBITRARY, which simply happens
 * to find the first valid entry in the process table, and returns it (and
 * the kernel will run the corresponding process).
 *
 * HandleTimerIntr () will be discussed in the Exercise C, and should not
 * be modified in this part.
 *
 *
 * There are additional procedures that are part of the kernel (but not in
 * mycode2.c), that you may call from within the above procedures:
 *
 * DoSched () will cause the kernel to make a scheduling decision at the next
 * opportune time (at which point SchedProc () will be called to determine
 * which process to select).  MANY STUDENTS HAVE DIFFICULTY UNDERSTANDING
 * WHY THIS FUNCTION IS NECESSARY, so please read carefully.  When you write
 * your code for the above procedures (StartingProc, EndingProc, ...),
 * there may be a point where you would like to cause the kernel to make a
 * scheduling decision regarding which process should run next.  This is
 * where you should call DoSched (), which tells the kernel that it should
 * EVENTUALLY call SchedProc (), as soon as the kernel determines it can
 * do so.  Why not immediately?  Because other code, including the remaining
 * code of the procedure you are writing, may need to execute before the
 * kernel is ready to select a process to run.  As this is a common confusion,
 * we emphasize that DoSched () does NOT directly call SchedProc().
 * DoSched() simply tells the kernel to record that SchedProc() should be
 * called eventually.  When the kernel is at a point where it is appropriate
 * for it to consider making a scheduling decision, if DoSched() was indeed
 * called in the past, the kernel THEN calls SchedProc().  (And by the way,
 * only a new call to DoSched() will cause the kernel to do this again.)
 *
 * SetTimer (int t) and GetTimer () will be discussed in Exercise C.
 *
 * Finally, your system must support up to MAXPROCS (a constant defined in
 * sys.h) existing processes at any single point in time.  In fact, more than
 * MAXPROCS processes may be created during the lifetime of the system (which
 * you must allow), but for any number created beyond MAXPROCS, there will
 * have been that same number (or more) that have exited the system (i.e., so
 * that only MAXPROCS processes actually exist AT ANY SINGLE POINT IN TIME).
 * 
 * Exercises
 *
 * 1. Implement the FIFO (First In First Out) scheduling policy.  This means
 * the order in which processes run (to completion) is the same as the order
 * in which they are created.  For the program below, the current scheduler
 * will print:  1111111111222222222244444444443333333333 (why this order?)
 * Under FIFO, it should print: 1111111111222222222233333333334444444444
 * (why this order?).
 *
 * 2. Implement the LIFO (Last In First Out) scheduling policy.  This means
 * that as soon as a process is created, it should run (to completion) before
 * any existing process.  Under the LIFO scheduling policy, the program
 * below should print:  4444444444222222222233333333331111111111 (why
 * this order, and not 4444444444333333333322222222221111111111?).
 */

#include <stdio.h>
#include "aux.h"
#include "umix.h"

void Main ()
{
	if (Fork () == 0) {

		if (Fork () == 0) {

			SlowPrintf (7, "4444444444");		// process 4
			Exit ();
		}

		SlowPrintf (7, "2222222222");			// process 2
		Exit ();
	}

	if (Fork () == 0) {

		SlowPrintf (7, "3333333333");			// process 3
		Exit ();
	}

	SlowPrintf (7, "1111111111");				// process 1
	Exit ();
}
