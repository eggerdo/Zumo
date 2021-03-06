#pragma once

#include "Arduino.h"
#include "Zumo.h"

#define DEBUG                0
#define INFO                 1
#define WARN                 2
#define ERROR                3
#define FATAL                4
#define NONE                 5

//#define SERIAL_VERBOSITY NONE
#ifndef SERIAL_VERBOSITY
#define SERIAL_VERBOSITY NONE
#endif

#if SERIAL_VERBOSITY<NONE

#define log(level, fmt, ...) \
		timeStamp(); write(fmt "\r\n", ##__VA_ARGS__)
#define _log(level, fmt, ...) \
		   write(fmt, ##__VA_ARGS__)

#else

#define log(level, fmt, ...)
#define _log(level, fmt, ...)

#endif

#define LOGd(fmt, ...) log(DEBUG, fmt, ##__VA_ARGS__)
#define LOGi(fmt, ...) log(INFO, fmt, ##__VA_ARGS__)
#define LOGe(fmt, ...) log(ERROR, fmt, ##__VA_ARGS__)

#if SERIAL_VERBOSITY>DEBUG
#undef LOGd
#define LOGd(fmt, ...)
#endif

#if SERIAL_VERBOSITY>INFO
#undef LOGi
#define LOGi(fmt, ...)
#endif

#if SERIAL_VERBOSITY>WARN
#undef LOGw
#define LOGw(fmt, ...)
#endif
//
#if SERIAL_VERBOSITY>ERROR
#undef LOGe
#define LOGe(fmt, ...)
#endif

void initLogging(Stream *stream);
void setLogLevel(int level);
void write(const char* fmt, ... );
void timeStamp();

char* floatToString(double value);
