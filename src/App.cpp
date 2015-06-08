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

#include "App.h"

#include "drivers/Actuator.h"
#include "drivers/Compass.h"
#include "behaviours/MazeSolver.h"
#include "behaviours/Sumo.h"

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
		mazeSolver.init();
		break;
	}

	case START_MAZE: {
		LOGi("mazSolver.start");
		mazeSolver.start();
		break;
	}

	case STOP_MAZE: {
		LOGi("mazSolver.stop");
		mazeSolver.stop();
		break;
	}

	case REPEAT_MAZE: {
		LOGi("mazSolver.repeat");
		mazeSolver.repeat();
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
		setHeading(payload->angle);
		break;
	}
#endif

#ifdef SUMO
	case SUMO_START: {
		LOGi("sumo.start");
		sumo.start();
		break;
	}

	case SUMO_STOP: {
		LOGi("sumo.stop");
		sumo.stop();
		break;
	}
#endif

	}
}

// PRIVATE

