// Do not remove the include below
#include "Compass.h"
#include "Actuator.h"

#include <QTRSensors.h>
#include <ZumoReflectanceSensorArray.h>
#include <ZumoMotors.h>
#include <ZumoBuzzer.h>
//#include <Pushbutton.h>
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

//ZumoMotors _motors;
//Pushbutton button(ZUMO_BUTTON);

LSM303 compass;
bool calibrated = false;
float target_heading = 0;

void setTargetHeading(float heading) {
	target_heading = heading;
}

float getTargetHeading() {
	return target_heading;
}

// Setup will calibrate our compass by finding maximum/minimum magnetic readings
void calibrate()
{
	calibrated = false;

	// The highest possible magnetic value to read in any direction is 2047
	// The lowest possible magnetic value to read in any direction is -2047
	LSM303::vector<int16_t> running_min = {32767, 32767, 32767}, running_max = {-32767, -32767, -32767};
	unsigned char index;

	// Initiate the Wire library and join the I2C bus as a master
	Wire.begin();

	// Initiate LSM303
	compass.init();

	// Enables accelerometer and magnetometer
	compass.enableDefault();

	compass.writeReg(LSM303::CRB_REG_M, CRB_REG_M_2_5GAUSS); // +/- 2.5 gauss sensitivity to hopefully avoid overflow problems
	compass.writeReg(LSM303::CRA_REG_M, CRA_REG_M_220HZ);    // 220 Hz compass update rate

	delay(500);

	LOGi("starting calibration");

	// To calibrate the magnetometer, the Zumo spins to find the max/min
	// magnetic vectors. This information is used to correct for offsets
	// in the magnetometer data.
	drive(SPEED, -SPEED);

	for(index = 0; index < CALIBRATION_SAMPLES; index ++)
	{
		// Take a reading of the magnetic vector and store it in compass.m
		compass.read();

		running_min.x = min(running_min.x, compass.m.x);
		running_min.y = min(running_min.y, compass.m.y);

		running_max.x = max(running_max.x, compass.m.x);
		running_max.y = max(running_max.y, compass.m.y);

		Serial.println(index);

		delay(50);
	}

	drive_stop();

	LOGi("max.x   %d", running_max.x);
	LOGi("max.y   %d", running_max.y);
	LOGi("min.x   %d", running_min.x);
	LOGi("min.y   %d", running_min.y);

	// Set calibrated values to compass.m_max and compass.m_min
	compass.m_max.x = running_max.x;
	compass.m_max.y = running_max.y;
	compass.m_min.x = running_min.x;
	compass.m_min.y = running_min.y;

	calibrated = true;
}

void reportStart() {
	Looper::getInstance()->registerLoopFunc(reportHeading);
}

void reportDone() {
	Looper::getInstance()->unregisterLoopFunc(reportHeading);
}

int reportHeading() {
	if (!calibrated) return 0;

	float heading, relative_heading;

	// Heading is given in degrees away from the magnetic vector, increasing clockwise
	heading = averageHeading();

	// This gives us the relative heading with respect to the target angle
	relative_heading = relativeHeading(heading, target_heading);

	char target_heading_[32];
	dtostrf(target_heading, 6, 3, target_heading_);
	char heading_[32];
	dtostrf(heading, 6, 3, heading_);
	char relative_heading_[32];
	dtostrf(relative_heading, 6, 3, relative_heading_);

	//	LOGi("target_heading: %s", target_heading_);
	//	LOGi("heading: %s", heading_);
	//	LOGi("relative_heading: %s", relative_heading_);
	LOGi("target_heading: %s, heading: %s, relative_heading: %s", target_heading_, heading_, relative_heading_);

	return 200;
}

// Yields the angle difference in degrees between two headings
float relativeHeading(float heading_from, float heading_to)
{
	float relative_heading = heading_to - heading_from;

	// constrain to -180 to 180 degree range
	if (relative_heading > 180)
		relative_heading -= 360;
	if (relative_heading < -180)
		relative_heading += 360;

	return relative_heading;
}

// Average 10 vectors to get a better measurement and help smooth out
// the _motors' magnetic interference.
float averageHeading()
{
	LSM303::vector<int32_t> avg = {0, 0, 0};

	for(int i = 0; i < 10; i ++)
	{
		compass.read();
		avg.x += compass.m.x;
		avg.y += compass.m.y;
	}
	avg.x /= 10.0;
	avg.y /= 10.0;

	// avg is the average measure of the magnetic vector.
	return heading<int32_t>(avg);
}

bool isCalibrated() {
	return calibrated;
}

//float heading(LSM303::vector<int32_t> v)
//{
//	float x_scaled =  2.0*(float)(v.x - compass.m_min.x) / ( compass.m_max.x - compass.m_min.x) - 1.0;
//	float y_scaled =  2.0*(float)(v.y - compass.m_min.y) / (compass.m_max.y - compass.m_min.y) - 1.0;
//
//	float angle = atan2(y_scaled, x_scaled)*180 / M_PI;
//	if (angle < 0)
//		angle += 360;
//	return angle;
//}

