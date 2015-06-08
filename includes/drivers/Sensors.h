/**
* 456789------------------------------------------------------------------------------------------------------------120
*
* @brief:
* @file: Sensors.h
*
* @desc:
*
*
* @author:        Dominik Egger
* @date:        May 14, 2015
* @project:    Zumo
*
*/

#pragma once

//-------------------------------------------------------------------
// INCLUDES
//-------------------------------------------------------------------

#include <QTRSensors.h>
#include <ZumoReflectanceSensorArray.h>
#include <ZumoBuzzer.h>

//-------------------------------------------------------------------
// CONFIG
//-------------------------------------------------------------------

//-------------------------------------------------------------------
// FUNCTIONS
//-------------------------------------------------------------------

extern ZumoReflectanceSensorArray reflectanceSensors;
extern ZumoBuzzer buzzer;

void initReflectantSensors();

