/**
 * 456789------------------------------------------------------------------------------------------------------------120
 *
 * @brief:
 * @file: Commander.cpp
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
 * @date:        Mar 21, 2015
 * @project:    Zumo
 * @company:     Distributed Organisms B.V.
 */

//-------------------------------------------------------------------
// INCLUDES
//-------------------------------------------------------------------

#include <cfg/Config.h>
#include "Commander.h"

#include "drivers/Serial.h"
#include "drivers/Compass.h"
#include "drivers/Actuator.h"

#include "behaviours/CompassTest.h"
#include "behaviours/LineFollower.h"
#include "behaviours/MazeSolver.h"

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
		setHeading(-135);
		break;
	case '2':
		setHeading(180);
		break;
	case '3':
		setHeading(135);
		break;
	case '4':
		setHeading(-90);
		break;
	case '5':
		if (compass.isCalibrated()) {
			calibrateHeading();
		}
		break;
	case '6':
		setHeading(90);
		break;
	case '7':
		setHeading(-45);
		break;
	case '8':
		setHeading(0);
		break;
	case '9':
		setHeading(45);
		break;

	// compass
	case 'h':
		compass.calibrate();
		break;

	// compass
	case 'i':
		turnDegrees(90);
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
