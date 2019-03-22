// Author: Tianheng "Oliver" Liu

#include "../aux.h"
#include "../umix.h"
#include "../mycode4.h"

#define STACKSIZE 60000

void WriteAndYield(int p);
void CreateAndYield(int p);
void YieldTo(int p);
void DoNothing(int p);

void Main() {
	int i;
	int t;
  /*
	Printf("*************************************************************\n");
	Printf("Reference of t5.\n");
	Printf("*************************************************************\n");
*/
	InitThreads();

	for(i = 0; i < MAXTHREADS; i++) {
		if(i == 0) {		// thread 1
			CreateThread(CreateAndYield, GetThread());

		} else if(i == 1) {	// thread 2
			t = CreateThread(YieldTo, GetThread());

		} else {
			CreateThread(DoNothing, -1);
		}
	}

	char stack[STACKSIZE];
	if(((int)&stack[STACKSIZE - 1]) - ((int)&stack[0]) + 1 != STACKSIZE) {
		Printf("Failed to create the stack\n");
		Exit();
	}

	YieldTo(t);
	
	Printf("Thread %d: exiting.\n", GetThread());
	ExitThread();
}

void WriteAndYield(p)
	int p;
{
	int i;
	char stack[STACKSIZE];

	Printf("Thread %d: writing on my own stack.\n", GetThread());

	for(i = 0; i < STACKSIZE; i++) {
		stack[i] = 'c';
	}

	Printf("Thread %d: yielding to %d.\n", GetThread(), p);
	YieldThread(p);
}


void CreateAndYield(p)
	int p;
{
	int something = 1;
	int t;
	Printf("Thread %d: created something = %d.\t***\n", GetThread(), something);
	Printf(
		"Thread %d: creating and yielding to thread %d.\n",
		GetThread(),
		(t = CreateThread(WriteAndYield, GetThread()))
	);

	t = YieldThread(t);

	Printf("Thread %d: resumed by %d.\n", GetThread(), t);
	Printf("Thread %d: my something is now %d.\t***\n", GetThread(), something);
}

void YieldTo(p)
	int p;
{
	int t;

	Printf("Thread %d: yielding to thread %d.\n", GetThread(), p);

	t = YieldThread(p);

	Printf("Thread %d: resumed by thread %d.\n", GetThread(), t);
}

void DoNothing(p)
	int p;
{
	// do nothing
}
