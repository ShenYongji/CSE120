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
	Printf("Reference of t3.\n");
	Printf("*************************************************************\n");
*/
	InitThreads();

	for(i = 0; i < 7; i++) {
		if(i == 2) {		// thread 3
			CreateThread(YieldTo, 7);

		} else if(i == 6) {	// thread 7
			CreateThread(YieldTo, GetThread());

		} else {
			CreateThread(DoNothing, -1);
		}
	}

	YieldThread(7);

	ExitThread();
}

void YieldTo(p)
	int p;
{
	int x;

	Printf("Thread %d: yielding to thread %d.\n", GetThread(), p);
	x = YieldThread(p);
	Printf("Thread %d: resumed by thread %d.\n", GetThread(), x);
}

void DoNothing(p)
	int p;
{
	// do nothing
}
