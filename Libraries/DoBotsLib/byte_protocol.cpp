#include "byte_protocol.h"

#include "util/Log.h"

Stream *serial_stream;

//#ifdef USB_SERIAL

#define HEADER_ID 0x02FF

//	aJsonStream *debugStream;
//#endif

#ifdef HEADER_BIG
int mTransactionId = 0;
#endif

// set the serial line used for communication
void setSerialLine(Stream *stream) {
	serial_stream = stream;
}

// get the message type. see header for the available types
int getType(message_t* msg) {
	return msg->messageType;
}

// create a drive command message with left and right speed setpoints
// the speed values can be in the range [-255, 255], where negative
// values corresponds to backward motion
//aJsonObject* createDriveCommand(int left, int right) {
//
//}

// decode a drive command to get the left and right speed setpoints
void decodeDriveCommand(message_t* msg, int* left, int* right) {
	drive_payload* payload = (drive_payload*)msg->payload;
	*left = payload->left;
	*right = payload->right;
}

void decodeControlCommand(message_t* msg, boolean* enabled) {
	*enabled = msg->payload[0];
}

// send a message
//void sendMessage(aJsonObject* json) {
//  aJson.print(json, jsonStream);
//  serial_stream->write("\n");
//  aJson.deleteItem(json);
//}

void printArray(byte* arr, uint16_t len) {
	for (int i = 0; i < len; ++i) {
		_log(DEBUG, " %02X", arr[i]);
		if ((i+1) % 30 == 0) {
			_log(DEBUG, "\r\n");
		}
	}
	_log(DEBUG, "\r\n");
}

byte buffer[32] = {};

// listen for incoming messages and return the message
// NULL is returned if nothing is available or if the message is
// not a valid json string
message_t* readMessage() {

	byte* ptr = buffer;
	if (serial_stream->available()) {
		size_t length = 0;

		while (serial_stream->available()) {
			int c = serial_stream->read();
			if (c < 0) break;
			*ptr++ = c;
			length++;
			if (length > 32) {
				break;
			}
		}

		LOGd("received: ");
		printArray(buffer, length);

		message_t* msg = (message_t*)buffer;
		if (msg->headerId == HEADER_ID) {
			LOGi("received");
			return msg;
		}
	}

	return NULL;
}
