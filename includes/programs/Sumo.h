/**
 * 456789------------------------------------------------------------------------------------------------------------120
 *
 * @brief:
 * @file: SumoCollision.h
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

//#include <QTRSensors.h>
//#include <ZumoReflectanceSensorArray.h>
//#include <ZumoBuzzer.h>

#include "Zumo.h"
#include <LSM303.h>

//-------------------------------------------------------------------
// CONFIG
//-------------------------------------------------------------------

// #define LOG_SERIAL // write log output to serial port

//#define LED 13
//Pushbutton button(ZUMO_BUTTON); // pushbutton on pin 12

// Accelerometer Settings
#define RA_SIZE 3  // number of readings to include in running average of accelerometer readings
#define XY_ACCELERATION_THRESHOLD 2400  // for detection of contact (~16000 = magnitude of acceleration due to gravity)

// Reflectance Sensor Settings
#define NUM_SENSORS 6
// this might need to be tuned for different lighting conditions, surfaces, etc.
#define QTR_THRESHOLD  1500 // microseconds
//ZumoReflectanceSensorArray sensors(QTR_NO_EMITTER_PIN);

// Motor Settings
//ZumoMotors motors;

// these might need to be tuned for different motor types
#define REVERSE_SPEED     250 // 0 is stopped, 400 is full speed
#define TURN_SPEED        250
#define SEARCH_SPEED      250
#define SUSTAINED_SPEED   400 // switches to SUSTAINED_SPEED from FULL_SPEED after FULL_SPEED_DURATION_LIMIT ms
#define FULL_SPEED        400
#define STOP_DURATION     100 // ms
#define REVERSE_DURATION  200 // ms
#define TURN_DURATION     300 // ms

#define RIGHT 1
#define LEFT -1

enum ForwardSpeed { SearchSpeed, SustainedSpeed, FullSpeed };
#define FULL_SPEED_DURATION_LIMIT     250  // ms

// Sound Effects
//ZumoBuzzer buzzer;
const char sound_effect[] PROGMEM = "O4 T100 V15 L4 MS g12>c12>e12>G6>E12 ML>G2"; // "charge" melody
// use V0 to suppress sound effect; v15 for max volume

// Timing
#define MIN_DELAY_AFTER_TURN          400  // ms = min delay before detecting contact event
#define MIN_DELAY_BETWEEN_CONTACTS   1000  // ms = min delay between detecting new contact event



//-------------------------------------------------------------------
// FUNCTIONS
//-------------------------------------------------------------------

// RunningAverage class
// based on RunningAverage library for Arduino
// source:  http://playground.arduino.cc/Main/RunningAverage
template <typename T>
class RunningAverage
{
public:
	RunningAverage(void);
	RunningAverage(int);
	~RunningAverage();
	void clear();
	void addValue(T);
	T getAverage() const;
	void fillValue(T, int);
protected:
	int _size;
	int _cnt;
	int _idx;
	T _sum;
	T * _ar;
	static T zero;
};

// Accelerometer Class -- extends the LSM303 Library to support reading and averaging the x-y acceleration
//   vectors from the onboard LSM303DLHC accelerometer/magnetometer
class Accelerometer : public LSM303
{
	typedef struct acc_data_xy
	{
		unsigned long timestamp;
		int x;
		int y;
		float dir;
	} acc_data_xy;

public:
	Accelerometer() : ra_x(RA_SIZE), ra_y(RA_SIZE) {};
	~Accelerometer() {};
	void enable(void);
	void getLogHeader(void);
	void readAcceleration(unsigned long timestamp);
	float len_xy() const;
	float dir_xy() const;
	int x_avg(void) const;
	int y_avg(void) const;
	long ss_xy_avg(void) const;
	float dir_xy_avg(void) const;
private:
	acc_data_xy last;
	RunningAverage<int> ra_x;
	RunningAverage<int> ra_y;
};

class Sumo {
public:
	Sumo() : _initialized(false), _running(false), _whiteLines(WHITE_LINES), _inContact(false), _contactMadeTime(0),
	_lastTurnTime(0), _forwardSpeed(SearchSpeed), _fullSpeedStartTime(0) {
	};

	virtual ~Sumo() {};

	bool isInitialized() { return _initialized; }

	bool isRunning() { return _running; }

	void init();
	void start();
	void stop();

	int execute();

private:
	bool _initialized;
	bool _running;

	bool _whiteLines;

	ForwardSpeed _forwardSpeed;  // current forward speed setting
	unsigned long _fullSpeedStartTime;

	// Timing
	unsigned long _loopStartTime;
	unsigned long _lastTurnTime;
	unsigned long _contactMadeTime;

	Accelerometer* _lsm303;

	boolean _inContact;  // set when accelerometer detects contact with opposing robot

	unsigned int _sensorValues[NUM_SENSORS];

	void setForwardSpeed(ForwardSpeed speed);
	int getForwardSpeed();

	// check for contact, but ignore readings immediately after turning or losing contact
	bool checkForContact();

	// sound horn and accelerate on contact -- fight or flight
	void onContactMade();

	// reset forward speed
	void onContactLost();

	void reset() {
		_inContact = false;  // 1 if contact made; 0 if no contact or contact lost
		_contactMadeTime = 0;
		_lastTurnTime = millis();  // prevents false contact detection on initial acceleration
		_forwardSpeed = SearchSpeed;
		_fullSpeedStartTime = 0;
	}

	void turn(char direction, bool randomize);
};

extern Sumo sumo;
