/**
 * 456789------------------------------------------------------------------------------------------------------------120
 *
 * @brief:
 * @file: BluetoothShield.h
 *
 * @desc:
 *
 *
 *
 * @author:        Dominik Egger
 * @date:        Dec 6, 2014
 * @project:    Zumo
 *
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
