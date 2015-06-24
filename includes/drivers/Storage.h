/**
 * 456789------------------------------------------------------------------------------------------------------------120
 *
 * @brief:
 * @file: Storage.h
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
 * @date:        Jun 10, 2015
 * @project:    Zumo
 * @company:     Distributed Organisms B.V.
 */

#pragma once

//-------------------------------------------------------------------
// INCLUDES
//-------------------------------------------------------------------

#include <EEPROM.h>

//-------------------------------------------------------------------
// CONFIG
//-------------------------------------------------------------------

enum Program {
	Prog_MazeSolver,
	Prog_LineFollower,
	Prog_Sumo
};

#define PROGRAM_ADDRESS 0
#define WHITE_LINES_ADDRESS 1

//-------------------------------------------------------------------
// FUNCTIONS
//-------------------------------------------------------------------

class Storage {

private:

public:
	// helper function to reduce unnecessary writes
	// only write if new value is not equal to stored value
	static void update(int address, uint8_t value) {
		if (EEPROM.read(address) != value) {
			EEPROM.write(address, value);
		}
	}

	static void setCurrentProgram(Program prog) {
		update(PROGRAM_ADDRESS, (uint8_t)prog);
	}

	static Program getCurrentProgram() {
		return (Program) EEPROM.read(PROGRAM_ADDRESS);
	}

	static void setWhiteLines(bool val) {
		update(WHITE_LINES_ADDRESS, (uint8_t)val);
	}

	static bool getWhiteLines() {
		return (bool) EEPROM.read(WHITE_LINES_ADDRESS);
	}
};

