/* Copyright 1989-2018, Joseph Pasquale, University of California, San Diego
 *
 *	Umix thread package
 *
 */

#define MAXTHREADS	10			// maximum number of threads

void MyInitThreads ();				// initialize thread system
int MyCreateThread (void (*f)(), int p);	// create a thread to run f (p)
int MyGetThread ();				// return ID of current thread
int MyYieldThread (int t);			// yield to thread t
void MySchedThread ();				// yield to scheduler
void MyExitThread ();				// exit this thread

/*	Working version of above functions (reference thread system)
 */

extern void InitThreads ();			// initialize thread system
extern int CreateThread (void (*f)(), int p);	// create a thread to run f (p)
extern int GetThread ();			// return ID of current thread
extern int YieldThread (int t);			// yield to thread t
extern void SchedThread ();			// yield to scheduler
extern void ExitThread ();			// exit this thread
