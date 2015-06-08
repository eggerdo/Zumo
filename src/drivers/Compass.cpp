// Do not remove the include below
#include "Compass.h"

#include "drivers/Actuator.h"
//#include "drivers/Sensors.h"
#include <Wire.h>

/* This example uses the magnetometer in the Zumo Shield's onboard
 * LSM303DLHC to help the Zumo make precise 90-degree turns and drive
 * in squares. It uses the Zumo_motors, Pushbutton, and LSM303
 * (compass) libraries. The LSM303 library is not included in the Zumo
 * Shield libraries; it can be downloaded from GitHub at:
 *
 *   https://github.com/pololu/LSM303
 *
 * This program first calibrates the compass to account for offsets in
 *  its output. Calibration is accomplished in setup().
 *
 * In loop(), The driving angle then changes its offset by 90 degrees
 * from the heading every second. Essentially, this navigates the
 * Zumo to drive in square patterns.
 *
 * It is important to note that stray magnetic fields from electric
 * current (including from the Zumo's own _motors) and the environment
 * (for example, steel rebar in a concrete floor) might adversely
 * affect readings from the LSM303 compass and make them less
 * reliable.
 */

//LSM303 compass;

Compass compass;

void Compass::setTargetHeading(int heading) {
	_targetHeading = heading;
}

int Compass::getTargetHeading() {
	return _targetHeading;
}

// Setup will calibrate our compass by finding maximum/minimum magnetic readings
void Compass::calibrate()
{
	_calibrated = false;

	// The highest possible magnetic value to read in any direction is 2047
	// The lowest possible magnetic value to read in any direction is -2047
	LSM303::vector<int16_t> running_min = {32767, 32767, 32767}, running_max = {-32767, -32767, -32767};
	unsigned char index;

	// Initiate the Wire library and join the I2C bus as a master
	Wire.begin();

	// Initiate LSM303
	_compass.init();

	// Enables accelerometer and magnetometer
	_compass.enableDefault();

	_compass.writeReg(LSM303::CRB_REG_M, CRB_REG_M_2_5GAUSS); // +/- 2.5 gauss sensitivity to hopefully avoid overflow problems
	_compass.writeReg(LSM303::CRA_REG_M, CRA_REG_M_220HZ);    // 220 Hz compass update rate

	delay(500);

	LOGi("starting calibration");

	// To calibrate the magnetometer, the Zumo spins to find the max/min
	// magnetic vectors. This information is used to correct for offsets
	// in the magnetometer data.
	drive(SPEED, -SPEED);

	for(index = 0; index < CALIBRATION_SAMPLES; index ++)
	{
		// Take a reading of the magnetic vector and store it in compass.m
		_compass.read();

		running_min.x = min(running_min.x, _compass.m.x);
		running_min.y = min(running_min.y, _compass.m.y);

		running_max.x = max(running_max.x, _compass.m.x);
		running_max.y = max(running_max.y, _compass.m.y);

		delay(50);
	}

	drive_stop();

	LOGi("max.x   %d", running_max.x);
	LOGi("max.y   %d", running_max.y);
	LOGi("min.x   %d", running_min.x);
	LOGi("min.y   %d", running_min.y);

	// Set _calibrated values to compass.m_max and compass.m_min
	_compass.m_max.x = running_max.x;
	_compass.m_max.y = running_max.y;
	_compass.m_min.x = running_min.x;
	_compass.m_min.y = running_min.y;

	_calibrated = true;
}

extern "C" int report_heading() {
//	return Compass::getInstance().reportHeading();
	return compass.reportHeading();
}

void Compass::reportStart() {
	Looper::getInstance().registerLoopFunc(report_heading);
}

void Compass::reportDone() {
	Looper::getInstance().unregisterLoopFunc(report_heading);
}

int Compass::reportHeading() {
	if (!_calibrated) return 0;

	int heading, relative_heading;

	// Heading is given in degrees away from the magnetic vector, increasing clockwise
	heading = averageHeading();

	// This gives us the relative heading with respect to the target angle
	relative_heading = relativeHeading(heading, _targetHeading);

//	char target_heading_[32];
//	dtostrf(_targetHeading, 6, 3, target_heading_);
//	char heading_[32];
//	dtostrf(heading, 6, 3, heading_);
//	char relative_heading_[32];
//	dtostrf(relative_heading, 6, 3, relative_heading_);
//
//	LOGi("_targetHeading: %s, heading: %s, relative_heading: %s", target_heading_, heading_, relative_heading_);
	LOGi("targetHeading: %d, heading: %d, relative_heading: %d", _targetHeading, heading, relative_heading);

	return 200;
}

// Yields the angle difference in degrees between two headings
int Compass::relativeHeading(int heading_from, int heading_to)
{
	int relative_heading = heading_to - heading_from;

	// constrain to -180 to 180 degree range
	if (relative_heading > 180)
		relative_heading -= 360;
	if (relative_heading < -180)
		relative_heading += 360;

	return relative_heading;
}

// Average 10 vectors to get a better measurement and help smooth out
// the _motors' magnetic interference.
int Compass::averageHeading()
{
	LSM303::vector<int32_t> avg = {0, 0, 0};

	for(int i = 0; i < 10; i ++)
	{
		_compass.read();
		avg.x += _compass.m.x;
		avg.y += _compass.m.y;
	}
	avg.x /= 10.0;
	avg.y /= 10.0;

	// avg is the average measure of the magnetic vector.
	return heading<int32_t>(_compass, avg);
}
