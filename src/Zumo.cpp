// Do not remove the include below

#define MAZESOLVER

#include "Zumo.h"

#include "drivers/Actuator.h"
#include "drivers/Serial.h"
#include "drivers/BluetoothShield.h"
#include "drivers/Button.h"
#include "drivers/Compass.h"
#include "drivers/Sensors.h"

#ifdef LINEFOLLOWER
#include "behaviours/LineFollower.h"
#endif

#ifdef MAZESOLVER
#include "behaviours/MazeSolver.h"
#endif

long lastActivity = 0;

//The setup function is called once at startup of the sketch
void setup()
{
	Serial.begin(115200); // USB
	Stream* btStream = setupBluetoothShield(BT_RX, BT_TX);

#ifdef BT_SERIAL
	initLogging(btStream);
#else
	initLogging(&Serial);
#endif

	initSerial(btStream);
	setupButton(ZUMO_BUTTON);

	initReflectantSensors();

	drive(0, 0);

	setupLoopers();

	LOGi("initialisation done ...");
}

void setupLoopers() {
	Looper::registerLoopFunc(receiveCommands);
}

bool lineFollowing = false;
bool mazeSolving = false;

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
				lineFollowing = true;
			} else {
				lineFollower.stop();
				lineFollowing = false;
			}
		}
#endif

#ifdef MAZESOLVER
		if (!mazeSolver.isCalibrated()) {
			mazeSolver.init();
		} else {
			if (!mazeSolver.isMazeSolving()) {
				mazeSolver.start();
				mazeSolving = true;
			} else if (mazeSolver.isWaiting()) {
				mazeSolver.repeat();
			} else {
				mazeSolver.stop();
				mazeSolving = false;
			}
		}
#endif
	}
}

