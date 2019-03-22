/* Programming Assignment 1: Exercise D
 *
 * Let's continue using Yield (pid) to effect a variety of executions
 * sequences.  Using the code below, the sequence of the print statements
 * will be ABP:
 *
 *	First Child (A)
 *	Second Child (B)
 *	Parent (P)
 *
 *
 * Questions
 *
 * 1. See if you can cause a change in the sequence as specified below just by
 * ADDING Yield statements (i.e., do not remove any of the ORIGINAL Yield
 * statements, just add extra ones anywhere you wish):
 *
 * a. BAP
 * b. BPA
 * c. PAB
 * d. PBA
 * e. APB
 *
 */

#include <stdio.h>
#include "aux.h"
#include "umix.h"

void Main ()
{
	int ppid, pid1, pid2;

	ppid = Getpid ();
	if ((pid1 = Fork ()) == 0) {
		/* first child executes here */
        Yield(ppid);//BPA
        //Yield(ppid);//BAP
        //Yield(ppid);//PAB
        //Yield(ppid);//PBA
		Printf ("I am the first child, my pid is %d\n", Getpid ());
        
		Exit ();
	}
	
    Yield (pid1);	
    

	if ((pid2 = Fork ()) == 0) {
		/* second child executes here */
        //Yield(ppid);//PAB
        Yield(ppid);//PBA
        //Yield(ppid);//APB
		Printf ("I am the second child, my pid is %d\n", Getpid ());
		Exit ();
	}

	Yield (pid2);	// yield to second child before continuing
    //Yield (pid1);//BAP
    Printf ("I am the parent, my pid is %d\n", Getpid());
    Yield(pid2);//PBA

	
}
