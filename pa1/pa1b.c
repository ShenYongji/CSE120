/* Programming Assignment 1: Exercise B
 *
 * Now we will expand the program of Exercise A to call Fork () multiple times.
 * Run the program below and answer the following questions:
 *
 * 1. Can you explain the order of what gets printed based on the code?
 *    	I am the parent, my pid is 1
 *	I just created a child process whose pid is 2
 * 	I (the parent) just created a second child process whose pid is 3
 * 	I am the first child, my pid is 2
 * 	I am the second child, my pid is 3
 * 2. Why do you think the first child executes before the second child?
 *
 * 3. Move the two print statements executed by the parent to just after
 * where it says HERE.  How can you print the pid of the first child?
 * 	Has to have a temporary variable to save the first child.
 *
 */

#include <stdio.h>
#include "aux.h"
#include "umix.h"

void Main ()
{
	int pid = 0;

	if ((pid = Fork ()) == 0) {

		/* first child executes here */
		Printf ("I am the first child, my pid is %d\n", Getpid ());
		Exit ();
	}

	Printf ("I am the parent, my pid is %d\n", Getpid ());
	//int first = pid;
	Printf ("I just created a child process whose pid is %d\n", pid);

	if ((pid = Fork ()) == 0) {

		/* second child executes here */
		Printf ("I am the second child, my pid is %d\n", Getpid ());
		Exit ();
	}
	//int second = pid;

	/* HERE */

	Printf ("I (the parent) just created a second child process whose pid is %d\n", pid);
	//Printf("-----------------------------------------------\n");
	//Printf ("I am the first child, my pid is %d\n", first);
	//Printf ("I am the second child, my pid is %d\n", second);
	//Printf("-----------------------------------------------\n");
}
