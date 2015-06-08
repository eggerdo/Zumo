#include "messenger.h"
#include "byte_protocol.h"

Messenger::Messenger(handleControl_func onControl_cb, handleCommand_func onDisconnect_cb,
		  handleCommand_func onSensorRequest_cb, handleDrive_func onDrive_cb, handleCustom_func onCustom_cb) :
	onControl(onControl_cb),
	onDisconnect(onDisconnect_cb),
	onSensorRequest(onSensorRequest_cb),
	onDrive(onDrive_cb),
	onCustom(onCustom_cb) {
}

boolean Messenger::handleMessages() {
	message_t* msg;
	int left = 0, right = 0;
	boolean enabled = false;

	msg = readMessage();
	if (msg == NULL) {
		return false;
	}

	switch(getType(msg)) {
		case DRIVE_COMMAND:
			if (onDrive != NULL) {
				decodeDriveCommand(msg, &left, &right);
				onDrive(left, right);
			}
			break;
		case CONTROL_COMMAND:
			if (onControl != NULL) {
				decodeControlCommand(msg, &enabled);
				onControl(enabled);
			}
			break;
		case DISCONNECT:
			if (onDisconnect != NULL) {
				onDisconnect(msg);
			}
			break;
		case SENSOR_REQUEST:
			if (onSensorRequest != NULL) {
				onSensorRequest(msg);
			}
			break;
		default:
			if (onCustom != NULL) {
				onCustom(msg);
			}
			break;
	}

	return true;
}
