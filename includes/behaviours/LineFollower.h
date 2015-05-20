/**
 * 456789------------------------------------------------------------------------------------------------------------120
 *
 * @brief:
 * @file: LineFollower.h
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
 * @date:        Mar 28, 2015
 * @project:    Zumo
 * @company:     Distributed Organisms B.V.
 */

#pragma once

//-------------------------------------------------------------------
// INCLUDES
//-------------------------------------------------------------------

#include "Zumo.h"
#include "Sensors.h"

//
//#include <QTRSensors.h>
//#include <ZumoReflectanceSensorArray.h>
//#include <ZumoBuzzer.h>

//-------------------------------------------------------------------
// CONFIG
//-------------------------------------------------------------------

//-------------------------------------------------------------------
// FUNCTIONS
//-------------------------------------------------------------------


class LineFollower {
public:
	LineFollower() : _calibrated(false) {};
	virtual ~LineFollower() {};

	bool isCalibrated() { return _calibrated; }

	void init();
	void start();
	void stop();

	int execute();
private:
	bool _calibrated;

};

extern LineFollower lineFollower;
