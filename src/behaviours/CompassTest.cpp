/**
 * 456789------------------------------------------------------------------------------------------------------------120
 *
 * @brief:
 * @file: CompassTest.cpp
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
 * @date:        Mar 28, 2015
 * @project:    Zumo
 * @company:     Distributed Organisms B.V.
 */

#include "CompassTest.h"
#include "drivers/Actuator.h"
#include "drivers/Compass.h"

#define STRAIGHT 1
#define TURN 2
int state = TURN;

unsigned long startStraight;

void driveSquare() {
	LOGi("start Drive Square...");
	if (isCalibrated()) {
		state = TURN;
		setTargetHeading(averageHeading());
		Looper::getInstance()->registerLoopFunc(onDriveSquare);
		reportStart();
	}
}

void stopDriving() {
	LOGi(".. stop Drive Square");
	Looper::getInstance()->unregisterLoopFunc(onDriveSquare);
	reportDone();
	drive_stop();
}

int onDriveSquare()
{
	float heading, relative_heading;
	int speed;
//	target_heading = averageHeading();

	// Heading is given in degrees away from the magnetic vector, increasing clockwise
	heading = averageHeading();

	// This gives us the relative heading with respect to the target angle
	relative_heading = relativeHeading(heading, getTargetHeading());

	// If the Zumo has turned to the direction it wants to be pointing, go straight and then do another turn
	if(abs(relative_heading) < DEVIATION_THRESHOLD && state == TURN) {
		state = STRAIGHT;
		startStraight = Looper::now();
		LOGi("   Straight");
//	} else {
//		state = TURN;
//		LOGi("   Turn");
	}

	if (state == STRAIGHT) {
		if (Looper::now() < startStraight + 1000) {
			drive(SPEED, SPEED);
		} else if (Looper::now() < startStraight + 2000) {

		} else {
			// Turn off _motors and wait a short time to reduce interference from _motors
			drive_stop();
			delay(100);
			state = TURN;
			LOGi("   Turn");

			// Turn 90 degrees relative to the direction we are pointing.
			// This will help account for variable magnetic field, as opposed
			// to using fixed increments of 90 degrees from the initial
			// heading (which might have been measured in a different magnetic
			// field than the one the Zumo is experiencing now).
			// Note: fmod() is floating point modulo
			setTargetHeading(fmod(averageHeading() + 90, 360));

//			LOGi("   Turn");
		}
	}
	else
	{
		// To avoid overshooting, the closer the Zumo gets to the target
		// heading, the slower it should turn. Set the motor speeds to a
		// minimum base amount plus an additional variable amount based
		// on the heading difference.

		speed = SPEED*relative_heading/180;

		if (speed < 0)
			speed -= TURN_BASE_SPEED;
		else
			speed += TURN_BASE_SPEED;

		drive(speed, -speed);
	}

	return 0;
}

