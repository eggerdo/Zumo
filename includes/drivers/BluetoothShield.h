/**
 * 456789------------------------------------------------------------------------------------------------------------120
 *
 * @brief:
 * @file: BluetoothShield.h
 *
 * @desc: 
 *
 * This file is created at Almende B.V. and Distributed Organisms B.V. It is open-source software and belongs to a
 * larger suite of software that is meant for research on self-organization principles and multi-agent systems where
 * learning algorithms are an important aspect.
 *
 * This software is published under the GNU Lesser General Public license (LGPL).
 *
 * It is not possible to add usage restrictions to an open-source license. Nevertheless, we personally strongly object
 * against this software being used for military purposes, factory farming, animal experimentation, and "Universal
 * Declaration of Human Rights" violations.
 *
 * Copyright (c) 2013 Dominik Egger <dominik@dobots.nl>
 *
 * @author:        Dominik Egger
 * @date:        Dec 6, 2014
 * @project:    Zumo
 * @company:     Distributed Organisms B.V.
 */

#ifndef BLUETOOTHSHIELD_H_
#define BLUETOOTHSHIELD_H_

class BluetoothShield {
};

#include <SoftwareSerial.h>   //Software Serial Port

SoftwareSerial* setupBluetoothShield(int txPin, int rxPin);
void setupBlueToothConnection(SoftwareSerial* bluetoothSerial);

//void receive();
void bt_loop();

#endif /* BLUETOOTHSHIELD_H_ */
