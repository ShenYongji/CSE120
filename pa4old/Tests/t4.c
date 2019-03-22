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
	Printf("This test implements:\n");
	Printf(" * Here is an example for clarification: say MAXTHREADS = 10,\n");
	Printf(" * and seven threads are created: 0, 1, 2, ..., 6. Next,\n");
	Printf(" * threads 2 and 5 exit. Next four threads are created: 7, 8,\n");
	Printf(" * 9, and 2. Since 0 and 1 still exist, those IDs are skipped\n");
	Printf(" * over. Next 0 and 3 exit. Next three threads are created.\n");
	Printf(" * Since the last ID assigned was 2, and since 3 is the next\n");
	Printf(" * available, the three threads are assigned the following\n");
	Printf(" * IDs: 3, 5 and 0.\n");
	Printf("Compare the output with the output of r4.\n");
	Printf("*************************************************************\n");
*/
	MyInitThreads();

	for(i = 0; i < 6; i++) {
		if(i == 1 || i == 4) {	// thread 2 and 5
			t = MyCreateThread(DoNothing, -1);

		} else if(i == 5) {		// thread 6
			t = MyCreateThread(CreateThreeThreads, -1);

		} else {
			t = MyCreateThread(YieldTo, MyGetThread());
		}
		Printf("Thread %d: created thread %d.\n", MyGetThread(), t);
	}

	MyYieldThread(2);

	MyYieldThread(5);

	for(i = 0; i < 4; i++) {
		t = MyCreateThread(DoNothing, -1);
		Printf("Thread %d: created thread %d.\n", MyGetThread(), t);
	}

	MyYieldThread(3);

	MyExitThread();

}

void DoNothing(p)
	int p;
{
	// do nothing
}

void YieldTo(p)
	int p;
{
	MyYieldThread(p);
}

void CreateThreeThreads(p)
	int p;
{
	int i;
	int t;

	for(i = 0; i < 3; i++) {
		t = MyCreateThread(DoNothing, -1);
		Printf("Thread %d: created thread %d.\n", MyGetThread(), t);
	}
}
