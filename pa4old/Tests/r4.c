// Author: Tianheng "Oliver" Liu

#include "../aux.h"
#include "../umix.h"
#include "../mycode4.h"

void DoNothing(int p);
void YieldTo(int p);
void CreateThreeThreads(int p);

void Main() {
	int i, t;
/*
	Printf("*************************************************************\n");
	Printf("Reference of t4\n");
	Printf("*************************************************************\n");
*/
	InitThreads();

	for(i = 0; i < 6; i++) {
		if(i == 1 || i == 4) {	// thread 2 and 5
			t = CreateThread(DoNothing, -1);
			
		} else if(i == 5) {		// thread 6
			t = CreateThread(CreateThreeThreads, -1);

		} else {
			t = CreateThread(YieldTo, GetThread());
		}
		Printf("Thread %d: created thread %d.\n", GetThread(), t);
	}

	YieldThread(2);

	YieldThread(5);

	for(i = 0; i < 4; i++) {
		t = CreateThread(DoNothing, -1);
		Printf("Thread %d: created thread %d.\n", GetThread(), t);
	}

	YieldThread(3);

	ExitThread();

}

void DoNothing(p)
	int p;
{
	// do nothing
}

void YieldTo(p)
	int p;
{
	YieldThread(p);
}

void CreateThreeThreads(p)
	int p;
{
	int i;
	int t;

	for(i = 0; i < 3; i++) {
		t = CreateThread(DoNothing, -1);
		Printf("Thread %d: created thread %d.\n", GetThread(), t);
	}
}
