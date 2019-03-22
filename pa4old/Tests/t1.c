// Author: Tianheng "Oliver" Liu

#include "../aux.h"
#include "../umix.h"
#include "../mycode4.h"

void YieldTo(int p);
void DoNothing(int p);

void Main() {
	int t;
/*
	Printf("*************************************************************\n");
	Printf("Thread 0 yields to itself when there is no other thread.\n");
	Printf("Thread 0 creates thread 1 and yields to it.\n");
	Printf("Thread 1 yields to itself when there is thread 0.\n");
	Printf("Thread 1 yields to thread 0.\n");
	Printf("Thread 0 yiedls to itself when there is thread 1.\n");
	Printf("Thread 0 exits.\n");
	Printf("Thread 1 yields to itself when there is no other thread.\n");
	Printf("Compare the output with the output of r1.\n");
	Printf("*************************************************************\n");
*/
	MyInitThreads();

	Printf(
		"Thread %d: yielding to thread %d.\n",
		MyGetThread(),
		MyGetThread()
	);
	t = MyYieldThread(MyGetThread());
	Printf("Thread %d: resumed by thread %d.\n", MyGetThread(), t);

	t = MyCreateThread(YieldTo, MyGetThread());

	Printf("Thread %d: yielding to thread %d.\n", MyGetThread(), t);
	t = MyYieldThread(t);
	Printf("Thread %d: resumed by thread %d.\n", MyGetThread(), t);

	Printf(
		"Thread %d: yielding to thread %d.\n",
		MyGetThread(),
		MyGetThread()
	);
	t = MyYieldThread(MyGetThread());
	Printf("Thread %d: resumed by thread %d.\n", MyGetThread(), t);

	MyExitThread();
}

void YieldTo(p)
	int p;
{
	int t;

	Printf("Thread %d: yielding to thread %d.\n",
		MyGetThread(),
		MyGetThread()
	);
	t = MyYieldThread(MyGetThread());
	Printf("Thread %d: resumed by thread %d.\n", MyGetThread(), t);

	Printf("Thread %d: yielding to thread %d.\n", MyGetThread(), p);
	t = MyYieldThread(p);
	Printf("Thread %d: resumed by thread %d.\n", MyGetThread(), t);

	Printf("Thread %d: yielding to thread %d.\n",
		MyGetThread(),
		MyGetThread()
	);
	t = MyYieldThread(MyGetThread());
	Printf("Thread %d: resumed by thread %d.\n", MyGetThread(), t);
}
