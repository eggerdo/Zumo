/**
* 456789------------------------------------------------------------------------------------------------------------120
*
* @brief:
* @file: App.cpp
*
* @desc:
*
*
* @author:        Dominik Egger
* @date:        May 20, 2015
* @project:    Zumo
*
*/

//-------------------------------------------------------------------
// INCLUDES
//-------------------------------------------------------------------

#include <programs/MazeSolver.h>
#include <programs/Sumo.h>
#include "App.h"

#include "drivers/Actuator.h"
#include "drivers/Compass.h"

//-------------------------------------------------------------------
// CONFIG
//-------------------------------------------------------------------

//-------------------------------------------------------------------
// FUNCTIONS
//-------------------------------------------------------------------

// PUBLIC

void onCustom(message_t* msg) {
	CustomCommands cmd = (CustomCommands)getType(msg);
	switch(cmd) {

#ifdef MAZESOLVER
	case INIT_MAZE: {
		LOGi("mazSolver.init");
//		Storage::setCurrentProgram(Prog_MazeSolver);
		LOGi("Program MazeSolver selected");
//		mazeSolver.init();
		break;
	}

	case START_MAZE: {
		LOGi("mazSolver.start");
//		mazeSolver.start();
		break;
	}

	case STOP_MAZE: {
		LOGi("mazSolver.stop");
//		mazeSolver.stop();
		break;
	}

	case REPEAT_MAZE: {
		LOGi("mazSolver.repeat");
//		mazeSolver.repeat();
		break;
	}

	case INIT_LINE_FOLLOWER: {
		LOGi("linefollower.init");
//		Storage::setCurrentProgram(Prog_LineFollower);
		LOGi("Program LineFollower selected");
//		mazeSolver.init();
		break;
	}

	case START_LINE_FOLLOWER: {
		LOGi("linefollower.start");
//		mazeSolver.start();
		break;
	}

	case STOP_LINE_FOLLOWER: {
		LOGi("linefollower.stop");
//		mazeSolver.stop();
		break;
	}
#endif

#ifdef LINE_FOLLOWER
	case INIT_LINE_FOLLOWER: {
		LOGi("linefollower.init");
//		linefollower.init();
		break;
	}

	case START_LINE_FOLLOWER: {
		LOGi("linefollower.start");
//		linefollower.start();
		break;
	}

	case STOP_LINE_FOLLOWER: {
		LOGi("linefollower.stop");
//		linefollower.stop();
		break;
	}
#endif

#ifdef USE_COMPASS
	case CALIBRATE_COMPSS: {
		LOGi("compass.calibrate");
		compass.calibrate();
		break;
	}

	case INIT_HEADING: {
		LOGi("compass.reset");
		calibrateHeading();
		break;
	}

	case TURN_DEG: {
		LOGi("compass.turn");
		turndegree_payload* payload = (turndegree_payload*) msg->payload;
		turnDegrees(payload->angle);
		break;
	}

	case SET_HEADING: {
		LOGi("setAbsAngle");
		turndegree_payload* payload = (turndegree_payload*) msg->payload;
		setTargetHeading(payload->angle);
		break;
	}
#endif

#ifdef SUMO
	case START_SUMO: {
		LOGi("sumo.start");
//		Storage::setCurrentProgram(Prog_Sumo);
		LOGi("Program Sumo selected");
//		sumo.start();
		break;
	}

	case STOP_SUMO: {
		LOGi("sumo.stop");
//		sumo.stop();
		break;
	}
#endif

//	case SET_PROGRAM: {
//		program_payload* payload = (program_payload*)msg->payload;
//		_currentProgram = (Program)payload->program;
//		Storage::setCurrentProgram(_currentProgram);
//		break;
//	}

	case SET_WHITE_LINES: {
		whitelines_payload* payload = (whitelines_payload*)msg->payload;
		_whiteLines = (bool)payload->whiteLines;
		Storage::setWhiteLines(_whiteLines);
		break;
	}

	}
}

// PRIVATE

