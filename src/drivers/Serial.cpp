/*
 * Bluetooth.cpp
 *
 *  Created on: Nov 7, 2013
 *      Author: dominik
 */

#include "Zumo.h"
#include "Serial.h"
#include "messenger.h"
#include "protocol.h"
#include "util/Log.h"
#include "Actuator.h"
#include "Debug.h"
#include "Commander.h"

#ifdef BT_APP
	Messenger messenger(onControl, onDisconnect, onSensorRequest, onDrive, onCustom);
#endif

Stream* btSerialLine = NULL;

int lastDrive = 0;
#define DRIVE_TIMEOUT 1000

void onControl(boolean enabled);
void onDisconnect(aJsonObject* json);
void onSensorRequest(aJsonObject* json);
void onDrive(int left, int right);
void onCustom(aJsonObject* json);

void initSerial(Stream *stream) {
	btSerialLine = stream;
	setSerialLine(stream);

//	Looper::registerLoopFunc(receiveCommands);
}

void onControl(boolean enabled) {
	LOGd("onControl: %s", (enabled ? "true" : "false"));
}

void onDisconnect(aJsonObject* json) {
	LOGd("onDisconnect");
}

void onSensorRequest(aJsonObject* json) {
	LOGd("onSensorRequest");
//	sendSensorData();
}

void onDrive(int left, int right) {
	right = capSpeed(right);
	left = capSpeed(left);

	LOGd("handleDriveCommand (%d, %d)", left, right);
	drive(left, right);

	if (left != 0 && right != 0) {
		lastDrive = millis();
	}
}

void onCustom(aJsonObject* json) {
	switch(getType(json)) {
	}
}

int receiveCommands() {

#ifdef USB_SERIAL
	if (Serial.available()) {
		int incoming = Serial.read();
		handleInput(incoming);

		lastActivity = millis();
	}
#endif
#ifdef BT_SERIAL
	if (btSerialLine->available()) {
		int incoming = btSerialLine->read();
		handleInput(incoming);

		lastActivity = millis();
	}
#endif
#ifdef BT_APP
	if (messenger.handleMessages()) {
		lastActivity = millis();
	}

	// check for driving command timeouts. It's not really the correct place
	// to handle this in the bluetooth loop, but it's the easiest to do here,
	// otherwise a new loop has to be added in the Actuator.cpp handling the
	// drive timeout.
	// if no drive command is received within the DRIVE_TIMEOUT then stop
	// driving.
	// lastDrive is the time when the last drive command (not stop command)
	// was received
	if (lastDrive && (millis() > lastDrive + DRIVE_TIMEOUT)) {
		drive_stop();
		lastDrive = 0;
	}
#endif

	return 0; // no delay, call as much as possible
}
