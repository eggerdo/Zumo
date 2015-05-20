/**
 * 456789------------------------------------------------------------------------------------------------------------120
 *
 * @brief:
 * @file: LineFollower.cpp
 *
 *
 * adapter from demo line-following code for the Pololu Zumo Robot:
 *
 * This code will follow a black line on a white background, using a
 * PID-based algorithm.  It works decently on courses with smooth, 6"
 * radius curves and has been tested with Zumos using 30:1 HP and
 * 75:1 HP motors.  Modifications might be required for it to work
 * well on different courses or with different motors.
 *
 * http://www.pololu.com/catalog/product/2506
 * http://www.pololu.com
 * http://forum.pololu.com
 *
 *
 *
 * @author:        Dominik Egger
 * @date:        Mar 28, 2015
 * @project:    Zumo
 */

//-------------------------------------------------------------------
// INCLUDES
//-------------------------------------------------------------------
#include "LineFollower.h"

#include "cfg/config.h"
#include "drivers/Actuator.h"

//-------------------------------------------------------------------
// CONFIG
//-------------------------------------------------------------------

int lastError = 0;
LineFollower lineFollower;

//-------------------------------------------------------------------
// FUNCTIONS
//-------------------------------------------------------------------

void LineFollower::init()
{
//	LOGi("LineFollower init...");

	unsigned int sensors[6];

	// Play a little welcome song
	buzzer.play(">g32>>c32");

	// Initialize the reflectance sensors module
//	reflectanceSensors.init(QTR_NO_EMITTER_PIN);
//	reflectanceSensors.init(A4);
	initReflectantSensors();

	// Turn on LED to indicate we are in calibration mode
//	pinMode(13, OUTPUT);
//	digitalWrite(13, HIGH);

	// Wait 1 second and then begin automatic sensor calibration
	// by rotating in place to sweep the sensors over the line
	delay(1000);
	int i;
	for(i = 0; i < 80; i++)
	{
		if ((i > 10 && i <= 30) || (i > 50 && i <= 70))
			drive(-200, 200);
		else
			drive(200, -200);
		reflectanceSensors.calibrate();

		reflectanceSensors.readLine(sensors);

		delay(10);
//		LOGi("sensor[0]: %d, sensor[5]: %d", sensors[0], sensors[5]);

		// Since our counter runs to 80, the total delay will be
		// 80*20 = 1600 ms.
		delay(20);
	}
	drive_stop();

	// Turn off LED to indicate we are through with calibration
//	digitalWrite(13, LOW);
//	buzzer.play(">g32>>c32");

	_calibrated = true;
//	LOGi("... done");
}

extern "C" int run_linefollower() {
	return lineFollower.execute();
}

void LineFollower::start() {
	if (!_calibrated) return;

//	LOGi("LineFollower start...");

	// Play music and wait for it to finish before we start driving.
	buzzer.play("L16 cdegreg4");
	while(buzzer.isPlaying());

	Looper::getInstance()->registerLoopFunc(run_linefollower);
}

void LineFollower::stop() {
	if (!_calibrated) return;

//	LOGi("LineFollower stop...");

	Looper::getInstance()->unregisterLoopFunc(run_linefollower);
	drive_stop();
}

int LineFollower::execute()
{
	unsigned int sensors[6];

	// Get the position of the line.  Note that we *must* provide the "sensors"
	// argument to readLine() here, even though we are not interested in the
	// individual sensor readings
	int position = reflectanceSensors.readLine(sensors);

	// Our "error" is how far we are away from the center of the line, which
	// corresponds to position 2500.
	int error = position - 2500;

	// Get motor speed difference using proportional and derivative PID terms
	// (the integral term is generally not very useful for line following).
	// Here we are using a proportional constant of 1/4 and a derivative
	// constant of 6, which should work decently for many Zumo motor choices.
	// You probably want to use trial and error to tune these constants for
	// your particular Zumo and line course.
	int speedDifference = error / 4 + 6 * (error - lastError);

	lastError = error;

	// Get individual motor speeds.  The sign of speedDifference
	// determines if the robot turns left or right.
	int m1Speed = MAX_LINEFOLLOWER_SPEED + speedDifference;
	int m2Speed = MAX_LINEFOLLOWER_SPEED - speedDifference;

	// Here we constrain our motor speeds to be between 0 and MAX_SPEED.
	// Generally speaking, one motor will always be turning at MAX_SPEED
	// and the other will be at MAX_SPEED-|speedDifference| if that is positive,
	// else it will be stationary.  For some applications, you might want to
	// allow the motor speed to go negative so that it can spin in reverse.
	if (m1Speed < 0)
		m1Speed = 0;
	if (m2Speed < 0)
		m2Speed = 0;
	if (m1Speed > MAX_LINEFOLLOWER_SPEED)
		m1Speed = MAX_LINEFOLLOWER_SPEED;
	if (m2Speed > MAX_LINEFOLLOWER_SPEED)
		m2Speed = MAX_LINEFOLLOWER_SPEED;

	drive(m1Speed, m2Speed);

	return 0;
}
