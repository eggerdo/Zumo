/**
* 456789------------------------------------------------------------------------------------------------------------120
*
* @brief:
* @file: App.h
*
* @desc:
*
*
* @author:        Dominik Egger
* @date:        May 20, 2015
* @project:    Zumo
*
*/

#pragma once

//-------------------------------------------------------------------
// INCLUDES
//-------------------------------------------------------------------

#include "Zumo.h"
#include "drivers/Serial.h"
#include "byte_protocol.h"

//-------------------------------------------------------------------
// CONFIG
//-------------------------------------------------------------------

enum CustomCommands {
	INIT_MAZE = USER,
	START_MAZE,
	STOP_MAZE,
	REPEAT_MAZE,
	CALIBRATE_COMPSS,
	INIT_HEADING,
	TURN_DEG,
	SET_HEADING,
	SUMO_START,
	SUMO_STOP
};

struct turndegree_payload {
	int16_t angle;
};

//-------------------------------------------------------------------
// FUNCTIONS
//-------------------------------------------------------------------



