// Do not remove the include below

//#define MAZESOLVER

#include "Zumo.h"

#include "drivers/Actuator.h"
#include "drivers/Serial.h"
#include "drivers/BluetoothShield.h"
#include "drivers/Button.h"
//#include "drivers/Compass.h"
#include "drivers/Sensors.h"

#ifdef LINEFOLLOWER
#include "behaviours/LineFollower.h"
#endif

#ifdef MAZESOLVER
#include "behaviours/MazeSolver.h"
#endif

#ifdef SUMO
#include "behaviours/Sumo.h"
#endif

Stream* btStream;

//The setup function is called once at startup of the sketch
void setup()
{
	Serial.begin(115200); // USB
	btStream = setupBluetoothShield(BT_RX, BT_TX);

#ifdef BT_SERIAL
	initLogging(btStream);
#else
	initLogging(&Serial);
#endif

	setBluetoothSerial(btStream);

	initButton(ZUMO_BUTTON);
//	initMotors();
	initReflectantSensors();

	drive(0, 0);

	setupLoopers();

	LOGi("initialisation done ...");
}

void setupLoopers() {
	Looper::registerLoopFunc(receiveCommands);
}

// The loop function is called in an endless loop
void loop()
{
	Looper::loop();

	if (checkForButton()) {
#ifdef LINEFOLLOWER
		if (!lineFollower.isCalibrated()) {
			lineFollower.init();
		} else {
			if (!lineFollowing) {
				lineFollower.start();
			} else {
				lineFollower.stop();
			}
		}
#endif

#ifdef MAZESOLVER
		if (!mazeSolver.isCalibrated()) {
			mazeSolver.init();
		} else {
			if (!mazeSolver.isMazeSolving()) {
				mazeSolver.start();
			} else if (mazeSolver.isWaiting()) {
				mazeSolver.repeat();
			} else {
				mazeSolver.stop();
			}
		}
#endif

#ifdef SUMO
//		if (!sumo.isInitialized()) {
//			sumo.init();
//		} else {
			if (!sumo.isRunning()) {
				sumo.start();
			} else {
				sumo.stop();
			}
//		}
#endif

	}
}

