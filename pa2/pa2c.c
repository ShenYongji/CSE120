/* Programming Assignment 2: Exercise C
 *
 * In this third and final set of exercises, you will experiment with
 * preemptive scheduling.  We now return to the file mycode2.c, and study
 * the procedures that were briefly mentioned but not discussed in detail
 * in Exercise B.
 *
 * HandleTimerIntr () is called by the kernel whenever a timer interrupt
 * occurs.  The system has an interval timer that can be set to interrupt
 * after a specified time.  This is done by calling SetTimer (t).  Notice
 * that the first thing that HandleTimerIntr () does is to reset the timer
 * to go off again in the future (otherwise no more timer interrupts would
 * occur).  Depending on the policy (something for you to think about), it
 * may then call DoSched (), which informs the kernel to make a scheduling
 * decision at the next opportune time, at which point the kernel will
 * generate a call to SchedProc () to select the next process to run, and
 * then switch to that process.
 *
 * MyRequestCPUrate (int p, int n) is called by the kernel whenever a process
 * with PID p calls RequestCPUrate (int n), which is a system call that allows
 * a process to request that it should be scheduled to run n > 0 out of every
 * 100 quantums (i.e., n% of the time).  By default, processes run at whatever
 * rate is determined by the scheduler (which generally depends on what other
 * processes are in the system and what they are doing), and so a process
 * does not generally have any guarantee of how much CPU time it gets.
 * RequestCPUrate (n) addresses this by giving the calling process such a
 * guarantee.  Allowable values of n are 0 to 100, where positive (non-zero)
 * values indicate a request to run at n% of the CPU's execution rate, and
 * 0 indicates that it should run at no specific guaranteed rate (the same
 * as the default situation, when a process first starts running).  The
 * request with n = 0 is typically used to "undo" a process's previous
 * request where some guaranteed rate was previously in place, and after
 * the n = 0 request is made, the guaranteed rate is no longer in effect.
 *
 * Consider the following example.  If a process wants to run at 33% of the
 * CPU's execution rate, it can call RequestCPUrate (33), thus asking that
 * it run 33 out of every 100 quantums.  What happens in the kernel is that
 * when a process p calls RequestCPUrate (n), the kernel is entered, and the
 * kernel calls your MyRequestCPUrate (p, n) in mycode2.c, giving you the
 * opportunity to implement the way in which proportional share is to be
 * achieved.  Continuing with the example, if the process were to later call
 * RequestCPUrate (0), the process no longer gets the 33% proporational rate;
 * it just gets whatever CPU is made available (more about this below).
 *
 * MyRequestCPUrate (p, n) should return 0 if successful, and -1 if it fails.
 * MyRequestCPUrate (p, n) should fail if n is invalid (n < 0 or n > 100).
 * It should also fail if a process calls RequestCPUrate (n) such that it
 * would result in over-allocating the CPU.  Over-allocation occurs if the
 * sum of the rates requested by processes exceeds 100%.  If the call fails
 * (for whatever reason), MyRequestCPUrate (p, n) should have NO EFFECT, as
 * if the call to MyRequestCPUrate (p, n) were never made; thus, it should
 * not affect the scheduling of other processes, nor that of the calling
 * process.  Note that when a process exits, its portion of the CPU is
 * released and is available to other processes.  A process may change its
 * allocation by again calling RequestCPUrate (n) with a different value
 * for n > 0, or go back to an unallocated rate with n = 0.
 *
 * IMPORTANT: If the sum of the requested rates does not equal 100%, then
 * the remaining fraction should be allocated to processes that have not
 * made rate requests (or ones that made only failing rate requests).  This
 * is important, as a process needs some CPU time just to be able to execute
 * to be able to actually call RequestCPUrate (n).  A good policy for
 * allocating the unrequested portion is to spread it as evenly as possible
 * amongst processes that still have not made (or have only made failed)
 * rate requests.
 *
 * Here's an example that should help clarify the above points, including
 * what to do with unrequested CPU time and what happens when requests fail.
 * Consider the following sequence of 5 processes A, B, C, D, E, F entering
 * the system and some making CPU requests:
 *
 *	- A enters the system: A is able to use 100% of the CPU since there
 *	  are no other processes
 *	- B enters the system: B shares the CPU with A; both get an equal
 *	  amount, 50% each
 *	- B requests 40%: since there is at least 40% unrequested (in fact,
 *	  there is 100% unrequested), B gets 40%; A now gets the remaining 60%
 *	- C enters the system: it shares the unrequested 60% with A (both
 *	  get 30%)
 *	- C requests 50%: since there is at least 50% unrequested (in fact,
 *	  there is 60% unrequested), C gets 50%; A now gets the remaining 10%
 *	- B requests 0%: this actually means it wants to give up its 40%
 *	  guaranteed rate, and go back to competing for whatever cycles it
 *	  can get; C still keeps its 50%, and A and B compete for the
 *	  remaining 50%, thus each getting 25%.
 *	- B requests 40% again; just as when it made this request earlier,
 *	  since there is at least 40% unrequested (in fact, there is 50%
 *	  unrequested which it is sharing with A), B gets 40%; A now gets
 *	  the remaining 10%
 *	- D enters the system: it shares the unrequested 10% with A (both
 *	  get 5%)
 *	- D requests 20%: the request fails, and so D is treated as if it
 *	  never made the request; A and D continue to share 10% (both get 5%)
 *	- D requests 10%: since there is at least 10% unrequested (in fact,
 *	  there is exactly 10% unrequested), D gets 10%; A now gets the
 *	  remaining 0%, i.e., it does not get any CPU time
 *	- E enters the system: it shares the unrequested 0% with A (both
 *	  get zero CPU time, i.e., neither can run)
 *	- D exits, freeing up 10%, which A and E now share (A and E both
 *	  get 5%)
 *	- B requests 30%: it had 40%, but it is OK to make another request,
 *	  which in this case causes B to get 30% from this point, and 10%
 *	  is made available to the unrequesting processes
 *	- A exits, and so E gets the remaining 20%
 *	- E exits, and now there are only processes B (which is getting 30%)
 *	  and C (which is getting 50%).  These processes have no expectation
 *	  of additional CPU time, so the remaining 20% may be allocated any
 *	  way you want: it can be allocated evenly, proportionally, randomly,
 *	  and even not at all! As long as a process gets (at least) what it
 *	  requested, the kernel considers it satisfied.
 *
 * SetTimer (int t) will cause the timer to interrupt after t timer ticks.
 * A timer tick is a system-dependent time interval (and is 10 msecs in the
 * current implementation).  Once the timer is set, it begins counting down.
 * When it reaches 0, a timer interrupt is generated (and the kernel will
 * automatically call HandleTimerIntr ()).  The timer is then stopped until
 * a call to SetTimer (t) is made.  Thus, to cause a new interrupt to go
 * off in the future, the timer must be reset by calling SetTimer (t).
 *
 * GetTimer () will return the current value of the timer.
 *
 *
 * Exercises
 *
 * 1. Set the TIMERINTERVAL to 1, and run the program below using the three
 * existing scheduling policies: ARBITRARY, FIFO, and LIFO.  (Note that the
 * calls to RequestCPUrate (n) will be ignored since this procedure is not
 * relevant to these policies.)  What is the effect on the outputs, and why?
 *
 * 2. Implement the ROUNDROBIN scheduling policy.  This means that each
 * process should get a turn whenever a scheduling decision is made.  For
 * ROUNDROBIN to be effective, the timer interrupt period must be small.
 * With the TIMERINTERVAL set to 1 (the smallest possible value), you
 * should then see that the outputs of the processes will be interleaved,
 * as in: 1234123412341234123412341234123412341234 (not necessarily perfectly
 * ordered as shown.  Why?  Hint: Distinguish between a fixed amount of time
 * and the time needed to execute the instructions to print out a character,
 * which cannot be expected to precisely match.)
 *
 * 3. Try larger values for TIMERINTERVAL, such as 10, 100, and 1000.  What
 * is the effect on the interleaving of the output, and why?
 * 
 * 4. Implement the PROPORTIONAL scheduling policy.  This allows processes
 * to call RequestCPUrate (n) to receive a fraction of CPU time equal to n%,
 * i.e., n out of every 100 quantums.  For example, consider the four
 * processes in the program below, where process 1 requests 40% of the CPU
 * by calling RequestCPUrate (40), process 2 requests 30% of the CPU by
 * calling RequestCPUrate (30), process 3 requests 20% of the CPU by calling
 * RequestCPUrate (20), and process 4 requests 10% of the CPU by calling
 * RequestCPUrate (10).  With TIMERINTERVAL set to 1, this should ideally
 * produce an interleaving of the processes' outputs where ratio of
 * characters printed by processes 1, 2, 3, and 4, are 4 to 3 to 2 to 1,
 * respectively.  A sample output is as follows:
 * 121312412312131241231213124123121312412312132423232423343343343444444444
 * NOTE: THIS IS JUST A SAMPLE, YOUR OUTPUT MAY DIFFER FROM THIS!
 *
 * Your solution should work with any number of processes (up to MAXPROCS)
 * that have each called RequestCPUrate (n).  You should allow any process
 * to call RequestCPUrate (n) multiple times, which would change its share.
 * RequestCPUrate should fail if n < 0 or n > 100, or if n would cause the
 * overall CPU allocation to exceed 100%.  If the call fails, then it should
 * have no effect (as if the call were never made).  For any process that
 * does not call RequestCPUrate (n), that process should get any left-over
 * cycles (unless 100% were requested, then it would get none).
 *
 * A valid solution MUST have the following properties:
 *
 * 1. After a process successfully calls RequestCPUrate (n), that process
 * should utilize at least n% of the CPU over the time period measured from
 * when the call is made to when the process exits (or when a new successful
 * call is made, at which point a new period of measurement begins; if the
 * call is not successful, then the prevous request remains in force).
 *
 * 2. 100 quantums will be used as the maximum allowable time over which the
 * target n% CPU utilization must be achieved.  Furthermore, you will be
 * allowed a 10% slack in how close you come to n% from the low end, meaning
 * that your solution will be considered correct if the actual utilization
 * of EACH AND EVERY process is at least 90% of its requested n%.
 * For example, if 3 processes request and are allocated 50%, 30% and 10%,
 * respectively, their measured utilizations MUST be as least 45% (since 10%
 * of 50% is 5%, and 50%-5% = 45%), 27%, and 9%, respectively.  There is no
 * limit as to how much any of the requesting processes can get above their
 * requests, as long as (1) all the other requesting processes get at least
 * their lower bound, and (2) if the sum of the requests is below 100% and
 * there are k non-requesting processes, then each of those processes should
 * get a chance to run at least once every (k * 100) quantums.
 *
 * 3. Unused CPU time should be EQUALLY distributed to any remaining
 * non-requesting processes that have not requested CPU time.  "Equally
 * distributed" means that each process should get a chance to run
 * before any other gets a second chance.
 * 
 * 4. You should only use integer operations; NO FLOATING POINT ALLOWED.
 *
 * You must turn in your version of mycode2.c, with all the scheduling
 * policies implemented.  You must set TIMERINTERVAL to 1, which must work
 * for ALL of your policies.
 */

#include <stdio.h>
#include "aux.h"
#include "umix.h"

void Main ()
{
	if (Fork () == 0) {

		if (Fork () == 0) {

			RequestCPUrate (10);			// process 4
			SlowPrintf (7, "444444444444444444");
			Exit ();
		}

		RequestCPUrate (30);				// process 2
		SlowPrintf (7, "222222222222222222");
		Exit ();
	}

	if (Fork () == 0) {

		RequestCPUrate (20);				// process 3
		SlowPrintf (7, "333333333333333333");
		Exit ();
	}

	RequestCPUrate (40);					// process 1
	SlowPrintf (7, "111111111111111111");
	Exit ();
}
