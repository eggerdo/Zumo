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
#include <programs/LineFollower.h>
#endif

#ifdef MAZESOLVER
#include <programs/MazeSolver.h>
#endif

#ifdef SUMO
#include <programs/Sumo.h>
#endif

Stream* btStream;

Program _currentProgram;
bool _whiteLines;

//The setup function is called once at startup of the sketch
void setup()
{
	Serial.begin(115200); // USB
	btStream = setupBluetoothShield(BT_RX, BT_TX);

#ifdef BT_SERIAL
	initLogging(btStream);
#else
#ifdef USB_SERIAL
	initLogging(&Serial);
#endif
#endif

	setBluetoothSerial(btStream);

	initButton(ZUMO_BUTTON);
//	initMotors();
	initReflectantSensors();

	drive(0, 0);

	setupLoopers();

	_whiteLines = Storage::getWhiteLines();
	LOGi("use white lines: %s", _whiteLines ? "true" : "false");

	_currentProgram = Storage::getCurrentProgram();

	switch (_currentProgram) {
	case Prog_MazeSolver:
		LOGi("Program MazeSolver selected");
		break;
	case Prog_LineFollower:
		LOGi("Program LineFollower selected");
		break;
	case Prog_Sumo:
		LOGi("Program Sumo selected");
		break;
	default:
		LOGi("unknown program");
		_currentProgram = Prog_MazeSolver;
		Storage::setCurrentProgram(_currentProgram);
		break;
	}

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

		switch(_currentProgram) {
#ifdef MAZESOLVER
		case Prog_MazeSolver: {
			if (!mazeSolver.isCalibrated()) {
				mazeSolver.init();
			} else {
				if (!mazeSolver.isRunning()) {
					mazeSolver.start();
				} else if (mazeSolver.isWaiting()) {
					mazeSolver.repeat();
				} else {
					mazeSolver.stop();
				}
			}
			break;
		}
		case Prog_LineFollower: {
			if (!mazeSolver.isCalibrated()) {
				mazeSolver.init();
			} else {
				if (!mazeSolver.isRunning()) {
					mazeSolver.start();
				} else {
					mazeSolver.stop();
				}
			}
			break;
		}
#endif
#ifdef LINEFOLLOWER
		case Prog_LineFollower: {
			if (!lineFollower.isCalibrated()) {
				lineFollower.init();
			} else {
				if (!lineFollowing) {
					lineFollower.start();
				} else {
					lineFollower.stop();
				}
			}
			break;
		}
#endif
#ifdef SUMO
		case Prog_Sumo: {
//		if (!sumo.isInitialized()) {
//			sumo.init();
//		} else {
			if (!sumo.isRunning()) {
				sumo.start();
			} else {
				sumo.stop();
			}
//		}
			break;
		}
#endif
		default:
			break;
		}

	}
}

