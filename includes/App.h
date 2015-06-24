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
	INIT_MAZE = USER,		// 20 (0x14)
	START_MAZE,				// 21
	STOP_MAZE,				// 22
	REPEAT_MAZE,			// 23
	CALIBRATE_COMPSS,		// 24
	INIT_HEADING,			// 25
	TURN_DEG,				// 26 (0x1A)
	SET_HEADING,			// 27
	INIT_LINE_FOLLOWER,		// 28
	START_LINE_FOLLOWER,	// 29
	STOP_LINE_FOLLOWER,		// 30
	START_SUMO,				// 31 (0x1F)
	STOP_SUMO,				// 32
	SET_WHITE_LINES,		// 33
//	SET_PROGRAM,
};

struct whitelines_payload {
	uint8_t whiteLines;
};

struct program_payload {
	uint8_t program;
};

struct turndegree_payload {
	int16_t angle;
};

//-------------------------------------------------------------------
// FUNCTIONS
//-------------------------------------------------------------------



