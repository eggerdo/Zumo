// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section

#ifndef _Zumo_H_
#define _Zumo_H_

#include "Arduino.h"
//add your includes for the project Zumo here

#include "Pinout.h"
#include "util/Log.h"
#include "util/Looper.h"
#include "Debug.h"
#include "cfg/config.h"

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

void setupLoopers();


//Do not add code below this line
#endif /* _Zumo_H_ */
