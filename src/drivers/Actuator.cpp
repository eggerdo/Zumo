/**
 * 456789------------------------------------------------------------------------------------------------------------120
 *
 * @brief:
 * @file: Actuator.cpp
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
 * @date:        Dec 6, 2014
 * @project:    Zumo
 * @company:     Distributed Organisms B.V.
 */


#include "Actuator.h"

#include "Zumo.h"
#include "Compass.h"
#include <ZumoMotors.h>

ZumoMotors motors;

#define CAPSPEED 400 // this cap should be low enough for all phones (give ppl option to up it for better phones)

// --------------------------------------------------------------------
// BASIC DRIVING
// --------------------------------------------------------------------

// --------------------------------------------------------------------
int capSpeed(int value)
{
	return max(min(value,CAPSPEED),-CAPSPEED);
}

// --------------------------------------------------------------------
void drive(int leftSpeed, int rightSpeed)
{
	int left = capSpeed(leftSpeed);
	int right = capSpeed(rightSpeed);

	LOGd("drive(%d, %d)", leftSpeed, rightSpeed);
	LOGd("driveCapped(%d, %d)", left, right);

	motors.setLeftSpeed(left);
	motors.setRightSpeed(right);
}

// --------------------------------------------------------------------
void drive_stop() {
	drive(0,0);
}


// --------------------------------------------------------------------
// COMPASS DRIVING
// --------------------------------------------------------------------

// --------------------------------------------------------------------
// PUBLIC
// --------------------------------------------------------------------
static float target_heading;
void turnDegrees(float angle) {
	if (!isCalibrated()) {
		LOGe("calibrate first!!");
		return;
	}

	target_heading = fmod(averageHeading() + angle, 360);

	char* _angle = floatToString(angle);
	LOGi("turnDegrees: %s", _angle);
	free(_angle);

	reportHeading();

	Looper::getInstance()->registerLoopFunc(doTurn);
	reportStart();
}

static float headingOffset = 0;
void calibrateHeading() {
	if (!isCalibrated()) {
		LOGe("calibrate first!!");
		return;
	}

	headingOffset = averageHeading();

	char* _headingOffset = floatToString(headingOffset);
	LOGd("headingOffset: %s", _headingOffset);
	free(_headingOffset);

}

void setHeading(float angle) {
	if (!isCalibrated()) {
		LOGe("calibrate first!!");
		return;
	}

	target_heading = fmod(headingOffset + angle, 360);

	char* _angle = floatToString(angle);
	LOGi("setHeading to: %s", _angle);
	free(_angle);

	reportHeading();

	Looper::getInstance()->registerLoopFunc(doTurn);
	reportStart();
}


// --------------------------------------------------------------------
// PRIVATE
// --------------------------------------------------------------------

int doTurn() {
	float heading, relative_heading;
	int speed;

	// Heading is given in degrees away from the magnetic vector, increasing clockwise
	heading = averageHeading();

	// This gives us the relative heading with respect to the target angle
	relative_heading = relativeHeading(heading, target_heading);

	if(abs(relative_heading) < DEVIATION_THRESHOLD) {
		LOGi("... done");
		Looper::getInstance()->unregisterLoopFunc(doTurn);
		reportDone();
		drive_stop();
		return 0;
	}

	// To avoid overshooting, the closer the Zumo gets to the target
	// heading, the slower it should turn. Set the motor speeds to a
	// minimum base amount plus an additional variable amount based
	// on the heading difference.

	speed = SPEED*relative_heading/180;

	if (speed < 0)
		speed -= TURN_BASE_SPEED;
	else
		speed += TURN_BASE_SPEED;

	LOGd("turn: %d", speed);
	drive(speed, -speed);

	return 0;
}