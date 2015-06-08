/**
 * 456789------------------------------------------------------------------------------------------------------------120
 *
 * @brief:
 * @file: ExecutionHandler.cpp
 *
 * @desc:
 *
 *
 *
 * @author:        Dominik Egger
 * @date:        Aug 22, 2014
 * @project:    Zumo
 *
 */

#include "Arduino.h"

#include "util/Looper.h"
#include "util/Log.h"

Looper::Looper() :
		mTimeNow(0),
		last(NULL),
		first(NULL)
{
	LOGi("looper create");
}

void Looper::loop() {
	getInstance().mTimeNow = millis();
	getInstance().run();
}

void Looper::run() {

	// loop through all registered functions
	loopFunc* loop = first;
	while (loop != NULL) {
		// if next execution time
		if (loop->nextExecution < mTimeNow) {
			// call function, which returns the delay
			unsigned long delay = loop->cb();
			// update the next execution time
			loop->nextExecution = mTimeNow + delay;
		}
		loop = loop->nextLoopFunc;
	}
}

unsigned long Looper::now() {
	return getInstance().mTimeNow;
}

int idcnt = 0;
void Looper::registerLoopFunc(loopFunc_t func, int delay) {

	loopFunc* loop = new loopFunc();
	loop->id = idcnt++;
	loop->cb = func;
	loop->nextExecution = Looper::now();
	loop->nextLoopFunc = NULL;

	// if it's the first entry, set the first pointer to this entry
	if (getInstance().first == NULL) {
		getInstance().first = loop;
	}

	// if there was already an entry, update the next pointer of that
	// entry to this entry
	if (getInstance().last != NULL) {
		getInstance().last->nextLoopFunc = loop;
	}

	// update the last pointer to this entry
	getInstance().last = loop;

//	LOGi("register id: %d", loop->id);
}

void Looper::unregisterLoopFunc(loopFunc_t func) {
	if (first->cb == func) {
		loopFunc* tmp = first->nextLoopFunc;
		delete first;
		first = tmp->nextLoopFunc;
	} else {

		loopFunc* loop = first;
		while (loop != NULL) {

//			LOGi("loop->nextLoopFunc->cb: %p", loop->nextLoopFunc->cb);
//			LOGi("func: %p", func);
			if (loop->nextLoopFunc->cb == func) {
//				LOGi("unregister id: %d", loop->nextLoopFunc->id);

				loopFunc* tmp = loop->nextLoopFunc;
				loop->nextLoopFunc = loop->nextLoopFunc->nextLoopFunc;

				if (last == tmp) {
					last = loop;
				}
				delete tmp;
			}

			loop = loop->nextLoopFunc;
		}
	}
}
