/* Programming Assignment 1: Exercise E
 *
 * Study the program below.  This will be used for your next and final
 * exercise, so make sure you thoroughly understand why the execution
 * sequence of the processes is the way it is.
 *
 * Questions
 *
 * 1. Can you explain the order of what gets printed based on the code?
 * 	1 about to fork
 *	1 just forked 2
 *	1 about to fork
 *	1 just forked 3
 *	1 about to fork
 *	1 just forked 4
 *	1 yielding to 4
 *	4 starting
 *	4 yielding to 3
 *	3 starting
 * 	3 yielding to 2
 *	2 starting
 *	2 yielding to 1
 *	1 resumed by 2, yielding to 4
 *	4 resumed by 1, yielding to 3
 *	3 resumed by 4, yielding to 2
 *	2 resumed by 3, yielding to 1
 *	1 exiting
 *	2 exiting
 *	3 exiting
 *	4 exiting
 *	System exiting (normal)
 */

#include <stdio.h>
#include "aux.h"
#include "umix.h"

#define NUMPROCS 3

void handoff (int p);

void Main ()
{
	int i, p, c, r;

	for (i = 0, p = Getpid (); i < NUMPROCS; i++, p = c) {
		Printf ("%d about to fork\n", Getpid ());
		if ((c = Fork ()) == 0) {
			Printf ("%d starting\n", Getpid ());
			handoff (p);
			Printf ("%d exiting\n", Getpid ());
			Exit ();
		}
		Printf ("%d just forked %d\n", Getpid (), c);
	}

	Printf ("%d yielding to %d\n", Getpid (), c);
	r = Yield (c);
	Printf ("%d resumed by %d, yielding to %d\n", Getpid (), r, c);
	Yield (c);
	Printf ("%d exiting\n", Getpid ());
}

void handoff (p)
	int p;				// process to yield to
{
	int r;

	Printf ("%d yielding to %d\n", Getpid (), p);
	r = Yield (p);
	Printf ("%d resumed by %d, yielding to %d\n", Getpid (), r, p);
	Yield (p);
}
