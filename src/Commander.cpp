/**
 * 456789------------------------------------------------------------------------------------------------------------120
 *
 * @brief:
 * @file: Commander.cpp
 *
 * @desc:
 *
 *
 *
 * @author:        Dominik Egger
 * @date:        Mar 21, 2015
 * @project:    Zumo
 *
 */

//-------------------------------------------------------------------
// INCLUDES
//-------------------------------------------------------------------

#include <cfg/Config.h>
#include <programs/CompassTest.h>
#include <programs/LineFollower.h>
#include <programs/MazeSolver.h>
#include "Commander.h"

#include "drivers/Serial.h"
#include "drivers/Compass.h"
#include "drivers/Actuator.h"

//-------------------------------------------------------------------
// CONFIG
//-------------------------------------------------------------------

int speed = DEFAULT_SPEED;

//-------------------------------------------------------------------
// LOCAL FUNCTIONS DEFINITION
//-------------------------------------------------------------------

void incSpeed();
void decSpeed();

//-------------------------------------------------------------------
// FUNCTIONS
//-------------------------------------------------------------------

void handleInput(int incoming) {

	LOGi("incoming: %c (%d)", incoming, incoming);
	switch(incoming) {

	// driving
	case 'w':
		drive(speed, speed);
		break;
	case 's':
		drive(-speed, -speed);
		break;
	case 'a':
		drive(-speed, speed);
		break;
	case 'd':
		drive(speed, -speed);
		break;
	case '+':
		incSpeed();
		break;
	case '-':
		decSpeed();
		break;

#ifdef USE_COMPASS
	// HEADING
	case '1':
		setTargetHeading(-135);
		break;
	case '2':
		setTargetHeading(180);
		break;
	case '3':
		setTargetHeading(135);
		break;
	case '4':
		setTargetHeading(-90);
		break;
	case '5':
		if (compass.isCalibrated()) {
			calibrateHeading();
		}
		break;
	case '6':
		setTargetHeading(90);
		break;
	case '7':
		setTargetHeading(-45);
		break;
	case '8':
		setTargetHeading(0);
		break;
	case '9':
		setTargetHeading(45);
		break;

	// compass
	case 'h':
		compass.calibrate();
		break;

	// compass
	case 'i':
		turnDegrees(180);
		break;

	// compass
	case 'j':
		turnDegrees(-180);
		break;

		// compass
		case 'l':
			turnDegrees(-90);
			break;

			// compass
			case ';':
				turnDegrees(90);
				break;

	case 'k':
		stopTurn();
		break;

	case 'u':
		compass.reportHeading();
		break;

#endif

	// compass test
//	case 'j':
//		driveSquare();
//		break;
//	case 'k':
//		stopDriving();
//		break;

#ifdef MAZESOLVER
	case 'p':
		mazeSolver.print();
		break;

	case 'r':
		mazeSolver.repeat();
		break;
	case 'n':
		mazeSolver.start();
		break;
	case 'm':
		mazeSolver.stop();
		break;
#endif

	// line follower
#ifdef LINEFOLLOWER
//	case 'b':
//		lineFollower.init();
//		break;
//	case 'n':
//		lineFollower.start();
//		break;
//	case 'm':
//		lineFollower.stop();
//		break;
#endif

	case 'q':
	case 'e':
		drive(0, 0);
		break;
	}
}

void incSpeed() {
	speed += 50;
	speed = max(min(speed,400),-400);
//	LOGi("speed: %d", speed);
}

void decSpeed() {
	speed -= 50;
	speed = max(min(speed,400),-400);
//	LOGi("speed: %d", speed);
}
