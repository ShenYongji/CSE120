#include "../aux.h"
#include "../umix.h"
#include "../mycode4.h"
#include "stdio.h"

void Main() {
/*
	Printf("*************************************************************\n");
	Printf("Reference of t2.\n");
	Printf("*************************************************************\n");
*/
	InitThreads();

	void CreateAndRunThread();

	YieldThread(CreateThread(CreateAndRunThread, GetThread()));

	ExitThread();

}

void CreateAndRunThread(p)
	int p;
{
	int n;
	Printf(
		"Thread %d: creating and yielding to thread %d.\n",
		GetThread(),
		(n = CreateThread(CreateAndRunThread, GetThread()))
	);

	YieldThread(n);

	Printf("Thread %d: yielding back to %d.\n", GetThread(), p);
	YieldThread(p);
}
