/**
 * 456789------------------------------------------------------------------------------------------------------------120
 *
 * @brief:
 * @file: BluetoothShield.cpp
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

#include "BluetoothShield.h"
#include "Zumo.h"

SoftwareSerial* setupBluetoothShield(int txPin, int rxPin)
{
    pinMode(rxPin, INPUT);
    pinMode(txPin, OUTPUT);
    SoftwareSerial* bluetoothSerial = new SoftwareSerial(rxPin, txPin);
	bluetoothSerial->begin(38400);
//    setupBlueToothConnection(bluetoothSerial);

    return bluetoothSerial;
}

/***************************************************************************
 * Function Name: setupBlueToothConnection
 * Description:  initializing bluetooth connction
 * Parameters:
 * Return:
 *
 * !!! NOTE: settings only change if bt is NOT!!!! connected
 *
***************************************************************************/
void setupBlueToothConnection(SoftwareSerial* bluetoothSerial)
{
//    bluetoothSerial->begin(9600);

	bluetoothSerial->print("AT");
	delay(400);

	bluetoothSerial->print("AT+DEFAULT");             // Restore all setup value to factory setup
	delay(2000);

	bluetoothSerial->flush();
	bluetoothSerial->end();
	bluetoothSerial->begin(9600);

	bluetoothSerial->print("AT+AUTH0");
    delay(400);

	bluetoothSerial->print("AT+BAUD6");
    delay(400);

	bluetoothSerial->print("AT+NAMEZumo");    // the length of bluetooth name must less than 12 characters.
	delay(400);

    bluetoothSerial->flush();
    bluetoothSerial->end();
    bluetoothSerial->begin(38400);
}

/*********************************************
 * Baud Rates:
 *   AT+BAUD1 -> 1200
 *          2 -> 2400
 *          3 -> 4800
 *          4 -> 9600
 *          5 -> 19200
 *          6 -> 38400
 *          7 -> 57600
 *          8 -> 115200
 *          9 -> 230400
 *          A -> 460800
 *          B -> 921600
 *          C -> 1382400
 */

