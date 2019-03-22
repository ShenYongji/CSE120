/* Programming Assignment 1: Exercise A
 *
 * In this first exercise, you will learn how to use the UMIX (which stands
 * for "User Mode UNIX") CSE 120 instructional operating system and how to
 * create new processes.  Programs for this operating system must be written
 * in C.  UMIX is similar to UNIX, but there are of course differences.  One
 * cosmetic difference is that the main function is capitalized, i.e., "Main".
 * The same is true of all system calls.  In addition, it is recommended that
 * you use Printf/DPrintf, rather than printf, as the former immediately
 * flushes buffered output, thus avoiding the potential of unusual sequences
 * of combined output from multiple processes (you may wish to experiment
 * with trying both printf and Printf/DPrintf to see this behavior).
 *
 * This system uses UNIX-style processes: each process has a single thread
 * of control and its own private memory.  The Fork () system call is used
 * to create a process.  The process that calls Fork () is called the parent,
 * and the new process is called the child.  Fork () creates the child with
 * a memory that is almost identical to that of the parent, with the child
 * starting its execution by returning from Fork ().  In other words, after
 * the call to Fork (), there are two processes, and each has just returned
 * from Fork ().  The main (and important) difference is that, in the child
 * process, the return value from Fork () is 0, while in the parent, the
 * return value is the process identifier (pid) of the child.  This is
 * illustrated in the simple program below.  Notice the additional system
 * calls, including Getpid () which returns the pid of the calling process,
 * and Exit () which causes the calling process to be destroyed.
 *
 * Run the program below.  To do this, use the supplied Makefile and run
 * make.  You must run this from within your CSE 120 account, as this will
 * cause the compiler to use special libraries that work for the CSE 120
 * operating system.
 *
 * Things to think about
 *
 * 1. For this OS, the parent always continues to run after a call to
 * Fork () (despite that the child process exists and can potentially run
 * in competition with the parent), but this is simply an artifact of the
 * implementation.  For most UNIX OS's systems, once fork () is called,
 * the choice of which process actually runs is arbitrary and is determined
 * by the OS scheduler.  In the verson of UMIX for this first assignment,
 * the choice happens to be to always run the parent.  In some OS's, the
 * choice can be virtually random.
 *
 * 2. Notice the call to Exit () in the child.  If Exit () were not called,
 * the child would continue running beyond the if clause (executing the
 * statements that print the parent's identity, etc.).
 *
 * 3. When Fork () is called, the child's memory looks just like that of the
 * parent.  Thus, since the value of pid is 0 when Fork () is called, the
 * child will inherit this variable and its value (0).  Note that pid is
 * set AFTER Fork () returns.  In the parent, it will be set to the process
 * identifier of the child, and in the child, it will be set to zero, because
 * these are the semantics of Fork ().  Thus, for the child to learn its
 * identifier, it must call Getpid ().
 *
 * Review questions:
 *
 * 1. Change the program to print the value of pid in the code executed by the
 * child.  What does it print, and why?
 *
 * 2. Remove the Exit () statement.  What happens, and why?
 *
 */

#include <stdio.h>
#include "aux.h"
#include "umix.h"

void Main ()
{
	int pid;

	if ((pid = Fork ()) == 0) {

		/* child executes here */
		Printf ("I am the child, my pid is %d\n", Getpid ());
		Exit ();
	}

	Printf ("I am the parent, my pid is %d\n", Getpid ());
	Printf ("I just created a child process whose pid is %d\n", pid);
}
