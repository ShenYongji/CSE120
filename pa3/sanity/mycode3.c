/* mycode3.c: your portion of the kernel
 *
 *   	Below are procedures that are called by other parts of the kernel. 
 * 	Your ability to modify the kernel is via these procedures.  You may
 *  	modify the bodies of these procedures any way you desire (however,
 *  	you cannot change the interfaces).  
 */

#include "aux.h"
#include "sys.h"
#include "mycode3.h"

#define FALSE 0
#define TRUE 1

/* 	A sample semaphore table. You may change this any way you wish. 
 */

static struct {
	int valid;	// Is this a valid entry (was sem allocated)?
	int value;	// value of semaphore
    int List[MAXPROCS];  //list of process blocked
    int i; 
    int y;
} semtab[MAXSEMS];


/* 	InitSem () is called when kernel starts up.  Initialize data
 *  	structures (such as semaphore table) and call any initialization
 *   	procedures here. 
 */

void InitSem ()
{
	int s;

	/* modify or add code any way you wish */

	for (s = 0; s < MAXSEMS; s++) {		// mark all sems free
		semtab[s].valid = FALSE;
        semtab[s].i=0;
        semtab[s].y=0;
	    for (int i = 0; i< MAXPROCS;i++){
            semtab[s].List[i]=0;
        }
    }
}

/* 	MySeminit (p, v) is called by the kernel whenever the system
 *  	call Seminit (v) is called.  The kernel passes the initial
 *  	value v, along with the process ID p of the process that called
 * 	Seminit.  MySeminit should allocate a semaphore (find free entry
 * 	in semtab and allocate), initialize that semaphore's value to v,
 *  	and then return the ID (i.e., index of allocated entry). 
 */

int MySeminit (int p, int v)
{
	int s;

	/* modify or add code any way you wish */

	for (s = 0; s < MAXSEMS; s++) {
		if (semtab[s].valid == FALSE) {
			break;
		}
	}
	if (s == MAXSEMS) {
		DPrintf ("No free semaphores.\n");
		return (-1);
	}

	semtab[s].valid = TRUE;
	semtab[s].value = v;

	return (s);
}

/*   	MyWait (p, s) is called by the kernel whenever the system call
 * 	Wait (s) is called. 
 */

void MyWait (p, s)
	int p;				// process
	int s;				// semaphore
{
	/* modify or add code any way you wish */

	semtab[s].value--;

    if(semtab[s].value < 0){
        //add calling process to list and block
        semtab[s].List[semtab[s].i] = p;
        if(semtab[s].i != MAXPROCS-1){
            semtab[s].i++;
        }
        else{
            semtab[s].i=0;
        }
        Block(p);
    }
}

/*  	MySignal (p, s) is called by the kernel whenever the system call
 *  	Signal (s) is called.  
 */

void MySignal (p, s)
	int p;				// process
	int s;				// semaphore
{
	/* modify or add code any way you wish */
    
    int process = 0;
	semtab[s].value++;
    //check the list is empty or not 
    //by checking the first element in the blocked list
    if(semtab[s].List[semtab[s].y]!=0){
        //get the first process on the blocked list
        process = semtab[s].List[semtab[s].y];
        Unblock(process);
        //remove the process which we get above
        semtab[s].List[semtab[s].y]=0;
        if(semtab[s].y!=MAXPROCS-1){
            semtab[s].y++;
        }
        else{
            semtab[s].y=0;
        }
    }
    //Unblock(p);
}

