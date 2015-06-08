/**
 * 456789------------------------------------------------------------------------------------------------------------120
 *
 * @brief:
 * @file: Actuator.h
 *
 * @desc:
 *
 *
 *
 * @author:        Dominik Egger
 * @date:        Dec 6, 2014
 * @project:    Zumo
 *
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
