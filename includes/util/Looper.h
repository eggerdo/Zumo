/**
 * 456789------------------------------------------------------------------------------------------------------------120
 *
 * @brief:
 * @file: Looper.h
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

#pragma once

typedef int (*loopFunc_t)(void);

struct loopFunc {
	int id;
	// callback function to the loop that should be executed
	loopFunc_t cb;
	// time when the function should be executed
	unsigned long nextExecution;
	// pointer to the next entry
	loopFunc* nextLoopFunc;
};

class Looper {
public:
	static Looper& getInstance() {
		static Looper instance;
		return instance;
	}

	// loops through all registered function and executes them if the delay expired
	static void loop();

	// returns the start time of this loop iteration
	static unsigned long now();

	// register a function to be executed in each loop iteration. delay is the initial delay, for every
	// following iteration, the delay is returned by the function
	static void registerLoopFunc(loopFunc_t func, int delay = 0);

	void unregisterLoopFunc(loopFunc_t func);

private:
	Looper();

	void run();

	// time of the loop (so that each function can take the same time as now())
	unsigned long mTimeNow;

	// points to the first entry in the list
	loopFunc* first;
	// points to the last entry in the list
	loopFunc* last;

};

