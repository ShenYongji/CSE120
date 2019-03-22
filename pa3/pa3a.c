/* Programming Assignment 3: Exercise A
 *
 * In this assignment, you will implement semaphores, and use them in a
 * program that simulates cars sharing a single-lane road and that require
 * synchronization.
 *
 * In this first exercise, we begin by studying a simulation program
 * that creates multiple processes, where each process simulates a car
 * entering, proceeding over, and exiting, a single-lane road.  The road
 * is 10 miles long, and consists of 10 positions (each of length 1 mile).
 *
 * Cars entering    ------------------------------------------    Cars entering
 * from the WEST -> | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | <- from the EAST
 *                  ------------------------------------------
 *
 * Cars may enter the road from the West, at position 1, or they may enter
 * from the East, at position 10.  Cars enter and proceed at a certain speed
 * (expressed in MPH, miles per hour) and maintain that speed until they exit.
 *
 * The primary rule of the road is that each position can be occupied by
 * at most 1 car at any time.  If a car tries to proceed to a position
 * that is already occupied, a CRASH occurs.  When a crash occurs, both
 * cars get automatically removed from the road (allowing other cars to
 * proceed).
 *
 * In the program below, two cars drive over the road in the same direction,
 * both entering from the West (pos 1).  Car 1 drives at 40 MPH, and Car 2
 * at 60 MPH.  The procedure driveRoad simulates this activity: it takes
 * as parameters entrance point (EAST or WEST), and speed (in MPH).  To
 * make sure that both cars do not collide by entering the road at the
 * same time, Car 2 waits some time before entering.  This is implemented
 * by the procedure Delay (t) which takes a time (in seconds) as a parameter.
 * In the program, Car 2 delays by 900 seconds.  Since Car 1 drives at 40 MPH,
 * it will travel the 10-mile road in 15 minutes, or 900 seconds.  Hence,
 * Car 2 will enter at just about the time that Car 1 exits.
 *
 * Exercises
 *
 * 1. Run the program and note the output.  A line is printed each time a
 * car makes a move (either to enter, proceed, or exit, the road) or if a
 * collision occurs.
 *
 * 2. Since Car 2 is delayed by 900 seconds, it will have waited until Car 1
 * exits (note that times are NOT exact in the simulation, so Car 1 may or
 * may not exit right at the time Car 2 enters).  Modify the program so that
 * Car 2 delays by only 450 seconds.  What happens, and why?
 *
 * 3. Now modify the program so Car 2 delays by only 300 seconds.  What
 * happens, and why?  Run the program a few times to see if the same events
 * occur in the same way each time.
 *
 * 4. Modify the program so Car 2 delays by 0 seconds, just to see where
 * the crash occurs.
 *
 * 5. Now change the direction of Car 2 so that it enters from the East
 * (thus, starting at position 10).  Leave the delay at 0.  Where does the
 * crash occur, and why?
 *
 * 6. Still having both cars go in opposite directions, modify the delay so
 * that a crash does not occur.  How large does the delay have to be?
 *
 * 7. Study the implementation of driveRoad.  In addition to the Delay
 * procedure, it makes use of two other procedures, EnterRoad, and ProceedRoad.
 * EnterRoad causes a car to enter the road, and takes a parameter indicating
 * the car's point of entrance.  ProceedRoad simply causes the car to
 * move one position (one mile), in the direction it entered.  Notice how
 * movement is implemented by having the cars delay by 3600/mph, the number
 * of seconds in an hour divided by the speed in miles per hour.
 */

#include <stdio.h>
#include "aux.h"
#include "sys.h"
#include "umix.h"

int driveRoad (int from, int mph);

void Main ()
{
	if (Fork () == 0) {
						// car 2
		Delay (960);			// wait 900 simulated secs
		driveRoad (EAST, 60);		// enter from West at 60 mph
		Exit ();
	}

	driveRoad (WEST, 40);			// car 1

	Exit ();
}

#define IPOS(FROM)	(((FROM) == WEST) ? 1 : NUMPOS)

int driveRoad (from, mph)
	int from;			// coming from which direction
	int mph;			// speed of car
{
	int c;				// car ID c = process ID
	int p, np, i;			// positions

	c = Getpid ();			// learn this car's ID

	EnterRoad (from);
	PrintRoad ();
	Printf ("Car %d enters at %d at %d mph\n", c, IPOS(from), mph);

	for (i = 1; i < NUMPOS; i++) {
		if (from == WEST) {
			p = i;
			np = i + 1;
		} else {
			p = NUMPOS + 1 - i;
			np = p - 1;
		}

		Delay (3600/mph);
		ProceedRoad ();
		PrintRoad ();
		Printf ("Car %d moves from %d to %d\n", c, p, np);
	}

	Delay (3600/mph);
	ProceedRoad ();
	PrintRoad ();
	Printf ("Car %d exits road\n", c);
}
