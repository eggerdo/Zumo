/*
 * Bluetooth.cpp
 *
 *  Created on: Nov 7, 2013
 *      Author: dominik
 */

#include "Serial.h"
#include "messenger.h"
#include "byte_protocol.h"
//#include "protocol.h"
//#include "util/Log.h"
#include "Actuator.h"
#include "Commander.h"

#ifdef BT_APP
	void onControl(boolean enabled);
	void onDisconnect(message_t* json);
	void onSensorRequest(message_t* json);
	void onDrive(int left, int right);

	Messenger messenger(onControl, onDisconnect, onSensorRequest, onDrive, onCustom);
#endif

Stream* btSerialLine = NULL;

long lastActivity = 0;
long lastDrive = 0;

void setBluetoothSerial(Stream *stream) {
	btSerialLine = stream;
	setSerialLine(stream);
}

void onControl(boolean enabled) {
	LOGd("onControl: %s", (enabled ? "true" : "false"));
}

void onDisconnect(message_t* msg) {
	LOGd("onDisconnect");
}

void onSensorRequest(message_t* msg) {
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
//	if (btSerialLine->available()) {
//		while (btSerialLine->available()) {
//			int incoming = btSerialLine->read();
//			Serial.write(incoming);
//		}
////		Serial.println(":");
//	} else {
////		Serial.println("-");
//	}
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
