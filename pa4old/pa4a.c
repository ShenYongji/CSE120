/* Programming Assignment 4: Exercise A
 *
 * In this assignment, you will implement a thread package.
 *
 * In this first exercise, you will learn how to use some basic mechanisms
 * for building threads, specifically, the functions setjmp and longjmp.
 * These are standard C library functions that support "non-local jumps".
 * You can learn more about them by reading their man pages ("man setjmp"
 * and "man longjmp"), though you should be able to do this assignment
 * solely with the information provided here.
 *
 * setjmp (env) causes a process to save parts of its context in env, which
 * is a "jmp_buf" structure defined in <setjmp.h>.  (What is actually saved
 * in env is machine architecture and compiler dependent; examples include
 * the PC, SP, FP, return address, etc.  We need not be concerned with these
 * details here, though you may find these examples helpful to answer the
 * "challenge questions" below.)  setjmp (env) returns twice (similar to
 * Fork ()), returning 0 the first time, and a value other than 0 the second
 * time.  More on this below.
 *
 * longjmp (env, t) causes a process to do a non-local jump to the location
 * where setjmp (env) was called.  This is similar to a goto instruction,
 * except that it is non-local, i.e., transfer may be across previous
 * procedure calls that have not returned yet.  For example, given three
 * procedures A, B, and C, say setjmp (env) is called within A, and then
 * A calls B.  If B then calls C, and then longjmp (env, t) is called within
 * C, control returns to the point where setjmp (env) was previously called
 * by A.  Thus, the call stack is reduced because A then continues to run as
 * if B and C had returned (they are no longer pending).  This is because env
 * contains the stack pointer SP as it was when setjmp was called.
 *
 * It is VERY IMPORTANT to note that longjmp (env, t) can only jump into a
 * pending procedure that had called setjmp (env).  In our example, A called
 * setjmp (env) and THEN called B, which THEN called C, which THEN called
 * longjmp (env, t).  Note that A did not return before longjmp (env, t) was
 * called.  If A had called setjmp (env), but returned (to whatever called
 * it), a later call to longjmp (env, t) would generally not work properly
 * because the context described by env no longer exists; in particular,
 * the activation record for A is gone.
 *
 * To be more specific as to where control transfers after a call to
 * longjmp (env, t), it is to a location actually WITHIN the previous call
 * to setjmp (env).  Thus, setjmp (env) will return twice.  The first time
 * it returns, it had just set env and returns a value of 0.  When it returns
 * the second time as a result of calling longjmp (env, t), it (i.e., setjmp)
 * returns the value t if it is not 0, and 1 if t is 0.  How you use t is up
 * to you.
 *
 * An example will clarify the above discussion.  Study the program below.
 * To run the program, use the supplied Makefile and run make.
 *
 * Questions
 *
 * 1. Can you account for the output order: A: t=1, B: t=2, C: t=4, D: t=5?
 *
 * 2. Make the following minimal changes so that the order is ACBD rather
 * than ABCD: (a) change the test in the conditional to != rather than ==,
 * and (b) move the longjmp statement so that it is just after Point C.
 * Can you explain why the printed values of t are 1, 2, 2, 3?
 *
 * 3. Change the test in the conditional back to ==, and move the longjmp
 * statement to just after Point D.  Can you account for the process's
 * behavior?
 *
 * If you were able to answer the questions above, you now have a basic
 * understanding for how setjmp and longjmp works.  You may now proceed to
 * Exercise B, where you will build on this knowledge, and things will get
 * more difficult, and more interesting.  For those who want an additional
 * challenge (not necessary to complete the rest of the assignment), see
 * if you can answer the following optional questions.
 *
 * 4. Change back to the original setup (conditional test is ==, longjmp
 * is immediately after Point B).  Change setjmp to Setjmp and longjmp to
 * Longjmp.  What do Setjmp and Longjmp do?  When you run the program, you
 * should notice different behavior; why?  Hint: consider how the stack of
 * activation records changes over time.
 *
 * 5. Now change Setjmp to Setjmp1 and Longjmp to Longjmp1.  When you run
 * the program, do you notice any different behavior?  Can you explain why?
 * Hint: this behavior can only be justified by considering what must be
 * saved in (and restored from) env.
 *
 */

#include <setjmp.h>
#include "aux.h"
#include "umix.h"

void Main ()
{
	jmp_buf env;
	int t = 1;
	int Setjmp (), Setjmp1 (), Longjmp (), Longjmp1 ();

	Printf ("A: t = %d\n", t);			// Point A

	if ((t = setjmp (env)) == 0) {		// conditional test
		t = 2;
		Printf ("B: t = %d\n", t);		// Point B
		longjmp (env, t);
	} else {
		t = t + 2;
		Printf ("C: t = %d\n", t);		// Point C
	}
	t = t + 1;
	Printf ("D: t = %d\n", t);			// Point D
}

int Setjmp (env)
	jmp_buf env;			// to contain saved state
{
	Printf ("Inside Setjmp\n");
	return (Setjmp1 (env));
}

int Setjmp1 (env)
	jmp_buf env;			// to contain saved state
{
	Printf ("Inside Setjmp1\n");
	return (setjmp (env));
}

int Longjmp (env, t)
	jmp_buf env;			// state to restore
	int t;				// thread to resume
{
	Printf ("Inside Longjmp\n");
	Longjmp1 (env, t);
}

int Longjmp1 (env, t)
	jmp_buf env;				// state to restore
	int t;					// thread to resume
{
	Printf ("Inside Longjmp1\n");
	longjmp (env, t);
}
