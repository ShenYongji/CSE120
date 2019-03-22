/*   	Umix thread package
 *
 */

#include <setjmp.h>
#include <string.h>
#include "aux.h"
#include "mycode4.h"
#include "umix.h"

static int MyInitThreadsCalled = 0; // 1 if MyInitThreads called, else 0

static int threadIndex = 0;
static int curThread;
static int head;

static struct thread { // thread table
  int valid;           // 1 if entry is valid, else 0
  jmp_buf env;         // current context
  jmp_buf saved_env;

  void (*func)();
  int param;
  int next;
  int prev;
  int waker;
  int is_sched;
} thread[MAXTHREADS];

#define STACKSIZE 65536 // maximum size of thread stack

static int queue_is_empty() {
    return head == -1;
}

static void queue_print() {
    int h, cur = -1;
    if (head >= 0) {
        h = head;
        DPrintf("%d", h);
        cur = thread[h].next;
        while (cur != h) {
            DPrintf(" -> %d", cur);
            cur = thread[cur].next;
        }
    }
    DPrintf("\n");
}

static void queue_push(int i) {
    if (head < 0) {
        head = i;
        thread[i].next = thread[i].prev = i;
    } else if(thread[i].next < 0) {
        thread[i].prev = thread[head].prev;
        thread[i].next = head;
        thread[thread[head].prev].next = i;
        thread[head].prev = i;
    }
}

static int queue_pop() {
    int i = -1, tail;
    if (head < 0) {
        return -1;
    } else if (head == thread[head].next) {
        i = head;
        head = -1;
    } else {
        i = head;
        tail = thread[i].prev;
        thread[thread[head].next].prev = tail;
        head = thread[i].next;
        thread[tail].next = head;
    }
    thread[i].prev = thread[i].next = -1;
    return i;

}

static void queue_remove(int i) {
    if (thread[i].prev == -1 || thread[i].next == -1) {
        return;
    }
    thread[thread[i].prev].next = thread[i].next;
    thread[thread[i].next].prev = thread[i].prev;
    if (i == head) {
        head = thread[i].next;
        if (head == i) {
            head = -1;
        }
    }
    thread[i].prev = thread[i].next = -1;
}

static int only_one_thread() {
    return head != -1 && thread[head].next == head;
}

/* 	MyInitThreads () initializes the thread package. Must be the first
 *  	function called by any user program that uses the thread package.
 */

void MyInitThreads() {
  int i;

  if (MyInitThreadsCalled) { // run only once
    Printf("MyInitThreads: should be called only once\n");
    Exit();
  }

  for (i = 0; i < MAXTHREADS; i++) { // initialize thread table
    
    thread[i].valid = 0;
    thread[i].func = 0;
    thread[i].param = -1;
    thread[i].next = -1;
    thread[i].prev = -1;
    thread[i].waker = -1;
    thread[i].is_sched = 0;

    char stack[i * STACKSIZE];

    if (((int)&stack[i * STACKSIZE - 1]) - ((int)&stack[0]) + 1 !=
        i* STACKSIZE) {
      Printf("Stack space reservation failed\n");
      Exit();
    }

    if (setjmp(thread[i].env) != 0) {
        if (thread[curThread].func) {
            thread[curThread].func(thread[curThread].param);
        }
        MyExitThread();
    } 
    memcpy(thread[i].saved_env, thread[i].env, sizeof(jmp_buf));
  }

  thread[0].valid = 1; // initialize thread 0
  curThread = 0;
  head = -1;

  MyInitThreadsCalled = 1;
}

/*  	MyCreateThread (f, p) creates a new thread to execute
 * 	f (p), where f is a function with no return value and
 * 	p is an integer parameter.  The new thread does not begin
 *  	executing until another thread yields to it.
 */

int MyCreateThread(f, p) void (*f)(); // function to be executed
int p;                                // integer parameter
{
  int index;

  if (!MyInitThreadsCalled) {
    Printf("MyCreateThread: Must call MyInitThreads first\n");
    Exit();
  }
  index = threadIndex;
  while ((threadIndex = (threadIndex + 1) % MAXTHREADS) != index) {
    if (!thread[threadIndex].valid) {
      break;
    }
  }
  if (threadIndex == index) {
    return -1;
  }

  thread[threadIndex].func = f;
  thread[threadIndex].param = p;
  thread[threadIndex].valid = 1; // mark the entry for the new thread valid
  memcpy(thread[threadIndex].env, thread[threadIndex].saved_env, sizeof(jmp_buf));
 
  queue_push(threadIndex);

  return (threadIndex); // done, return new thread ID
}

/*   	MyYieldThread (t) causes the running thread, call it T, to yield to
 * 	thread t. Returns the ID of the thread that yielded to the calling
 *  	thread T, or -1 if t is an invalid ID.  Example: given two threads
 *  	with IDs 1 and 2, if thread 1 calls MyYieldThread (2), then thread 2
 * 	will resume, and if thread 2 then calls MyYieldThread (1), thread 1
 * 	will resume by returning from its call to MyYieldThread (2), which
 *  	will return the value 2.
 */

int MyYieldThread(t) int t; // thread being yielded to
{
  if (!MyInitThreadsCalled) {
    Printf("MyYieldThread: Must call MyInitThreads first\n");
    Exit();
  }

  if (t < 0 || t >= MAXTHREADS) {
    Printf("MyYieldThread: %d is not a valid thread ID\n", t);
    return (-1);
  }
  if (!thread[t].valid) {
    Printf("MyYieldThread: Thread %d does not exist\n", t);
    return (-1);
  }

  if (setjmp(thread[curThread].env) == 0) {
    if (thread[curThread].valid) {
        if (!thread[t].is_sched) {
            thread[t].waker = curThread;
        } else {
            queue_push(curThread);
        }
    } else {
        thread[t].is_sched = 0;
        if (thread[curThread].waker >= 0 && thread[thread[curThread].waker].valid) {
            queue_push(thread[curThread].waker);
        }
        thread[t].waker = -1;
    }
	curThread = t;
    queue_remove(t);
    longjmp(thread[t].env, 1);
  }

  int waker = thread[curThread].waker;
  if (waker >= 0 && thread[waker].valid) {
    queue_push(waker);
  }
  return waker;
}

/*   	MyGetThread () returns ID of currently running thread.
 */

int MyGetThread() {
  if (!MyInitThreadsCalled) {
    Printf("MyGetThread: Must call MyInitThreads first\n");
    Exit();
  }
  return curThread;    
}

/* 	MySchedThread () causes the running thread to simply give up the
 *  	CPU and allow another thread to be scheduled.  Selecting which
 *  	thread to run is determined here.  Note that the same thread may
 * 	be chosen (as will be the case if there are no other threads).
 */

void MySchedThread() {
  int i = -1;
  if (!MyInitThreadsCalled) {
    Printf("MySchedThread: Must call MyInitThreads first\n");
    Exit();
  }


  if ( !queue_is_empty()) {
    i = queue_pop();
    while (i >= 0 && !thread[i].valid && !queue_is_empty()) {
        i = queue_pop();
    }
  }

  if (i >= 0) {
       thread[i].is_sched = 1;
//       DPrintf("thread %d sched to %d\n", curThread, i);
       MyYieldThread(i);
  }
}

/* 	MyExitThread () causes the currently running thread to exit.
 */

void MyExitThread() {

  if (!MyInitThreadsCalled) {
    Printf("MyExitThread: Must call MyInitThreads first\n");
    Exit();
  }


	thread[curThread].valid = 0;
    thread[curThread].func = 0;
    thread[curThread].param = -1;
    if (thread[curThread].next >= 0) {
        queue_remove(curThread);
    }
    if (!queue_is_empty()) {
    	MySchedThread();
    } else if (thread[curThread].waker >= 0 && thread[thread[curThread].waker].valid) {
        queue_push(thread[curThread].waker);
    } else {
        Exit();
    }
}
