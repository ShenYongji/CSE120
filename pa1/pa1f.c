/* Programming Assignment 1: Exercise F
 * 
 * We are finally ready to modify the UMIX operating system kernel!
 * Up until now, you have studied and modified only user programs that
 * make system calls, but not the kernel.  Here, you will make your first
 * addition to the kernel by implementing the all-important mechanism of
 * context switching.
 * 
 * Whenever Yield (p) is called, the kernel is entered and eventually calls
 * MySwitchContext (p), which causes a context switch to process p, and
 * returns the process ID of the process that just called Yield (p).  This
 * function can be found in the file mycode1.c, which will contain all
 * of YOUR modifications to the kernel.  If you look at MySwitchContext (p),
 * you will see that it calls yet another function SwitchContext (p), which
 * is an internal kernel function that actually does the context switch, and
 * returns whatever SwitchContext (p) returns.  Without SwitchContext (p),
 * the kernel would not have worked properly up until now, and we needed it
 * to be able to run all the programs shown so far.  In this exercise, you
 * will REMOVE the call to SwitchContext (p), which will break the kernel,
 * and in its place, add your own code to make the kernel work properly again.
 * 
 * MySwitchContext (p) should cause a context switch from the currently
 * running process to process p.  To implement MySwitchContext (p), you
 * are given three utility functions:
 *
 * void SaveContext (CONTEXT *c)
 *
 *	Saves the context of the currently running process into a CONTEXT
 *	variable (CONTEXT is a special data type in UMIX) pointed to by c.
 *	This context comprises the current values of registers, including
 *	the SP (stack pointer) and the PC (program counter), though you
 *	need not be concerned with these details.
 *
 * RestoreContext (CONTEXT *c)
 *
 *	Restores the context of the process whose previously saved context
 *	is in the CONTEXT variable pointed to by c.  This saved context is
 *	the result of either (1) a call to SaveContext (c) at some point
 *	in the past (and whatever process was running at that point will
 *	be the one restored), or (2) a call to NewContext (p, c) that was
 *	called when the process was created but has not had a chance to
 *	run yet.  NewContext (p, c) is described below.
 *
 * int GetCurProc ()
 *
 *	Simply returns the ID of the currently running process.
 * 
 * Consider what happens when a process's context is restored.  It begins
 * executing from wherever the PC was pointing to when its state was saved,
 * specifically somewhere within SaveContext (c).  Therefore, SaveContext (c)
 * will return TWICE, even though it was called only once!  The first time
 * corresponds to when SaveContext (c) was explicitly called, and the second
 * time is when the process's context is restored (as a result of calling
 * RestoreContext (c)).  YOUR JOB is to find a way of distinguishing
 * between returns so that your code can determine whether or not
 * RestoreContext (c) should be called.  (Hint: study the Lecture notes
 * on context switching.  If you understand the notes, you will know how to
 * write this code).
 *
 * As both SaveContext (c) and RestoreContext (c) make use of contexts,
 * you are provided with a data type (a typedef structure in C) named CONTEXT,
 * defined in sys.h.  You can use this to declare variables to store process
 * contexts as follows.  First, declare a variable of type CONTEXT:
 *
 * 	CONTEXT ctxt;
 *
 * You can then call SaveContext (&ctxt), which will store the context of the
 * currently running process into the variable ctxt.  If you wish to restore
 * the context of a process whose context was previously saved into ctxt,
 * you can then call RestoreContext (&ctxt).  If you find it more
 * convenient to pass a pointer, you may do the following:
 *
 *	CONTEXT ctxt;
 *	CONTEXT *c = &ctxt;
 *
 *	SaveContext (c);	// which is equivalent to SaveContext (&ctxt)
 *
 * Note that you do not need to (nor should you) modify the actual CONTEXT
 * structure defined in sys.h, all you need to do is declare a variable of
 * type CONTEXT and use it by passing the variable to SaveContext (c) and
 * RestoreContext (c).
 *
 * You will need to keep track of ALL active processes, and so each will
 * require its own CONTEXT variable.  It's up to you as to what data
 * structure to use for this purpose (a typical data structure is a table,
 * i.e., an array).  The following two facts about UMIX will be helpful:
 *
 *	1. There is a maximum number of processes that UMIX will support;
 *	this number is defined by the constant MAXPROCS, which you may use
 *	in declaring your data structure.
 *
 *	2. Process IDs range from 1 to MAXPROCS.  Every time a new process
 *	is created, an ID that is currently not in use in that range will
 *	be assigned to that process.
 *
 * Finally, we must deal with the issue of how to restore the context of
 * a process that is newly created (i.e., the result of a Fork () system
 * call) but has not run yet.  Since it has not run, there was been no
 * call to SaveContext (c), and so there is no saved context variable from
 * which to restore.  This is solved with the function NewContext (p, c):
 *
 * NewContext (int p, CONTEXT *c)
 *
 *	This function is called by the kernel whenever a new process is
 *	created.  The process ID is p, and the INITIAL context is pointed
 *	to by c.  NewContext (p, c) is part of mycode1.c, and so it is
 *	a function that you get to implement.  The kernel is basically
 *	providing a notification that a new process was created by calling
 *	NewContext (p, c), and this is your opportunity to record that
 *	process p is a new process and that its corresponding context
 *	is pointed to by c.  You should take this opportunity by copying
 *	the context pointed to by c into whatever data structure you are
 *	maintaining for the contexts of all processes, making sure to
 *	associate it with process ID p, so that a future call to
 *	RestoreContext (c) (where c points to the context of this new
 *	process) will work.  Once NewContext (p, c) returns, you cannot
 *	count on c pointing to process p's context, so make sure you
 *	make a copy of it while in NewContext (p, c).
 *
 * You now have all the tools to implement NewContext (p, c) and
 * MySwitchContext (p).  Here are some hints to help implement them:
 *
 * NewContext
 *
 * You MAY use the memcpy (a, b, n) library function, which copies n bytes
 * from address b to address a.  You may use sizeof (CONTEXT) to determine
 * the number of bytes for a CONTEXT structure variable.
 *
 * MySwitchContext
 *
 * Remove the call to SwitchContext (p) so that the body of
 * MySwitchContext (p) is now empty, and then replace with YOUR code.
 * The only functions you will need for this are SaveContext (c),
 * RestoreContext (c), and GetCurProc ().  You should review how variables
 * get allocated in the data and stack memory areas (and how to effect this
 * via declarations in C, including the use of the keyword "static"),
 * favoring variables of minimal scope.  (Think about why this is important,
 * especially for a large complex program like an operating system where
 * many programmers may eventually modify it).  Also, you should NOT make
 * any system calls from within MySwitchContext (p), since system calls
 * are called by processes from outside the kernel, and MySwitchContext (p)
 * is called from inside the kernel.
 *
 * Next, make sure that MySwitchContext (p) RETURNS THE PROPER VALUE, which
 * is the ID of the process that just called Yield (p).  Getting this right
 * can be a bit tricky!  Verify that the output using your version of
 * MySwitchContext (p) matches the output of the original unmodified
 * MySwitchContext (p), INCLUDING output based on the return value.  You
 * should test your kernel by seeing if it will work with the program below,
 * as well as OTHER TESTS YOU DESIGN (and which you may share with others).
 *
 * Good luck!
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
			Exit (0);
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
