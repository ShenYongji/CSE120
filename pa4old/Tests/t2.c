#include "../aux.h"
#include "../umix.h"
#include "../mycode4.h"
#include "stdio.h"

void Main() {
/*
	Printf("*************************************************************\n");
	Printf("Each thread creates and yields to the next thread, then\n");
	Printf("yields back to the previous thread.\n");
	Printf("Thread 0 should exit first. Then threads 9-0 exit in order.\n");
	Printf("Put a debug statment in MyExitThread() to check the exiting\n");
	Printf("order.\n");
	Printf("Check the output with the output of r2.\n");
	Printf("*************************************************************\n");
*/
	MyInitThreads();

	void CreateAndRunThread();

	MyYieldThread(MyCreateThread(CreateAndRunThread, MyGetThread()));

	MyExitThread();

}

void CreateAndRunThread(p)
	int p;
{
	int n;
	Printf(
		"Thread %d: creating and yielding to thread %d.\n",
		MyGetThread(),
		(n = MyCreateThread(CreateAndRunThread, MyGetThread()))
	);

	MyYieldThread(n);

	Printf("Thread %d: yielding back to %d.\n", MyGetThread(), p);
	MyYieldThread(p);
}
