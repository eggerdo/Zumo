/**
* 456789------------------------------------------------------------------------------------------------------------120
*
* @brief:
* @file: Pinout.h
*
* @desc:
*
*
* @author:        Dominik Egger
* @date:        Dec 7, 2014
* @project:    Zumo
*
*/

#ifndef PINOUT_H_
#define PINOUT_H_

//-----------------------------------------------
// ZUMO SHIELD
//-----------------------------------------------

#define RX 0
#define TX 1

#define BUZZER 3

#define BT_RX 6
#define BT_TX 2

#define DIR_R 7
#define DIR_L 8
#define PWM_R 9
#define PWM_L 10

#define PUSH_BTN 12
#define LED 13

#define BATTERY A1
#define SDA A4
#define SCL A5

//-----------------------------------------------
// ZUMO REFLECTIVE SENSOR
//-----------------------------------------------

#define LEDON 2

#define IRSENSOR_LLL  4 // total left
#define IRSENSOR_LL   A3
#define IRSENSOR_L    11
#define IRSENSOR_R    A0
#define IRSENSOR_RR   A2
#define IRSENSOR_RRR  5 // total right


#endif /* PINOUT_H_ */
