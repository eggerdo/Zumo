/**
 * 456789------------------------------------------------------------------------------------------------------------120
 *
 * @brief:
 * @file: BluetoothShield.cpp
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

#include "BluetoothShield.h"
#include "Zumo.h"

SoftwareSerial* blueToothSerial;

SoftwareSerial* setupBluetoothShield(int txPin, int rxPin)
{
    pinMode(rxPin, INPUT);
    pinMode(txPin, OUTPUT);
    blueToothSerial = new SoftwareSerial(rxPin, txPin);
	blueToothSerial->begin(115200);
//    setupBlueToothConnection();

//    blueToothSerial->print("\r\nbt ready\r\n");

    return blueToothSerial;
}

void bt_loop() {

	if (blueToothSerial->available()) {
		int incoming = blueToothSerial->read();
		Serial.print((char)incoming);
		Serial.print(' ');
	}
	if (Serial.available()) {
		char incoming = Serial.read();
		Serial.print(incoming);
	}
}

void receive()
{
    char recvChar;
    if (blueToothSerial->available()) {
		while(blueToothSerial->available())
		{
			recvChar = blueToothSerial->read();
			Serial.print(recvChar);

	//            if(recvChar == 't' || recvChar == 'T')
	//            {
	//                blueToothSerial.print("temperature: ");
	//                blueToothSerial.println(getTemp());
	//            }
	//        if(Serial.available())
	//        {//check if there's any data sent from the local serial terminal, you can add the other applications here
	//            recvChar  = Serial.read();
	//            blueToothSerial.print(recvChar);
	//        }
		}
//		Serial.println(' ');
    }
    if(Serial.available())
	{//check if there's any data sent from the local serial terminal, you can add the other applications here
		recvChar  = Serial.read();
		Serial.print(recvChar);
		blueToothSerial->print(recvChar);
	}

}

void rec()
{
	while(!blueToothSerial->available()) {};

    char recvChar;
    while(blueToothSerial->available())
    {
		recvChar = blueToothSerial->read();
		Serial.print(recvChar);

//            if(recvChar == 't' || recvChar == 'T')
//            {
//                blueToothSerial.print("temperature: ");
//                blueToothSerial.println(getTemp());
//            }
//        if(Serial.available())
//        {//check if there's any data sent from the local serial terminal, you can add the other applications here
//            recvChar  = Serial.read();
//            blueToothSerial.print(recvChar);
//        }
    }
    Serial.println(' ');

}
//
//void write(char c) {
//	blueToothSerial.print(c);
//}


///***************************************************************************
// * Function Name: setupBlueToothConnection
// * Description:  initializing bluetooth connction
// * Parameters:
// * Return:
// *
// * !!! NOTE: settings only change if bt is NOT!!!! connected
// *
//***************************************************************************/
//void setupBlueToothConnection()
//{
//	blueToothSerial->begin(19200);
//
//	Serial.println("\r\nsetupBlueToothConnection");
//
//	Serial.println("AT");
//	blueToothSerial->print("AT");
//	delay(400);
//	rec();
//	delay(400);
//
//	Serial.println("AT+BAUD?");
//	blueToothSerial->print("AT+BAUD?");
//	delay(400);
//	rec();
//	delay(400);
////
////	Serial.println('3');
////	blueToothSerial->print("AT+DEFAULT");             // Restore all setup value to factory setup
////	rec();
////	delay(2000);
//
////	blueToothSerial->print("AT+TYPE?");
////	rec();
////	delay(400);
//
////	Serial.println('4');
////	blueToothSerial.print("AT+NAMESeeedBTSlave");    // set the bluetooth name as "SeeedBTSlave" ,the length of bluetooth name must less than 12 characters.
////	rec();
////	delay(400);
//
////	Serial.println('5');
////    blueToothSerial.print("AT+PIN0000");             // set the pair code to connect
////	rec();
////	delay(400);
//
////	Serial.println("51");
////	blueToothSerial->print("AT+TYPE1");
////	rec();
////	delay(400);
//
////	Serial.println('6');
////	blueToothSerial.print("AT+AUTH1");             //
////	rec();
////    delay(400);
//
////	Serial.println('2');
////	blueToothSerial->print("AT+BAUD5");
////	rec();
////	delay(400);
//
////	Serial.println('7');
////	blueToothSerial.print("AT+BAUD?");
////	rec();
////	delay(400);
////
////	Serial.println("71");
////	blueToothSerial.print("AT+SECH?");
////	rec();
////	delay(400);
////
//	Serial.println("AT+TYPE?");
//	blueToothSerial->print("AT+TYPE?");
//	rec();
//	delay(400);
////
////	Serial.println('8');
//    blueToothSerial->flush();
//
////    blueToothSerial->end();
////	blueToothSerial->begin(19200);
//}

void setupBlueToothConnection()
{

    blueToothSerial->begin(115200);

	blueToothSerial->print("AT");
	delay(400);

	blueToothSerial->print("AT+DEFAULT");             // Restore all setup value to factory setup
	delay(2000);

	blueToothSerial->print("AT+NAMEZumo");    // set the bluetooth name as "SeeedMaster" ,the length of bluetooth name must less than 12 characters.
	delay(400);

	blueToothSerial->print("AT+AUTH0");
    delay(400);

	blueToothSerial->print("AT+BAUD8");
    delay(400);

    blueToothSerial->flush();
    blueToothSerial->end();
    blueToothSerial->begin(115200);


}

