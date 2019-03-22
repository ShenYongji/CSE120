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
	Printf("Reference of t1.\n");
	Printf("*************************************************************\n");
*/
	InitThreads();

	Printf(
		"Thread %d: yielding to thread %d.\n",
		GetThread(),
		GetThread()
	);
	t = YieldThread(GetThread());
	Printf("Thread %d: resumed by thread %d.\n", GetThread(), t);

	t = CreateThread(YieldTo, GetThread());

	Printf("Thread %d: yielding to thread %d.\n", GetThread(), t);
	t = YieldThread(t);
	Printf("Thread %d: resumed by thread %d.\n", GetThread(), t);

	Printf(
		"Thread %d: yielding to thread %d.\n",
		GetThread(),
		GetThread()
	);
	t = YieldThread(GetThread());
	Printf("Thread %d: resumed by thread %d.\n", GetThread(), t);

	ExitThread();
}

void YieldTo(p)
	int p;
{
	int t;

	Printf("Thread %d: yielding to thread %d.\n",
		GetThread(),
		GetThread()
	);
	t = YieldThread(GetThread());
	Printf("Thread %d: resumed by thread %d.\n", GetThread(), t);

	Printf("Thread %d: yielding to thread %d.\n", GetThread(), p);
	t = YieldThread(p);
	Printf("Thread %d: resumed by thread %d.\n", GetThread(), t);

	Printf("Thread %d: yielding to thread %d.\n",
		GetThread(),
		GetThread()
	);
	t = YieldThread(GetThread());
	Printf("Thread %d: resumed by thread %d.\n", GetThread(), t);
}
