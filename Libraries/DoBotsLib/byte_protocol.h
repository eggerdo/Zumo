#ifndef PROTOCOL_H
#define PROTOCOL_H

#include "aJSON.h"

struct drive_payload {
	int16_t left;
	int16_t right;
};

struct message_t {
	uint16_t headerId;
	uint8_t messageType;
	uint16_t length;
	uint8_t payload[1];
};

// message types
#define CONTROL_COMMAND 0
#define DISCONNECT 		1
#define DRIVE_COMMAND 	2
#define SENSOR_REQUEST  3
#define SENSOR_DATA 	4

#define USER			20

// PUBLIC FUNCTIONS

void setSerialLine(Stream *stream);

int getType(message_t* msg);

//aJsonObject* createDriveCommand(int left, int right);
void decodeDriveCommand(message_t* msg, int* left, int* right);
void decodeControlCommand(message_t* msg, boolean* enabled);

// sendMessage also deletes the json object after it is sent
void sendMessage(message_t* msg);
message_t* readMessage();

#endif PROTOCOL_H
