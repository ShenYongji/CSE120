// Author: Tianheng "Oliver" Liu

#include "../aux.h"
#include "../umix.h"
#include "../mycode4.h"

// notice this value is smaller than the STACKSIZE in mycode4.c
#define STACKSIZE 60000

void WriteAndYield(int p);
void CreateAndYield(int p);

void Main() {
	int t1, t2;

	InitThreads();
/*
	Printf("***************************************************************\n");
	Printf("Reference of t6.\n");
	Printf("***************************************************************\n");
*/
	if(1) {
		char stack[STACKSIZE];
		if(((int)&stack[STACKSIZE - 1]) - ((int)&stack[0]) + 1 != STACKSIZE) {
			Printf("Stack space reservation failed.\n");
			Exit();
		}

		t1 = CreateThread(WriteAndYield, GetThread());
	}

	t2 = CreateThread(CreateAndYield, GetThread());

	YieldThread(t2);

	YieldThread(t1);
	
	YieldThread(t2);

	ExitThread();
}

void WriteAndYield(p)
	int p;
{
	int i;
	Printf("Thread %d: writing on my own stack.\n", GetThread());
	char stack[STACKSIZE];
	for(i = 0; i < STACKSIZE; i++) {
		stack[i] = 'c';
	}

	YieldThread(p);
}

void CreateAndYield(p)
	int p;
{
	int something = 1;
	Printf("Thread %d: created something = %d.\t***\n", GetThread(), something);

	YieldThread(p);

	Printf("Thread %d: my something is now: %d.\t***\n", GetThread(), something);
}
