#ifndef MESSENGER_H
#define MESSENGER_H

#include "protocol.h"

typedef void (*handleCommand_func)(aJsonObject* json);
typedef void (*handleControl_func)(boolean enabled);
typedef void (*handleDrive_func)(int left, int right);
typedef void (*handleCustom_func)(aJsonObject* json);

class Messenger {

public:
	Messenger(handleControl_func onControl_cb, handleCommand_func onDisconnect_cb,
			  handleCommand_func onSensorRequest_cb, handleDrive_func onDrive_cb, handleCustom_func onCustom_cb = NULL);

	handleControl_func onControl;
	handleCommand_func onDisconnect;
	handleCommand_func onSensorRequest;
	handleDrive_func onDrive;
	handleCustom_func onCustom;

	boolean handleMessages();
};

#endif MESSENGER_H
