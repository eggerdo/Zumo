/*
 * Bluetooth.h
 *
 *  Created on: Nov 7, 2013
 *      Author: dominik
 */

#ifndef BLUETOOTH_H_
#define BLUETOOTH_H_

#include "Zumo.h"
#include "byte_protocol.h"

void setBluetoothSerial(Stream *stream);

int receiveCommands();

void onCustom(message_t* msg);

#endif /* BLUETOOTH_H_ */
