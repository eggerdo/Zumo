/**
 * 456789------------------------------------------------------------------------------------------------------------120
 *
 * @brief:
 * @file: Actuator.h
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

#ifndef ACTUATOR_H_
#define ACTUATOR_H_

//-------------------------------------------------------------------
// INCLUDES
//-------------------------------------------------------------------

#include "Zumo.h"

//-------------------------------------------------------------------
// CONFIG
//-------------------------------------------------------------------

// Allowed deviation (in degrees) relative to target angle that must be achieved before driving straight
#define DEVIATION_THRESHOLD 5

#define SPEED           200 // Maximum motor speed when going straight; variable speed when turning
#define TURN_BASE_SPEED 100 // Base speed when turning (added to variable speed)

#define CAPSPEED 400 // this cap should be low enough for all phones (give ppl option to up it for better phones)

//-------------------------------------------------------------------
// FUNCTIONS
//-------------------------------------------------------------------

void initMotors();

int capSpeed(int value);
void drive(int leftSpeed, int rightSpeed);
void drive_stop();


void setHeading(int angle);
void calibrateHeading();
void turnDegrees(int angle);
int doTurn();

#endif /* ACTUATOR_H_ */
