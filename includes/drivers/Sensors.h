/**
* 456789------------------------------------------------------------------------------------------------------------120
*
* @brief:
* @file: Sensors.h
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
* @date:        May 14, 2015
* @project:    Zumo
* @company:     Distributed Organisms B.V.
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

