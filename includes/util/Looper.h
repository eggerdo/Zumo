/**
 * 456789------------------------------------------------------------------------------------------------------------120
 *
 * @brief:
 * @file: ExecutionHandler.h
 *
 * @desc: 
 *
 * This file is created at Almende B.V. and Distributed Organisms B.V. It is open-source software and belongs to a
 * larger suite of software that is meant for research on self-organization principles and multi-agent systems where
 * learning algorithms are an important aspect.
 *
 * This software is published under the GNU Lesser General Public license (LGPL).
 *
 * It is not possible to add usage restrictions to an open-source license. Nevertheless, we personally strongly object
 * against this software being used for military purposes, factory farming, animal experimentation, and "Universal
 * Declaration of Human Rights" violations.
 *
 * Copyright (c) 2013 Dominik Egger <dominik@dobots.nl>
 *
 * @author:        Dominik Egger
 * @date:        Aug 22, 2014
 * @project:    PirateDotty
 * @company:     Distributed Organisms B.V.
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
	static Looper* getInstance();

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

