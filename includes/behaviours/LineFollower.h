/**
 * 456789------------------------------------------------------------------------------------------------------------120
 *
 * @brief:
 * @file: LineFollower.h
 *
 * @desc:
 *
 *
 *
 * @author:        Dominik Egger
 * @date:        Mar 28, 2015
 * @project:    Zumo
 *
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
