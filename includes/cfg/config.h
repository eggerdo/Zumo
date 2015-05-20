/**
* 456789------------------------------------------------------------------------------------------------------------120
*
* @brief:
* @file: config.h
*
* @desc: 
*
* @date:        May 14, 2015
* @project:    Zumo
*/

#pragma once

//-------------------------------------------------------------------
// INCLUDES
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// CONFIG
//-------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////
// Remote Control
/////////////////////////////////////////////////////////////////////

const int DEFAULT_SPEED				= 100;

/////////////////////////////////////////////////////////////////////
// Line Follower
/////////////////////////////////////////////////////////////////////

const int MAX_LINEFOLLOWER_SPEED	= 400;

/////////////////////////////////////////////////////////////////////
// Maze Solver
/////////////////////////////////////////////////////////////////////

#define WHITE_LINES true

// Motor speed when turning. TURN_SPEED should always
// have a positive value, otherwise the Zumo will turn
// in the wrong direction.
#define MAX_MAZESOLVER_TURN_SPEED 200

// Motor speed when driving straight. SPEED should always
// have a positive value, otherwise the Zumo will travel in the
// wrong direction.
#define MAX_MAZESOLVER_SPEED 200

// SENSOR_THRESHOLD is a value to compare reflectance sensor
// readings to to decide if the sensor is over a black line
#define SENSOR_THRESHOLD 300

// Thickness of your line in inches
#define LINE_THICKNESS 2.5

// When the motor speed of the zumo is set by
// motors.setSpeeds(200,200), 200 is in ZUNITs/Second.
// A ZUNIT is a fictitious measurement of distance
// and only helps to approximate how far the Zumo has
// traveled. Experimentally it was observed that for
// every inch, there were approximately 17142 ZUNITs.
// This value will differ depending on setup/battery
// life and may be adjusted accordingly. This value
// was found using a 75:1 HP Motors with batteries
// partially discharged.
#define INCHES_TO_ZUNITS 17142.0
