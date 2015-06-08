/**
 * 456789------------------------------------------------------------------------------------------------------------120
 *
 * @brief:
 * @file: SumoCollision.cpp
 *
 *
 * @author:        Dominik Egger
 * @date:        May 14, 2015
 * @project:    Zumo
 *
 */


/* This example uses the accelerometer in the Zumo Shield's onboard LSM303DLHC with the LSM303 Library to
 * detect contact with an adversary robot in the sumo ring. The LSM303 Library is not included in the Zumo
 * Shield libraries; it can be downloaded separately from GitHub at:
 *
 *    https://github.com/pololu/LSM303
 *
 * This example extends the BorderDetect example, which makes use of the onboard Zumo Reflectance Sensor Array
 * and its associated library to detect the border of the sumo ring.  It also illustrates the use of the
 * ZumoMotors, PushButton, and ZumoBuzzer libraries.
 *
 * In loop(), the program reads the x and y components of acceleration (ignoring z), and detects a
 * contact when the magnitude of the 3-period average of the x-y vector exceeds an empirically determined
 * XY_ACCELERATION_THRESHOLD.  On contact detection, the forward speed is increased to FULL_SPEED from
 * the default SEARCH_SPEED, simulating a "fight or flight" response.
 *
 * The program attempts to detect contact only when the Zumo is going straight.  When it is executing a
 * turn at the sumo ring border, the turn itself generates an acceleration in the x-y plane, so the
 * acceleration reading at that time is difficult to interpret for contact detection.  Since the Zumo also
 * accelerates forward out of a turn, the acceleration readings are also ignored for MIN_DELAY_AFTER_TURN
 * milliseconds after completing a turn. To further avoid false positives, a MIN_DELAY_BETWEEN_CONTACTS is
 * also specified.
 *
 * This example also contains the following enhancements:
 *
 *  - uses the Zumo Buzzer library to play a sound effect ("charge" melody) at start of competition and
 *    whenever contact is made with an opposing robot
 *
 *  - randomizes the turn angle on border detection, so that the Zumo executes a more effective search pattern
 *
 *  - supports a FULL_SPEED_DURATION_LIMIT, allowing the robot to switch to a SUSTAINED_SPEED after a short
 *    period of forward movement at FULL_SPEED.  In the example, both speeds are set to 400 (max), but this
 *    feature may be useful to prevent runoffs at the turns if the sumo ring surface is unusually smooth.
 *
 *  - logging of accelerometer output to the serial monitor when LOG_SERIAL is #defined.
 *
 *  This example also makes use of the public domain RunningAverage library from the Arduino website; the relevant
 *  code has been copied into this .ino file and does not need to be downloaded separately.
 */

//-------------------------------------------------------------------
// INCLUDES
//-------------------------------------------------------------------

#include <Sumo.h>
#include "Sensors.h"

#include "drivers/Actuator.h"
#include "drivers/Button.h"

#include <Wire.h>

#include <stdlib.h>

//-------------------------------------------------------------------
// CONFIG
//-------------------------------------------------------------------

Sumo sumo;

//-------------------------------------------------------------------
// FUNCTIONS
//-------------------------------------------------------------------

// PUBLIC


// PRIVATE

void Sumo::init()
{

	// Initiate the Wire library and join the I2C bus as a master
	Wire.begin();

	// Initiate LSM303
	_lsm303 = new Accelerometer();
	_lsm303->init();
	_lsm303->enable();

#ifdef LOG_SERIAL
	_lsm303->getLogHeader();
#endif

	randomSeed((unsigned int) millis());

	pinMode(LED, HIGH);
	buzzer.playMode(PLAY_AUTOMATIC);
	//	waitForButtonAndCountDown(false);

	_initialized = true;
}

extern "C" int run_sumo_collision_detection() {
	return sumo.execute();
}

void Sumo::start() {
	if (_running) return;

	if (!isInitialized()) {
		init();
	}

	LOGi("Sumo start...");
	reset();

	// play audible countdown
	for (int i = 0; i < 3; i++)
	{
		delay(1000);
		buzzer.playNote(NOTE_G(3), 50, 12);
	}

	delay(1000);
	buzzer.playFromProgramSpace(sound_effect);
	delay(1000);

	// reset loop variables
	reset();

	Looper::getInstance().registerLoopFunc(run_sumo_collision_detection);
	_running = true;
}

void Sumo::stop() {
	LOGi("Sumo stop...");

	Looper::getInstance().unregisterLoopFunc(run_sumo_collision_detection);
	drive_stop();
	_running = false;
}

bool compare(int value, int threshold) {
	if (WHITE_LINES) {
		return value < threshold;
	} else {
		return value > threshold;
	}
}

int Sumo::execute()
{

	if (checkForButton()) {
		stop();
		return 0;
	}

	_loopStartTime = millis();
	_lsm303->readAcceleration(_loopStartTime);
	reflectanceSensors.read(_sensorValues);

//	LOGi("%d, %d, %d, %d, %d, %d", _sensorValues[0], _sensorValues[1], _sensorValues[2], _sensorValues[3], _sensorValues[4], _sensorValues[5]);

	if ((_forwardSpeed == FullSpeed) && (_loopStartTime - _fullSpeedStartTime > FULL_SPEED_DURATION_LIMIT))
	{
		setForwardSpeed(SustainedSpeed);
	}

	if (compare(_sensorValues[0], QTR_THRESHOLD))
	{
		// if leftmost sensor detects line, reverse and turn to the right
		turn(RIGHT, true);
	}
	else if (compare(_sensorValues[5], QTR_THRESHOLD))
	{
		// if rightmost sensor detects line, reverse and turn to the left
		turn(LEFT, true);
	}
	else  // otherwise, go straight
	{
		if (checkForContact()) {
			onContactMade();
		}
		int speed = getForwardSpeed();
		drive(speed, speed);
	}

	return 0;
}

// execute turn
// direction:  RIGHT or LEFT
// randomize: to improve searching
void Sumo::turn(char direction, bool randomize)
{
#ifdef LOG_SERIAL
	LOGi("turning ...");
#endif

	// assume contact lost
	onContactLost();

	static unsigned int duration_increment = TURN_DURATION / 4;

	// drive(0,0);
	// delay(STOP_DURATION);
	drive(-REVERSE_SPEED, -REVERSE_SPEED);
	delay(REVERSE_DURATION);

	drive(TURN_SPEED * direction, -TURN_SPEED * direction);
	delay(randomize ? TURN_DURATION + (random(8) - 2) * duration_increment : TURN_DURATION);

	int speed = getForwardSpeed();
	drive(speed, speed);

	_lastTurnTime = millis();
}

void Sumo::setForwardSpeed(ForwardSpeed speed)
{
	_forwardSpeed = speed;
	if (speed == FullSpeed) {
		_fullSpeedStartTime = _loopStartTime;
	}
}

int Sumo::getForwardSpeed()
{
	int speed;
	switch (_forwardSpeed)
	{
	case FullSpeed:
		speed = FULL_SPEED;
		break;
	case SustainedSpeed:
		speed = SUSTAINED_SPEED;
		break;
	default:
		speed = SEARCH_SPEED;
		break;
	}
	return speed;
}

// check for contact, but ignore readings immediately after turning or losing contact
bool Sumo::checkForContact()
{
	static long threshold_squared = (long) XY_ACCELERATION_THRESHOLD * (long) XY_ACCELERATION_THRESHOLD;
	return (_lsm303->ss_xy_avg() >  threshold_squared) && \
			(_loopStartTime - _lastTurnTime > MIN_DELAY_AFTER_TURN) && \
			(_loopStartTime - _contactMadeTime > MIN_DELAY_BETWEEN_CONTACTS);
}

// sound horn and accelerate on contact -- fight or flight
void Sumo::onContactMade()
{
#ifdef LOG_SERIAL
	LOGi("contact made");
#endif
	_inContact = true;
	_contactMadeTime = _loopStartTime;
	setForwardSpeed(FullSpeed);
	buzzer.playFromProgramSpace(sound_effect);
}

// reset forward speed
void Sumo::onContactLost()
{
#ifdef LOG_SERIAL
	LOGi("contact lost");
#endif
	_inContact = false;
	setForwardSpeed(SearchSpeed);
}

// class Accelerometer -- member function definitions

// enable accelerometer only
// to enable both accelerometer and magnetometer, call enableDefault() instead
void Accelerometer::enable(void)
{
	// Enable Accelerometer
	// 0x27 = 0b00100111
	// Normal power mode, all axes enabled
	writeAccReg(LSM303::CTRL_REG1_A, 0x27);

	if (getDeviceType() == LSM303::device_DLHC)
		writeAccReg(LSM303::CTRL_REG4_A, 0x08); // DLHC: enable high resolution mode
}

void Accelerometer::getLogHeader(void)
{
	LOGi("millis    x      y     len     dir  | len_avg  dir_avg  |  avg_len");
}

void Accelerometer::readAcceleration(unsigned long timestamp)
{
	readAcc();
	if (a.x == last.x && a.y == last.y) return;

	last.timestamp = timestamp;
	last.x = a.x;
	last.y = a.y;

	ra_x.addValue(last.x);
	ra_y.addValue(last.y);


#ifdef LOG_SERIAL
	btStream->print(last.timestamp);
	btStream->print("  ");
	btStream->print(last.x);
	btStream->print("  ");
	btStream->print(last.y);
	btStream->print("  ");
	btStream->print(len_xy());
	btStream->print("  ");
	btStream->print(dir_xy());
	btStream->print("  |  ");
	btStream->print(sqrt(static_cast<float>(ss_xy_avg())));
	btStream->print("  ");
	btStream->print(dir_xy_avg());
	btStream->println();
#endif
}

float Accelerometer::len_xy() const
{
	return sqrt(last.x*a.x + last.y*a.y);
}

float Accelerometer::dir_xy() const
{
	return atan2(last.x, last.y) * 180.0 / M_PI;
}

int Accelerometer::x_avg(void) const
{
	return ra_x.getAverage();
}

int Accelerometer::y_avg(void) const
{
	return ra_y.getAverage();
}

long Accelerometer::ss_xy_avg(void) const
{
	long x_avg_long = static_cast<long>(x_avg());
	long y_avg_long = static_cast<long>(y_avg());
	return x_avg_long*x_avg_long + y_avg_long*y_avg_long;
}

float Accelerometer::dir_xy_avg(void) const
{
	return atan2(static_cast<float>(x_avg()), static_cast<float>(y_avg())) * 180.0 / M_PI;
}



// RunningAverage class
// based on RunningAverage library for Arduino
// source:  http://playground.arduino.cc/Main/RunningAverage
// author:  Rob.Tillart@gmail.com
// Released to the public domain

template <typename T>
T RunningAverage<T>::zero = static_cast<T>(0);

template <typename T>
RunningAverage<T>::RunningAverage(int n)
{
	_size = n;
	_ar = (T*) malloc(_size * sizeof(T));
	clear();
}

template <typename T>
RunningAverage<T>::~RunningAverage()
{
	free(_ar);
}

// resets all counters
template <typename T>
void RunningAverage<T>::clear()
{
	_cnt = 0;
	_idx = 0;
	_sum = zero;
	for (int i = 0; i< _size; i++) _ar[i] = zero;  // needed to keep addValue simple
}

// adds a new value to the data-set
template <typename T>
void RunningAverage<T>::addValue(T f)
{
	_sum -= _ar[_idx];
	_ar[_idx] = f;
	_sum += _ar[_idx];
	_idx++;
	if (_idx == _size) _idx = 0;  // faster than %
	if (_cnt < _size) _cnt++;
}

// returns the average of the data-set added so far
template <typename T>
T RunningAverage<T>::getAverage() const
{
	if (_cnt == 0) return zero; // NaN ?  math.h
	return _sum / _cnt;
}

// fill the average with a value
// the param number determines how often value is added (weight)
// number should preferably be between 1 and size
template <typename T>
void RunningAverage<T>::fillValue(T value, int number)
{
	clear();
	for (int i = 0; i < number; i++)
	{
		addValue(value);
	}
}
