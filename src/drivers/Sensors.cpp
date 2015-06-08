/**
* 456789------------------------------------------------------------------------------------------------------------120
*
* @brief:
* @file: Sensors.cpp
*
* @desc:
*
*
* @author:        Dominik Egger
* @date:        May 14, 2015
* @project:    Zumo
*
*/

//-------------------------------------------------------------------
// INCLUDES
//-------------------------------------------------------------------

#include "drivers/Sensors.h"

//-------------------------------------------------------------------
// CONFIG
//-------------------------------------------------------------------

ZumoReflectanceSensorArray reflectanceSensors;
ZumoBuzzer buzzer;

//-------------------------------------------------------------------
// FUNCTIONS
//-------------------------------------------------------------------

// PUBLIC

void initReflectantSensors() {
	reflectanceSensors.init(A4);
//	reflectanceSensors.init(QTR_NO_EMITTER_PIN);
	reflectanceSensors.emittersOff();
}

// PRIVATE

