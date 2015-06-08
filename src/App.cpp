/**
* 456789------------------------------------------------------------------------------------------------------------120
*
* @brief:
* @file: App.cpp
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
* @date:        May 20, 2015
* @project:    Zumo
* @company:     Distributed Organisms B.V.
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

