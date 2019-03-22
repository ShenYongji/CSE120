/* mycode2.c: your portion of the kernel
 *
 *   	Below are procedures that are called by other parts of the kernel. 
 * 	Your ability to modify the kernel is via these procedures.  You may
 *  	modify the bodies of these procedures any way you wish (however,
 *  	you cannot change the interfaces).  
 */

#include "aux.h"
#include "sys.h"
#include "mycode2.h"

#define TIMERINTERVAL 1	// in ticks (tick = 10 msec)

/* 	A sample process table. You may change this any way you wish. 
 */

static struct {
	int valid;		// is this entry valid: 1 = yes, 0 = no
	int pid;
    int requested;
    int stride;
    int pass;
    int value;   // process ID (as provided by kernel)
} proctab[MAXPROCS];

int totalCPU;
/* 	InitSched () is called when the kernel starts up.  First, set the
 *  	scheduling policy (see sys.h). Make sure you follow the rules
 *   	below on where and how to set it.  Next, initialize all your data
 * 	structures (such as the process table).  Finally, set the timer
 *  	to interrupt after a specified number of ticks. 
 */

void InitSched ()
{
	int i;

	/* First, set the scheduling policy. You should only set it
	 * from within this conditional statement.  While you are working
	 * on this assignment, GetSchedPolicy () will return NOSCHEDPOLICY. 
	 * Thus, the condition will be true and you may set the scheduling
	 * policy to whatever you choose (i.e., you may replace ARBITRARY).  
	 * After the assignment is over, during the testing phase, we will
	 * have GetSchedPolicy () return the policy we wish to test (and
	 * the policy WILL NOT CHANGE during the entirety of a test).  Thus
	 * the condition will be false and SetSchedPolicy (p) will not be
	 * called, thus leaving the policy to whatever we chose to test
	 * (and so it is important that you NOT put any critical code in
	 * the body of the conditional statement, as it will not execute when
	 * we test your program). 
	 */
	if (GetSchedPolicy () == NOSCHEDPOLICY) {	// leave as is
		SetSchedPolicy (ROUNDROBIN);		// set policy here
	 //PROPORTIONAL:
	}
		
	/* Initialize all your data structures here */
	for (i = 0; i < MAXPROCS; i++) {
		proctab[i].valid = 0;
		proctab[i].stride = 0;
		proctab[i].value = 0;
		proctab[i].pass = 0;
		proctab[i].requested = 0;
	}

	/* Set the timer last */
	SetTimer (TIMERINTERVAL);
}


/*  	StartingProc (p) is called by the kernel when the process
 * 	identified by PID p is starting. This allows you to record the
 * 	arrival of a new process in the process table, and allocate
 *  	any resources (if necessary).  Returns 1 if successful, 0 otherwise. 
 */
static struct{
    int valid;
    int pid;
}queue[MAXPROCS];
int index;


int usingCPU;
int pre;
int next = -1;
int StartingProc (p)
	int p;				// process that is starting
{
	int i;


    //if (GetSchedPolicy()==FIFO){
    //FIFO && LIFO 
    if(!queue[index].valid){
        queue[index].valid=1;
        queue[index].pid=p;
        if(GetSchedPolicy()==FIFO){
            index+=1;
            //DPrintf("%d\n",index);
        }
        if(GetSchedPolicy()==LIFO){
            index+=1;
            DoSched();
        }
        if(GetSchedPolicy() == ROUNDROBIN){
                index=(index+1)%MAXPROCS;
        }
    }

    //}
    //else{
	for (i = 0; i < MAXPROCS; i++) {
		if (! proctab[i].valid) {
			proctab[i].valid = 1;
			proctab[i].pid = p;
            totalCPU++;
			return (1);
		}
	}

	DPrintf ("Error in StartingProc: no free table entries\n");
	return (0);
}
			

/*   	EndingProc (p) is called by the kernel when the process
 * 	identified by PID p is ending.  This allows you to update the
 *  	process table accordingly, and deallocate any resources (if
 *  	necessary).  Returns 1 if successful, 0 otherwise. 
 */


int EndingProc (p)
	int p;				// process that is ending
{
	int i;


    for (i=0;i<MAXPROCS;i++){
        if(queue[i].valid&&queue[i].pid==p){
            queue[i].valid=0;
        }
    }

	for (i = 0; i < MAXPROCS; i++) {
		if (proctab[i].valid && proctab[i].pid == p) {
			proctab[i].valid = 0;
            totalCPU--;
            usingCPU=usingCPU-proctab[i].value;
			return (1);
		}
	}

	DPrintf ("Error in EndingProc: can't find process %d\n", p);
	return (0);
}


/* 	SchedProc () is called by kernel when it needs a decision for
 * 	which process to run next. It calls the kernel function
 *  	GetSchedPolicy () which will return the current scheduling policy
 *   	which was previously set via SetSchedPolicy (policy).  SchedProc ()
 * 	should return a process PID, or 0 if there are no processes to run. 
 */

int SchedProc ()
{
	int i;

    int count;

    int min = 100001;
    int curindex;

	switch (GetSchedPolicy ()) {

	case ARBITRARY:

		for (i = 0; i < MAXPROCS; i++) {
			if (proctab[i].valid) {
				return (proctab[i].pid);
			}
		}
		break;

	case FIFO:

		/* your code here */
        for(i = 0;i < MAXPROCS;i++){
            if(queue[i].valid){
                return (queue[i].pid);
            }
        }

		break;

	case LIFO:

		/* your code here */
        for (i=MAXPROCS-1;i>=0;i--){
            if(queue[i].valid){
                return(queue[i].pid);
            }
        }
		break;

	case ROUNDROBIN:


        
		/* your code here */
        //for(i = pre, i!=MAXPROCS;i=(i+1)%MAXPROCS){
        for(i = pre,count=0;i!=MAXPROCS,count!=MAXPROCS;i=(i+1)%MAXPROCS,\
                                                                  count++){
            //DPrintf("%d_%d\n",i,(i+1)%MAXPROCS);
            if(queue[i].valid){
                pre=(i+1)%MAXPROCS;
                return (queue[i].pid);
            }  
        }
		break;
	case PROPORTIONAL:

		/* your code here */
        for(i=0;i<MAXPROCS;i++){
            if(proctab[i].valid && proctab[i].pass < 100001){
                min = proctab[i].pass;
            }
        }
        for(i=0;i<MAXPROCS;i++){
            if(proctab[i].valid && proctab[i].pass == min){
                proctab[i].pass= proctab[i].pass+proctab[i].stride;
                return proctab[i].pid;
            }
        }
       /* for (i=0; i<MAXPROCS; i++) {
            if(proctab[i].valid){
               if(min == -1){
                   min = i;
               }
               if(proctab[i].pass<proctab[min].pass){
                   min = i;
               }
            }
        }
         proctab[min].pass=proctab[min].pass+proctab[min].stride;
         if(proctab[min].valid){
            return proctab[min].pid;
         }*/
        break;
	}
	
	return (0);
}


/*  	HandleTimerIntr () is called by the kernel whenever a timer
 *  	interrupt occurs.  Timer interrupts should occur on a fixed
 * 	periodic basis.
 */

void HandleTimerIntr ()
{
	SetTimer (TIMERINTERVAL);

	switch (GetSchedPolicy ()) {	// is policy preemptive?

	case ROUNDROBIN:		// ROUNDROBIN is preemptive
	case PROPORTIONAL:		// PROPORTIONAL is preemptive

		DoSched ();		// make scheduling decision
		break;

	default:			// if non-preemptive, do nothing
		break;
	}
}

/* 	MyRequestCPUrate (p, n) is called by the kernel whenever a process
 *  	identified by PID p calls RequestCPUrate (n).  This is a request for
 *   	n% of CPU time, i.e., requesting a CPU whose speed is effectively
 * 	n% of the actual CPU speed. Roughly n out of every 100 quantums
 *  	should be allocated to the calling process. n must be at least
 *  	0 and must be less than or equal to 100.  MyRequestCPUrate (p, n)
 * 	should return 0 if successful, i.e., if such a request can be
 * 	satisfied, otherwise it should return -1, i.e., error (including if
 *  	n < 0 or n > 100). If MyRequestCPUrate (p, n) fails, it should
 *   	have no effect on scheduling of this or any other process, i.e., AS
 * 	IF IT WERE NEVER CALLED.
 */
int AmountnewCPU;
int MyRequestCPUrate (p, n)
	int p;				// process whose rate to change
	int n;				// percent of CPU time
{
	/* your code here */
    int unusingCPU = 100 - usingCPU;
    
    if( n < 0|| n > 100 )
        return (-1);
	
    for (int i =0;i<MAXPROCS;i++){
        if(proctab[i].valid && proctab[i].pid==p){
            if(proctab[i].requested == 1){
                //0
                if (n == 0){
                    usingCPU = usingCPU - proctab[i].value;
                    proctab[i].requested = 0;
                    proctab[i].value = n;
                    AmountnewCPU--;
                }
                //need less space
                else if(proctab[i].value >= n){
                    usingCPU = usingCPU - proctab[i].value;
                    proctab[i].requested = 1;
                    proctab[i].value = n;
                    usingCPU = usingCPU + n;
                    proctab[i].stride = 100000/n;
                }
                //need more space
                else{
                    //enough space for requesting
                    if(proctab[i].value + unusingCPU >=n){
                        usingCPU = usingCPU - proctab[i].value;
                        proctab[i].requested = 1;
                        proctab[i].value = n;
                        usingCPU = usingCPU + n;
                        proctab[i].stride = 100000/n;
                    }
                    //no enough
                    else{
                        return (-1);
                    }
                }
            }
            else{
                //having space for new proc did not request before
                if( unusingCPU >= n){
                    proctab[i].requested = 1;
                    proctab[i].value = n;
                    usingCPU = usingCPU + n;
                    AmountnewCPU++;
                    proctab[i].stride = 100000/n;
                }
                else{
                    //no sapce for new proc
                    return (-1);
                }
            }
        }
        else
           return (-1);
    }

   
    return (0);
}
