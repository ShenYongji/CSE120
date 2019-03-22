/* Programming Assignment 3: Exercise C
 *
 * How to Use Shared Memory
 *
 * Processes in Umix are able to share memory.  An explicit mechanism is
 * needed because the default for processes is that, as in Unix, their
 * memories are independent of each other, with no portion being shared.
 * When a Fork occurs, a new process is created with its own memory that
 * is initialized with the state of the memory of the creating process.
 * But from that point on, when one of the processes modifies its private
 * memory, the private memory of the other process is unaffected.  Hence,
 * the need for a shared memory area.
 *
 * Two processes can share memory by identifying and registering variables
 * to be shared.  The variables in each process should be the same size
 * and type.  To share a set of variables, one can simply define a struct
 * containing the variables as members and then share a variable declared
 * as the struct.  The following example will clarify this.
 *
 * For two processes to share, say, three integers, a struct should be
 * defined in both programs as follows:
 *
 * struct {
 *	int x, y, z;
 * } shm;
 *
 * This structure defines three integers x, y, and z, as its members, and
 * the variable shm is declared of this structure's type.
 *
 * Within the program, shm must be registered with the operating system
 * as a shared variable using the Regshm system call:
 *
 *	Regshm ((char *) &shm, sizeof (shm))
 *
 * The first parameter is the address of the shared (struct) variable, 
 * and the second is its size.  The latter is needed because there is a
 * maximum size (MAXSHM) that the operating system supports for shared
 * memory.
 *
 * From then on, you can reference any of the shared variables via the
 * expression shm.x, where shm is the name of the addressing variable
 * and x is a member of the shared variable structure.
 *
 * Variables that become shared memory should NOT be used prior to the call
 * to Regshm (or if they are used, their values will be wiped away after
 * the Regshm returns).  Consequently, all initialization of shared memory
 * variables should happen AFTER the call the Regshm, and not before.
 *
 * If a process has a registered shared memory area, then any processes
 * it creates will inherit the shared memory, i.e., the child processes
 * do not have to call Regshm.  Otherwise, processes need to each call
 * Regshm to share memory.
 *
 * If Regshm is called more than once, the latest registration supercedes
 * the previous ones (i.e., only the last one matters, the others are
 * ignored).  Typically, all the variables that should be shared are
 * placed in a struct as shown above, and then registered once, with
 * no more registrations.
 *
 * A sample program is given below.  Note that the output of the program
 * is actually indeterminate because without any synchronization (e.g.,
 * semaphores), Process 2 may print before Process 1 sets the variables.
 */

#include <stdio.h>
#include "aux.h"
#include "sys.h"
#include "umix.h"

struct {		// structure of variables to be shared
	int x;		// example of an integer variable
	char y[10];	// example of an array of character variables
} shm;

void Main ()
{
	Regshm ((char *) &shm, sizeof (shm));		// register as shared
    //Printf("sizeofshm: %d\n",sizeof(shm));
	if (Fork () == 0) {

		/* Proc 2 inherits the shared memory registered by Proc 1 */

		Printf ("P2: x = %d, y[3] = %c\n", shm.x, shm.y[3]);
		Exit ();
	}

	shm.x = 1062;
	shm.y[3] = 'a';
	Printf ("P1: x = %d, y[3] = %c\n", shm.x, shm.y[3]);
    //Printf("sizeofshm: %d\n",sizeof(shm));
}
