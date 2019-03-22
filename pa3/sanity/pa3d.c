/* Programming Assignment 3: Exercise D
 *
 * Now that you have a working implementation of semaphores, you can
 * implement a more sophisticated synchronization scheme for the car
 * simulation. 
 *
 * Study the program below.  Car 1 begins driving over the road,  entering
 * from the East at 40 mph.  After 900 seconds,  both Car 3 and Car 4 try to
 * enter the road. Car 1 may or may not have exited by this time (it should
 * exit after 900 seconds, but recall that the times in the simulation are
 * approximate). If Car 1 has not exited and Car 4 enters, they will crash
 * (why?).  If Car 1 has exited,  Car 3 and Car 4 will be able to enter the
 * road, but since they enter from opposite directions,  they will eventually
 * crash. Finally,  after 1200 seconds, Car 2 enters the road from the West
 * and traveling twice as fast as Car 4.  If Car 3 was not coming from the
 * opposite direction, Car 2 would eventually reach Car 4 from the back and
 * crash.  You may wish to experiment with reducing the initial delay of
 * Car 2,  or increase the initial delay of Car 3, to cause Car 2 to crash
 * with Car 4 before Car 3 crashes with Car 4. 
 *
 *
 * Exercises
 *
 * 1) Modify the procedure driveRoad such that the following rules are obeyed:
 *
 *   	A) Avoid all collisions. 
 *
 * 	B) Multiple cars should be allowed on the road,  as long as they are
 *  	traveling in the same direction.  
 *
 *  	C) If a car arrives and there are already other cars traveling in the
 * 	SAME DIRECTION,  the arriving car should be allowed enter as soon as it
 * 	can. Two situations might prevent this car from entering immediately:
 *  	(1) there is a car immediately in front of it (going in the same
 *   	direction), and if it enters it will crash (which would break rule A);
 * 	(2) one or more cars have arrived at the other end to travel in the
 *  	opposite direction and are waiting for the current cars on the road
 *  	to exit, which is covered by the next rule.  
 *
 * 	D) If a car arrives and there are already other cars traveling in the
 * 	OPPOSITE DIRECTION,  the arriving car must wait until all these other
 *  	cars complete their course over the road and exit.  It should only wait
 *   	for the cars already on the road to exit; no new cars traveling in the
 * 	same direction as the existing ones should be allowed to enter. 
 *
 *  	E) The previous rule implies that if there are multiple cars at each
 *  	end waiting to enter the road, each side will take turns in allowing
 * 	one car to enter and exit. However,  if there are no cars waiting at
 * 	one end,  then as cars arrive at the other end, they should be allowed
 *  	to enter the road immediately.  
 *	
 *   	F) If the road is free (no cars), then any car attempting to enter
 * 	should not be prevented from doing so. 
 *
 *  	G) All starvation must be avoided.  For example,  any car that is
 *  	waiting must eventually be allowed to proceed.  
 *
 * This must be achieved ONLY by adding synchronization and making use of
 * shared memory (as described in Exercise C). You should NOT modify the
 * delays or speeds to solve the problem. In other words, the delays and
 * speeds are givens,  and your goal is to enforce the above rules by making
 * use of only semaphores and shared memory.  
 *
 * 2) Devise different tests (using different numbers of cars,  speeds
 * directions) to see whether your improved implementation of driveRoad
 * obeys the rules above. 
 *
 * IMPLEMENTATION GUIDELINES
 * 
 * 1) Avoid busy waiting.  In class one of the reasons given for using
 * semaphores was to avoid busy waiting in user code and limit it to
 * minimal use in certain parts of the kernel.  This is because busy
 * waiting uses up CPU time, but a blocked process does not. You have
 * semaphores available to implement the driveRoad function, so you
 * should not use busy waiting anywhere. 
 *
 * 2) Prevent race conditions.  One reason for using semaphores is to
 * enforce mutual exclusion on critical sections to avoid race conditions. 
 * You will be using shared memory in your driveRoad implementation.  
 * Identify the places in your code where there may be race conditions
 * (the result of a computation on shared memory depends on the order
 * that processes execute).  Prevent these race conditions from occurring
 * by using semaphores. 
 *
 * 3) Implement semaphores fully and robustly. It is possible for your
 * driveRoad function to work with an incorrect implementation of
 * semaphores,  because controlling cars does not exercise every use of
 * semaphores.  You will be penalized if your semaphores are not correctly
 * implemented, even if your driveRoad works. 
 *
 * 4) Control cars with semaphores: Semaphores should be the basic
 * mechanism for enforcing the rules on driving cars.  You should not
 * force cars to delay in other ways inside driveRoad such as by calling
 * the Delay function or changing the speed of a car.  (You can leave in
 * the delay that is already there that represents the car's speed,  just
 * don't add any additional delaying). Also,  you should not be making
 * decisions on what cars do using calculations based on car speed (since
 * there are a number of unpredictable factors that can affect the
 * actual cars' progress). 
 *
 * GRADING INFORMATION
 *
 * 1) Semaphores: We will run a number of programs that test your
 * semaphores directly (without using cars at all).  For example:
 * enforcing mututal exclusion, testing robustness of your list of
 * waiting processes, calling signal and wait many times to make sure
 * the semaphore state is consistent,  etc. 
 *
 * 2) Cars: We will run some tests of cars arriving in different ways
 * to make sure that you correctly enforce all the rules for cars given
 * in the assignment.  We will use a correct semaphore implementation for
 * these tests so that even if your semaphores are not correct you could
 * still get full credit on the driving part of the grade (you may assume
 * that in our implementation, semaphore unblocking is FIFO,  i.e.,  for
 * each semaphore, the order in which processes are unblocked is the
 * same order in which those same processes blocked).  Think about
 * how your driveRoad might handle different situations and write your
 * own tests with cars arriving in different ways to make sure you handle
 * all cases correctly. 
 *
 *
 * WHAT TO TURN IN
 *
 * You must turn in two files: mycode3.c and pa3d.c. mycode3.c should
 * contain you implementation of semaphores, and pa3d.c should contain
 * your modified version of InitRoad and driveRoad (Main will be ignored).  
 * Note that you may set up your static shared memory struct and other
 * functions as you wish. They should be accessed via InitRoad and driveRoad
 * as those are the functions that we will call to test your code.  
 *
 * Your programs will be tested with various Main programs that will exercise
 * your semaphore implementation,  AND different numbers of cars, directions
 * and speeds,  to exercise your driveRoad function.  Our Main programs will
 * first call InitRoad before calling driveRoad. Make sure you do as much
 * rigorous testing yourself to be sure your implementations are robust. 
 * Good luck.  
 */

#include <stdio.h>
#include "aux.h"
#include "sys.h"
#include "umix.h"

void InitRoad (void);
void driveRoad (int from, int mph);
void cross(void);
void same(void);
void one(void);

void Main ()
{
    DPrintf("WEST:%d, EAST:%d\n",WEST,EAST);
	InitRoad ();
   
	/* The following code is specific to this simulation,  e.g., number
	 * of cars, directions and speeds. You should experiment with
	 * different numbers of cars,  directions and speeds to test your
	 * modification of driveRoad.  When your solution is tested, we
	 * will use different Main procedures,  which will first call
	 * InitRoad before any calls to driveRoad.  So,  you should do
	 * any initializations in InitRoad. 
	 */

	if (Fork () == 0) {
		Delay (1200);			// car 2
		driveRoad (WEST, 60);
		Exit ();
	}

	if (Fork () == 0) {
		Delay (900);			// car 3
		driveRoad (EAST, 50);
		Exit ();
	}

	if (Fork () == 0) {
		Delay (900);			// car 4
		driveRoad (WEST, 30);
		Exit ();
	}

	driveRoad (EAST, 40);			// car 1

	Exit ();
    //cross();
    //same();
    //one();
}
void one(){
    /*
	if (Fork () == 0) {
		Delay (1200);			// car 2
		driveRoad (WEST, 60);
		Exit ();
	}*/
	driveRoad (EAST, 40);			// car 1

	Exit ();
}
void same(){
	if (Fork () == 0) {
		Delay (1200);			// car 2
		driveRoad (EAST, 60);
		Exit ();
	}

	if (Fork () == 0) {
		Delay (900);			// car 3
		driveRoad (EAST, 50);
		Exit ();
	}

	if (Fork () == 0) {
		Delay (900);			// car 4
		driveRoad (EAST, 30);
		Exit ();
	}

	driveRoad (EAST, 40);			// car 1

	Exit ();
}

void cross(){

	if (Fork () == 0) {
		Delay (1200);			// car 2
		driveRoad (WEST, 60);
		Exit ();
	}

	if (Fork () == 0) {
		Delay (900);			// car 3
		driveRoad (EAST, 50);
		Exit ();
	}
    
	if (Fork () == 0) {
		Delay (900);			// car 4
		driveRoad (WEST, 30);
		Exit ();
	}

	if (Fork () == 0) {
		Delay (900);			// car 4
		driveRoad (EAST, 30);
		Exit ();
	}
	driveRoad (EAST, 40);			// car 1

	Exit ();
}

/* Our tests will call your versions of InitRoad and driveRoad, so your
 * solution to the car simulation should be limited to modifying the code
 * below. This is in addition to your semaphore implementation contained
 * in mycode3.c.  
 */
struct{

    int position[NUMPOS];
    int currentdir;
    int westgate;//sem
    int eastgate;//sem
    int cartowest;
    int cartoeast;
    int waitonwest;
    int waitoneast;
    int onwest;
    int oneast;
    int westcheck;
    int eastcheck;
    int count;
    int mutex;//sem
    int lock;
}traffic;

void InitRoad ()
{
	/* do any initializations here */
    Regshm((char*)&traffic, sizeof(traffic));
    traffic.currentdir = -1;
    traffic.westgate = Seminit(1);//the gate of west or east
    traffic.eastgate = Seminit(1);
    traffic.westcheck = Seminit(1);
    traffic.eastcheck = Seminit(1);
    traffic.onwest = 0; //1 if a car is running to west
    traffic.oneast = 0; //1 if a car is running to east
    for(int i =0; i < NUMPOS; i++){
        traffic.position[i] = Seminit(1);
    }
    traffic.count = 0;
    traffic.mutex = Seminit(1);
    traffic.lock = 1;
    //goingwest = false;// true if a car runing to west
    //goingeast = false;// true if a car runing to east
}

#define IPOS(FROM)	(((FROM) == WEST) ? 1 : NUMPOS)


//1. before Enterroad()
//  when can't a car call EnterRoad()?
//      a. dir of the car(s) currently on the road != the dir of the car
//      b. dir of the car(s) currently on the road == the dir of the car &&
//      oppositie side has some car(s) waiting
//          wait!!!!!!!
//
//2. on the road
//  for loop......
//      p, np
//      wait(np)
//      proceedRoad()
//      printRoad()
//      sign(p)
//  signal(p)
//
//3. on exit
//  only the last car on the road exiting will do the signals
//  signal opposite cars
//  ??signal same dir cars???
//
//


#define IPOS(FROM)	(((FROM) == WEST) ? 1 : NUMPOS)

void driveRoad (from, mph)
	int from;			// which direction coming from
	int mph;			// speed of car
{
	int c;				// car ID c = process ID
	int p, np, i;			// positions

	c = Getpid ();			// get this car's ID

    //checking the current direction of car on the road
    //Wait(traffic.mutex);
    //no car
    //DPrintf("WEST:%d, EAST:%d\n",WEST,EAST);
    if(traffic.currentdir == -1){
        traffic.currentdir = from;
    }
          
//1. before Enterroad()
//  when can't a car call EnterRoad()?
//      a. dir of the car(s) currently on the road != the dir of the car
//      b. dir of the car(s) currently on the road == the dir of the car &&
//      oppositie side has some car(s) waiting
//          wait!!!!!!!
    //Wait(traffic.mutex);
    if(traffic.count > 0 && traffic.currentdir != from){
        //DPrintf(" >0 && != ,id:%d, dir:%d\n",c,from);
        if(from == WEST){
            //DPrintf(">0 && !=, ==west,id:%d\n",c);
            traffic.waitonwest++;
            //DPrintf("front,id:%d\n",c);
            //Signal(traffic.mutex);
            Wait(traffic.westgate);
            //DPrintf("back,id:%d\n",c);
            if(traffic.count>0&&traffic.currentdir != from ){
                //DPrintf(" >0 && !=,==west,!=id:%d\n",c);
                Wait(traffic.westgate);
            }   
        }
        if(from == EAST){
            //DPrintf(">0 && !=,==east,id:%d\n",c);
            //Signal(traffic.mutex);
            traffic.waitoneast++;
            Wait(traffic.eastgate);
            //Signal(traffic.mutex);
            //Wait(traffic.mutex);
            if(traffic.currentdir != from && traffic.count> 0){
                 //Signal(traffic.mutex);
                 Wait(traffic.eastgate);
                // Wait(traffic.mutex);
            }
        }      
    }else{ 
        if( traffic.count>0&&traffic.currentdir == from){
            //DPrintf(" == ,id:%d, dir:%d\n",c,from);
            if(from == WEST){
               // DPrintf("west east >0, cd:%d\n",c);
                if(traffic.waitoneast>0){
                    traffic.waitonwest++;
                    //Signal(traffic.mutex);
                    //Signal(traffic.mutex);
                    Wait(traffic.westgate);
                    //Wait(traffic.mutex);
                if(traffic.count > 0&&traffic.currentdir == from ){
                    //DPrintf("---west east >0, cd:%d\n",c);
                    //Signal(traffic.mutex);
                    Wait(traffic.westgate);
                    //Wait(traffic.mutex);
                }
            }
            }
            if(from == EAST){
               // DPrintf("east west >0, cd:%d\n",c);
                    //DPrintf(">0 && ==, ==east,>0,id:%d\n",c);
                if(traffic.waitonwest>0){
                    traffic.waitoneast++;
                //Signal(traffic.mutex);
                //Signal(traffic.mutex);
                    Wait(traffic.eastgate);
                //Wait(traffic.mutex);
                    if(traffic.count>0&&traffic.currentdir == from){
                    //Signal(traffic.mutex);
                        Wait(traffic.eastgate);
                    //Wait(traffic.mutex);
                }
            }
            }
       }
    }
       //else
       //{
       // Signal(traffic.mutex);
       //}
    
    //Signal(traffic.mutex);
    //Wait(traffic.mutex);
    if(from == WEST){
        Wait(traffic.position[0]);
    }
    if(from == EAST){
        Wait(traffic.position[NUMPOS-1]);
    }
    
	EnterRoad (from);
    traffic.count++;
    traffic.currentdir = from;
    //Signal(traffic.mutex);
    PrintRoad ();
	Printf ("Car %d enters at %d at %d mph\n", c, IPOS(from), mph);
    Wait(traffic.mutex);


	for (i = 1; i < NUMPOS; i++) {
		if (from == WEST) {
			p = i;
			np = i + 1;
		} else {
			p = NUMPOS + 1 - i;
			np = p - 1;
		}

		Delay (3600/mph);
        Wait(traffic.position[np-1]);
		ProceedRoad ();
		PrintRoad ();
		Printf ("Car %d moves from %d to %d\n", c, p, np);
        Signal(traffic.position[p-1]);

        Signal(traffic.mutex);
        //DPrintf("after for loop id: %d\n", c);
        Wait(traffic.mutex);
        if( i == 1 ){
            if(from==WEST){
                //Signal(traffic.westcheck);
                if(traffic.waitonwest > 0 && traffic.waitoneast == 0){
                    traffic.waitonwest--;
                    Signal(traffic.mutex);
                    Wait(traffic.mutex);
                    Signal(traffic.westgate);
                    Signal(traffic.mutex);
                    Wait(traffic.mutex);
                   // DPrintf("W_STRART,id:%d\n",c);
                }
            }
            if(from==EAST){
                //Signal(traffic.eastcheck);
                if(traffic.waitonwest == 0 && traffic.waitoneast > 0){
                    traffic.waitoneast--;
                    Signal(traffic.mutex);
                    Wait(traffic.mutex);
                    Signal(traffic.eastgate);
                    Signal(traffic.mutex);
                    Wait(traffic.mutex);
                   // DPrintf("E_START\n");
                }
            }
        }
        Signal(traffic.mutex);

		//PrintRoad ();
		//Printf ("Car %d moves from %d to %d\n", c, p, np);
	}

	Delay (3600/mph);
	ProceedRoad ();
	PrintRoad ();
	//Printf ("Car %d exits road\n", c);
    //Signal(traffic.position[np-1]);
    
    if(from == WEST){
        Signal(traffic.position[NUMPOS-1]);
    }
    if(from == EAST){
        Signal(traffic.position[0]);
    }
	//Delay (3600/mph);
	//ProceedRoad ();
	//PrintRoad ();
	Printf ("Car %d exits road\n", c);
    
    
    Wait(traffic.mutex); 
    //exists gate 
    traffic.count--;

    if(from == WEST){
       // DPrintf("count:%d,waitoneast:%d\n",traffic.count, traffic.waitoneast);
        if(traffic.count == 0 && traffic.waitoneast>0){
            traffic.waitoneast--;
            traffic.currentdir == -1;
            Signal(traffic.mutex);
             Wait(traffic.mutex); 
            Signal(traffic.eastgate);
            Signal(traffic.mutex);
         //   DPrintf("exists_east,id:%d\n",c);
        }
        else{
            Signal(traffic.mutex);
           // DPrintf("!exists_east,id:%d\n",c);
            //Signal(traffic.eastgate);
        }
    }
    
    if (from == EAST){ 
      //  DPrintf("count:%d,waitonwest:%d\n",traffic.count, traffic.waitonwest);
        if(traffic.count == 0 && traffic.waitonwest>0){
            traffic.waitonwest--;
            traffic.currentdir == -1;
            Signal(traffic.mutex);
            Wait(traffic.mutex); 
            Signal(traffic.westgate);
            Signal(traffic.mutex);
        //    DPrintf("exists_west,id:%d\n",c);
        }
        else{
            Signal(traffic.mutex);
          //  DPrintf("!exists_west,id:%d\n",c);
            //Signal(traffic.westgate);
        }
    }

}
