// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section

#ifndef _Zumo_H_
#define _Zumo_H_

#include <cfg/Config.h>
#include "Arduino.h"
//add your includes for the project Zumo here

#include "util/Log.h"
#include "util/Looper.h"
#include "cfg/Pinout.h"
#include "drivers/Storage.h"

//end of add your includes here
#ifdef __cplusplus
extern "C" {
#endif
void loop();
void setup();
#ifdef __cplusplus
} // extern "C"
#endif

//add your function definitions for the project Zumo here

extern long lastActivity;

extern Stream* btStream;

extern Program _currentProgram;
extern bool _whiteLines;

void setupLoopers();


//Do not add code below this line
#endif /* _Zumo_H_ */
