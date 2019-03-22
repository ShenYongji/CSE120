// Author: Tianheng "Oliver" Liu

#include "../aux.h"
#include "../umix.h"
#include "../mycode4.h"

void YieldTo(int p);
void DoNothing(int p);

void Main() {
	int i;
/*
	Printf("*************************************************************\n");
	Printf("This test implements:\n");
	Printf(" * To clarify this further, consider a program of many\n");
	Printf(" * threads, two of which are threads 3 and 7.");
	Printf(" * \n");
	Printf(" * Thread 3 contains the statement:\n");
	Printf(" * \n");
	Printf(" * x = MyYieldThread (7);  // thread 3 yields to thread 7\n");
	Printf(" * \n");
	Printf(" * Thread 7 contains the statement:\n");
	Printf(" * \n");
	Printf(" * x = MyYieldThread (t);  // thread 7 yields to thread t\n");
	Printf(" * \n");
	Printf(" * Assume that at some point in the past, thread 7 had run\n");
	Printf(" * and had executed its yield statement as shown above. To\n");
	Printf(" * what it yields to is not shown as it depends on the value\n");
	Printf(" * of t.  If t equalled 3, then control would have gone to\n");
	Printf(" * thread 3.  If it were another value, another thread would\n");
	Printf(" * have gotten control.  Regardless, assume that thread 3 is\n");
	Printf(" * now running, and executes its yield statement as shown\n");
	Printf(" * above.  At this point, control is given to thread 7, which\n");
	Printf(" * returns from its yield statement with x set to 3 (because\n");
	Printf(" * it was thread 3 that yielded to thread 7).  The value of\n");
	Printf(" * x that will be set in thread 3 when its yield statement\n");
	Printf(" * returns will depend on whatever other thread eventually\n");
	Printf(" * yields to it.\n");
	Printf("Compare the output with the output of r3.\n");
	Printf("*************************************************************\n");
*/
	MyInitThreads();

	for(i = 0; i < 7; i++) {
		if(i == 2) {		// thread 3
			MyCreateThread(YieldTo, 7);

		} else if(i == 6) {	// thread 7
			MyCreateThread(YieldTo, MyGetThread());

		} else {
			MyCreateThread(DoNothing, -1);
		}
	}

	MyYieldThread(7);

	MyExitThread();
}

void YieldTo(p)
	int p;
{
	int x;

	Printf("Thread %d: yielding to thread %d.\n", MyGetThread(), p);
	x = MyYieldThread(p);
	Printf("Thread %d: resumed by thread %d.\n", MyGetThread(), x);
}

void DoNothing(p)
	int p;
{
	// do nothing
}
