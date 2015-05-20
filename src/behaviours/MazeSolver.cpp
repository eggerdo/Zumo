/**
 * 456789------------------------------------------------------------------------------------------------------------120
 *
 * @brief:
 * @file: MazeSolver.cpp
 *
 * @desc:
 *
 * This file is created at Almende B.V. and Distributed Organisms B.V. It is open-source software and belongs to a
 * larger suite of software that is meant for research on self-organization principles and multi-agent systems where
 * learning algorithms are an important aspect.
 *
 * This software is published under the GNU Lesser General Public license (LGPL).
 *
 * It is not possible to add usage restrictions to an open-source license. Nevertheless, we personally strongly object
 * against this software being used for military purposes, factory farming, animal experimentation, and "Universal
 * Declaration of Human Rights" violations.
 *
 * Copyright (c) 2013 Dominik Egger <dominik@dobots.nl>
 *
 * @author:        Dominik Egger
 * @date:        May 14, 2015
 * @project:    Zumo
 * @company:     Distributed Organisms B.V.
 */

//-------------------------------------------------------------------
// INCLUDES
//-------------------------------------------------------------------

#include "behaviours/MazeSolver.h"

#include "Sensors.h"

#include "drivers/Actuator.h"
#include "drivers/Button.h"

//-------------------------------------------------------------------
// CONFIG
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// FUNCTIONS
//-------------------------------------------------------------------

// PUBLIC


// PRIVATE


/* This example uses the Zumo Reflectance Sensor Array
 * to navigate a black line maze with no loops. This program
 * is based off the 3pi maze solving example which can be
 * found here:
 *
 * http://www.pololu.com/docs/0J21/8.a
 * 
 * The Zumo first calibrates the sensors to account
 * for differences of the black line on white background.
 * Calibration is accomplished in setup().
 *
 * In loop(), the function solveMaze() is called and navigates
 * the Zumo until it finds the finish line which is defined as
 * a large black area that is thick and wide enough to
 * cover all six sensors at the same time.
 * 
 * Once the Zumo reaches the finishing line, it will stop and
 * wait for the user to place the Zumo back at the starting
 * line. The Zumo can then follow the shortest path to the finish
 * line.
 *
 * The macros MAX_MAZESOLVER_SPEED, MAX_MAZESOLVER_TURN_MAX_MAZESOLVER_SPEED, ABOVE_LINE(), and LINE_THICKNESS
 * might need to be adjusted on a case by case basis to give better 
 * line following results.
 */

// When the Zumo reaches the end of a segment it needs
// to find out three things: if it has reached the finish line,
// if there is a straight segment ahead of it, and which
// segment to take. OVERSHOOT tells the Zumo how far it needs
// to overshoot the segment to find out any of these things.
#define OVERSHOOT(line_thickness)(((INCHES_TO_ZUNITS * (line_thickness)) / MAX_MAZESOLVER_SPEED))

MazeSolver mazeSolver;

//const char* mazeSolverStateStr[] = {
//	"solving",
//	"solved",
//	"waiting",
//	"repeating",
//	"finished"
//};
//const char* repeatStepStr[] = {
//	"rs_started",
//	"rs_follow",
//	"rs_turn",
//	"rs_finished"
//};

void MazeSolver::init()
{

	LOGi("MazeSolver init...");

	unsigned int sensors[6];
	unsigned short count = 0;
	unsigned short last_status = 0;
	int turn_direction = 1;

	buzzer.play(">g32>>c32");

//	reflectanceSensors.init();
//	reflectanceSensors.init(A4);

	delay(1000);
//	pinMode(13, OUTPUT);
//	digitalWrite(13, HIGH);        // turn on LED to indicate we are in calibration mode

	//  button.waitForButton();

	// Calibrate the Zumo by sweeping it from left to right
	for(int i = 0; i < 4; i ++)
	{
		// Zumo will turn clockwise if turn_direction = 1.
		// If turn_direction = -1 Zumo will turn counter-clockwise.
		turn_direction *= -1;

		// Turn direction.
		drive(turn_direction * MAX_MAZESOLVER_TURN_SPEED, -1*turn_direction * MAX_MAZESOLVER_TURN_SPEED);

		// This while loop monitors line position
		// until the turn is complete.
		while(count < 2)
		{
			reflectanceSensors.calibrate();
			reflectanceSensors.readLine(sensors, QTR_EMITTERS_ON, true);
//			reflectanceSensors.readLine(sensors);

//			LOGi("sensor[0]: %d, sensor[5]: %d", sensors[0], sensors[5]);

			if(turn_direction < 0)
			{
				// If the right  most sensor changes from (over white space -> over
				// line or over line -> over white space) add 1 to count.
				count += ABOVE_LINE(sensors[5]) ^ last_status;
				last_status = ABOVE_LINE(sensors[5]);
			}
			else
			{
				// If the left most sensor changes from (over white space -> over
				// line or over line -> over white space) add 1 to count.
				count += ABOVE_LINE(sensors[0]) ^ last_status;
				last_status = ABOVE_LINE(sensors[0]);
			}
//			LOGi("count: %d, last_status: %d", count, last_status);
		}

		count = 0;
		last_status = 0;
	}

	// Turn left.
	turn('L');

	drive_stop();

	// Sound off buzzer to denote Zumo is finished calibrating
//	buzzer.play("L16 cdegreg4");

	// Turn off LED to indicate we are through with calibration
//	digitalWrite(13, LOW);

	_calibrated = true;
	LOGi("... done");
}

extern "C" int run_mazesolver() {
	return mazeSolver.execute();
}

void MazeSolver::start() {
	if (!_calibrated) return;

	LOGi("MazeSolver start...");
	reset();
	switchState(solving);

	// Play music and wait for it to finish before we start driving.
	buzzer.play("L16 cdegreg4");
	while(buzzer.isPlaying());

	if (startLooper) {
		Looper::getInstance()->registerLoopFunc(run_mazesolver);
		startLooper = false;
	}
}

void MazeSolver::stop() {
	if (!_calibrated) return;

	LOGi("MazeSolver stop...");

	Looper::getInstance()->unregisterLoopFunc(run_mazesolver);
	drive_stop();
	startLooper = true;
}

void MazeSolver::repeat() {

	LOGi("MazeSolver repeat...");

	// Play music and wait for it to finish before we start driving.
	buzzer.play("L16 cdegreg4");
	while(buzzer.isPlaying());

	switchState(repeating);
	switchStep(rs_started);
}

int MazeSolver::execute()
{

//	LOGi("execute");

	switch(_state) {
	case solving: {
		// solveMaze() explores every segment
		// of the maze until it finds the finish
		// line.
		if (solveMaze()) {
			switchState(solved);

			// Sound off buzzer to denote Zumo has solved the maze
			buzzer.play(">>a32");

			// The maze has been solved. When the user
			// places the Zumo at the starting line
			// and pushes the Zumo button, the Zumo
			// knows where the finish line is and
			// will automatically navigate.

			LOGi("solved");
		}
		break;
	}
	case solved: {

//		if (checkForButton()) {
//			switchState(repeating);
//		}
		break;
	}
	case repeating: {
		if (goToFinishLine()) {
			switchState(finished);

			// Sound off buzzer to denote Zumo is at the finish line.
			buzzer.play(">>a32");
		}
		break;
	}
	case finished: {
		LOGi("over");
		drive_stop();
//		stop();

		switchState(idle);
		break;
	}
	}

	return 0;
}

// Turns according to the parameter dir, which should be
// 'L' (left), 'R' (right), 'S' (straight), or 'B' (back).
void MazeSolver::turn(char dir)
{

	LOGi("turn: %c", dir);

	// count and last_status help
	// keep track of how much further
	// the Zumo needs to turn.
	unsigned short count = 0;
	unsigned short last_status = 0;
	unsigned int sensors[6];

	// dir tests for which direction to turn
	switch(dir)
	{

	// Since we're using the sensors to coordinate turns instead of timing them,
	// we can treat a left turn the same as a direction reversal: they differ only
	// in whether the zumo will turn 90 degrees or 180 degrees before seeing the
	// line under the sensor. If 'B' is passed to the turn function when there is a
	// left turn available, then the Zumo will turn onto the left segment.
	case 'L':
	case 'B':
		// Turn left.
		drive(-MAX_MAZESOLVER_TURN_SPEED, MAX_MAZESOLVER_TURN_SPEED);

		// This while loop monitors line position
		// until the turn is complete.
		while(count < 2)
		{
			reflectanceSensors.readLine(sensors, QTR_EMITTERS_ON, _whiteLines);

			// Increment count whenever the state of the sensor changes
			// (white->black and black->white) since the sensor should
			// pass over 1 line while the robot is turning, the final
			// count should be 2
			count += ABOVE_LINE(sensors[0]) ^ last_status;
			last_status = ABOVE_LINE(sensors[0]);

		}

		break;

	case 'R':
		// Turn right.
		drive(MAX_MAZESOLVER_TURN_SPEED, -MAX_MAZESOLVER_TURN_SPEED);

		// This while loop monitors line position
		// until the turn is complete.
		while(count < 2)
		{
			reflectanceSensors.readLine(sensors, QTR_EMITTERS_ON, _whiteLines);;
			count += ABOVE_LINE(sensors[5]) ^ last_status;
			last_status = ABOVE_LINE(sensors[5]);
		}

		break;

	case 'S':
		// Don't do anything!
		break;
	}
}

// This function decides which way to turn during the learning phase of
// maze solving.  It uses the variables found_left, found_straight, and
// found_right, which indicate whether there is an exit in each of the
// three directions, applying the "left hand on the wall" strategy.
char MazeSolver::selectTurn(unsigned char found_left, unsigned char found_straight,
		unsigned char found_right)
{
	LOGi("selectTurn");

	// Make a decision about how to turn.  The following code
	// implements a left-hand-on-the-wall strategy, where we always
	// turn as far to the left as possible.
	if(found_left)
		return 'L';
	else if(found_straight)
		return 'S';
	else if(found_right)
		return 'R';
	else
		return 'B';
}

// The maze is broken down into segments. Once the Zumo decides
// which segment to turn on, it will navigate until it finds another
// intersection. followSegment() will then return after the
// intersection is found.
bool MazeSolver::followSegment()
{
//	LOGi("followSegment");

	unsigned int position;
	unsigned int sensors[6];
	int offset_from_center;
	int power_difference;

	// Get the position of the line.
		position = reflectanceSensors.readLine(sensors, QTR_EMITTERS_ON, _whiteLines);;

		// The offset_from_center should be 0 when we are on the line.
		offset_from_center = ((int)position) - 2500;

		// Compute the difference between the two motor power settings,
		// m1 - m2.  If this is a positive number the robot will turn
		// to the left.  If it is a negative number, the robot will
		// turn to the right, and the magnitude of the number determines
		// the sharpness of the turn.
		power_difference = offset_from_center / 3;

		// Compute the actual motor settings.  We never set either motor
		// to a negative value.
		if(power_difference > MAX_MAZESOLVER_SPEED)
			power_difference = MAX_MAZESOLVER_SPEED;
		if(power_difference < -MAX_MAZESOLVER_SPEED)
			power_difference = -MAX_MAZESOLVER_SPEED;

		if(power_difference < 0)
			drive(MAX_MAZESOLVER_SPEED + power_difference, MAX_MAZESOLVER_SPEED);
		else
			drive(MAX_MAZESOLVER_SPEED, MAX_MAZESOLVER_SPEED - power_difference);

		// We use the inner four sensors (1, 2, 3, and 4) for
		// determining whether there is a line straight ahead, and the
		// sensors 0 and 5 for detecting lines going to the left and
		// right.

		if(!ABOVE_LINE(sensors[0]) && !ABOVE_LINE(sensors[1]) && !ABOVE_LINE(sensors[2]) && !ABOVE_LINE(sensors[3]) && !ABOVE_LINE(sensors[4]) && !ABOVE_LINE(sensors[5]))
		{
			// There is no line visible ahead, and we didn't see any
			// intersection.  Must be a dead end.

			return true;
		}
		else if(ABOVE_LINE(sensors[0]) || ABOVE_LINE(sensors[5]))
		{
			// Found an intersection.
			return true;
		}

		return false;
}

// The solveMaze() function works by applying a "left hand on the wall" strategy:
// the robot follows a segment until it reaches an intersection, where it takes the
// leftmost fork available to it. It records each turn it makes, and as long as the
// maze has no loops, this strategy will eventually lead it to the finish. Afterwards,
// the recorded path is simplified by removing dead ends. More information can be
// found in the 3pi maze solving example.
bool MazeSolver::solveMaze()
{
//	LOGi("solveMaze");

	// Navigate current line segment
	if (followSegment()) {

		// These variables record whether the robot has seen a line to the
		// left, straight ahead, and right, while examining the current
		// intersection.
		unsigned char found_left = 0;
		unsigned char found_straight = 0;
		unsigned char found_right = 0;

		// Now read the sensors and check the intersection type.
		unsigned int sensors[6];
		reflectanceSensors.readLine(sensors, QTR_EMITTERS_ON, _whiteLines);;

		// Check for left and right exits.
		if(ABOVE_LINE(sensors[0]))
			found_left = 1;
		if(ABOVE_LINE(sensors[5]))
			found_right = 1;

		// Drive straight a bit more, until we are
		// approximately in the middle of intersection.
		// This should help us better detect if we
		// have left or right segments.
		drive(MAX_MAZESOLVER_SPEED, MAX_MAZESOLVER_SPEED);
		delay(OVERSHOOT(LINE_THICKNESS)/2);

		reflectanceSensors.readLine(sensors, QTR_EMITTERS_ON, _whiteLines);;

		// Check for left and right exits.
		if(ABOVE_LINE(sensors[0]))
			found_left = 1;
		if(ABOVE_LINE(sensors[5]))
			found_right = 1;

		// After driving a little further, we
		// should have passed the intersection
		// and can check to see if we've hit the
		// finish line or if there is a straight segment
		// ahead.
		delay(OVERSHOOT(LINE_THICKNESS)/2);

		// Check for a straight exit.
		reflectanceSensors.readLine(sensors, QTR_EMITTERS_ON, _whiteLines);;

		// Check again to see if left or right segment has been found
		if(ABOVE_LINE(sensors[0]))
			found_left = 1;
		if(ABOVE_LINE(sensors[5]))
			found_right = 1;

		if(ABOVE_LINE(sensors[1]) || ABOVE_LINE(sensors[2]) || ABOVE_LINE(sensors[3]) || ABOVE_LINE(sensors[4]))
			found_straight = 1;

		// Check for the ending spot.
		// If all four middle sensors are on dark black, we have
		// solved the maze.
		if(ABOVE_LINE(sensors[0]) && ABOVE_LINE(sensors[1]) && ABOVE_LINE(sensors[2]) && ABOVE_LINE(sensors[3]) && ABOVE_LINE(sensors[4]) && ABOVE_LINE(sensors[5]))
		{
			drive(0,0);
			return true;
		}

		// Intersection identification is complete.
		unsigned char dir = selectTurn(found_left, found_straight, found_right);

		// Make the turn indicated by the path.
		turn(dir);

		// Store the intersection in the path variable.
		path[path_length] = dir;
		path_length++;

		// You should check to make sure that the path_length does not
		// exceed the bounds of the array.  We'll ignore that in this
		// example.

		// Simplify the learned path.
		simplifyPath();

	}

	return false;
}

// Now enter an infinite loop - we can re-run the maze as many
// times as we want to.
bool MazeSolver::goToFinishLine()
{

//	LOGi("goToFinishLine");

	unsigned int sensors[6];
//	int _currentSegment = 0;

	switch(_step) {
	case rs_started: {
		// Turn around if the Zumo is facing the wrong direction.
		if(path[0] == 'B')
		{
			switchStep(rs_turn);
		} else {
			switchStep(rs_follow);
		}
		break;
	}
	case rs_follow: {

		if (followSegment()) {

			// Drive through the intersection.
			drive(MAX_MAZESOLVER_SPEED, MAX_MAZESOLVER_SPEED);
			delay(OVERSHOOT(LINE_THICKNESS));

			// Make a turn according to the instruction stored in
			// path[i].
			switchStep(rs_turn);
		}

		break;
	}
	case rs_turn: {

		// Make a turn according to the instruction stored in
		// path[i].
		turn(path[_currentSegment]);

		_currentSegment++;
		if (_currentSegment == path_length) {
			_currentSegment = 0;
			switchStep(rs_finished);
		} else {
			switchStep(rs_follow);
		}

		break;
	}
	case rs_finished: {
//		LOGi("sensors[0], sensors[1], sensors[2], sensors[3], sensors[4], sensors[5]", sensors[0], sensors[1], sensors[2], sensors[3], sensors[4], sensors[5]);

		// Follow the last segment up to the finish.
		if (followSegment()) {

			// After driving a little further, we
			// should have passed the intersection
			// and can check to see if we've hit the
			// finish line or if there is a straight segment
			// ahead.
			delay(OVERSHOOT(LINE_THICKNESS)/2);

			// Check for a straight exit.
			reflectanceSensors.readLine(sensors, QTR_EMITTERS_ON, _whiteLines);;

			// Check for the ending spot.
			// If all four middle sensors are on dark black, we have
			// solved the maze.
			if(ABOVE_LINE(sensors[0]) && ABOVE_LINE(sensors[1]) && ABOVE_LINE(sensors[2]) && ABOVE_LINE(sensors[3]) && ABOVE_LINE(sensors[4]) && ABOVE_LINE(sensors[5]))
			{
				drive_stop();

				LOGi("Finished");
				return true;
			}

		}
		break;
	}
	}

	return false;

}


// simplifyPath analyzes the path[] array and reduces all the
// turns. For example: Right turn + Right turn = (1) Back turn.
void MazeSolver::simplifyPath()
{

//	LOGi("simplifyPath");

	// only simplify the path if the second-to-last turn was a 'B'
	if(path_length < 3 || path[path_length - 2] != 'B') {
		print();
		return;
	}

	int total_angle = 0;
	int i;

	for(i = 1; i <= 3; i++)
	{
		switch(path[path_length - i])
		{
		case 'R':
			total_angle += 90;
			break;
		case 'L':
			total_angle += 270;
			break;
		case 'B':
			total_angle += 180;
			break;
		}
	}

	// Get the angle as a number between 0 and 360 degrees.
	total_angle = total_angle % 360;

	// Replace all of those turns with a single one.
	switch(total_angle)
	{
	case 0:
		path[path_length - 3] = 'S';
		break;
	case 90:
		path[path_length - 3] = 'R';
		break;
	case 180:
		path[path_length - 3] = 'B';
		break;
	case 270:
		path[path_length - 3] = 'L';
		break;
	}

	// The path is now two steps shorter.
	path_length -= 2;

	print();
}
