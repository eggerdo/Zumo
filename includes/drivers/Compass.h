// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section
#pragma once

#include "Zumo.h"
//add your includes for the project Zumo_Shield here

#include <LSM303.h>

#define CALIBRATION_SAMPLES 70  // Number of compass readings to take when calibrating
#define CRB_REG_M_2_5GAUSS 0x60 // CRB_REG_M value for magnetometer +/-2.5 gauss full scale
#define CRA_REG_M_220HZ    0x1C // CRA_REG_M value for magnetometer 220 Hz update rate

// Converts x and y components of a vector to a heading in degrees.
// This function is used instead of LSM303::heading() because we don't
// want the acceleration of the Zumo to factor spuriously into the
// tilt compensation that LSM303::heading() performs. This calculation
// assumes that the Zumo is always level.
template <typename T> int heading(LSM303& compass, LSM303::vector<T> v)
{
  float x_scaled =  2.0*(float)(v.x - compass.m_min.x) / ( compass.m_max.x - compass.m_min.x) - 1.0;
  float y_scaled =  2.0*(float)(v.y - compass.m_min.y) / (compass.m_max.y - compass.m_min.y) - 1.0;

  int angle = (int)(atan2(y_scaled, x_scaled)*180 / M_PI);
  if (angle < 0)
    angle += 360;
  return angle;
}

class Compass {

public:
//	static Compass& getInstance() {
//		static Compass instance;
//		return instance;
//	}

	Compass() : _calibrated(false), _targetHeading(0) {}

	void calibrate();

	bool isCalibrated() { return _calibrated; }

	int relativeHeading(int heading_from, int heading_to);

	int averageHeading();

	void reportStart();
	void reportDone();

	int reportHeading();

	void setTargetHeading(int heading);
	int getTargetHeading();

private:

	LSM303 _compass;

	bool _calibrated;
	int _targetHeading;

};

extern Compass compass;
