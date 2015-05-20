/*
 * Bluetooth.h
 *
 *  Created on: Nov 7, 2013
 *      Author: dominik
 */

#ifndef BLUETOOTH_H_
#define BLUETOOTH_H_

#include "aJSON.h"

#define RxD 9
#define TxD 8

#define HOMING USER+1
#define SHOOT_GUNS USER+2
#define FIRE_VOLLEY USER+3
#define HIT_DETECTED USER+4

void initSerial(Stream *stream);

int receiveCommands();

#endif /* BLUETOOTH_H_ */
