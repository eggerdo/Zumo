/**
* 456789------------------------------------------------------------------------------------------------------------120
*
* @brief:
* @file: MazeSolver.h
*
* @desc:
*
*
* @author:        Dominik Egger
* @date:        May 14, 2015
* @project:    Zumo
*
*/

#pragma once

//-------------------------------------------------------------------
// INCLUDES
//-------------------------------------------------------------------

//#include <QTRSensors.h>
//#include <ZumoReflectanceSensorArray.h>
//#include <ZumoBuzzer.h>

#include "Zumo.h"

//-------------------------------------------------------------------
// CONFIG
//-------------------------------------------------------------------

#define MAX_PATH_LENGTH 20

// ABOVE_LINE is a helper macro that takes returns
// 1 if the sensor is over the line and 0 if otherwise
#define ABOVE_LINE(sensor)((sensor) > SENSOR_THRESHOLD)

//-------------------------------------------------------------------
// FUNCTIONS
//-------------------------------------------------------------------

enum MazeSolverState {
	solving,
	solved,
//	waiting,
	repeating,
	finished,
	idle
};

//extern const char* mazeSolverStateStr[];

enum RepeatStep {
	rs_started,
	rs_follow,
	rs_turn,
	rs_finished
};

//extern const char* repeatStepStr[];

class MazeSolver {
public:
	MazeSolver() : _calibrated(false), _pathLength(0), _state(idle), _step(rs_started),
		_currentSegment(0), _running(false), _whiteLines(WHITE_LINES), _pathInvalid(false) {
//		memset(path, 0, sizeof(path));
	};

	virtual ~MazeSolver() {};

	bool isCalibrated() { return _calibrated; }

	void init();
	void start();
	void stop();

	int execute();

	void print() {
		memset(&_path[_pathLength], 0, MAX_PATH_LENGTH-_pathLength);
		LOGi("path: %s", _path);
	}

	void repeat();

	bool isWaiting() { return _state == solved; }

//	bool isMazeSolving() { return _state != idle; };

	bool isRunning() { return _running; }

private:
	bool _calibrated;

	bool _whiteLines;

	// path[] keeps a log of all the turns made
	// since starting the maze
	char _path[MAX_PATH_LENGTH];
	unsigned char _pathLength; // the length of the path
	bool _pathInvalid;

	MazeSolverState _state;

	int _currentSegment;

	RepeatStep _step;

	bool _running;

	void reset() {
		_pathLength = 0;
		_pathInvalid = false;
	}

	void switchState(MazeSolverState state) {
		//		LOGi("switched state from %s to: %s", mazeSolverStateStr[_state], mazeSolverStateStr[state]);
		LOGi("switched state from %d to: %d", _state, state);
		_state = state;
	}

	void switchStep(RepeatStep step) {
		//		LOGi("switched step from %s to: %s", mazeSolverStateStr[_step], mazeSolverStateStr[step]);
//		LOGi("switched step from %d to: %d", _step, step);
		_step = step;
	}

	// Turns according to the parameter dir, which should be
	// 'L' (left), 'R' (right), 'S' (straight), or 'B' (back).
	void turn(char dir);

	// This function decides which way to turn during the learning phase of
	// maze solving.  It uses the variables found_left, found_straight, and
	// found_right, which indicate whether there is an exit in each of the
	// three directions, applying the "left hand on the wall" strategy.
	char selectTurn(unsigned char found_left, unsigned char found_straight,
			unsigned char found_right);

	// The maze is broken down into segments. Once the Zumo decides
	// which segment to turn on, it will navigate until it finds another
	// intersection. followSegment() will then return after the
	// intersection is found.
	// @return true if an intersection was found, false otherwise
	bool followSegment();

	// The solveMaze() function works by applying a "left hand on the wall" strategy:
	// the robot follows a segment until it reaches an intersection, where it takes the
	// leftmost fork available to it. It records each turn it makes, and as long as the
	// maze has no loops, this strategy will eventually lead it to the finish. Afterwards,
	// the recorded path is simplified by removing dead ends. More information can be
	// found in the 3pi maze solving example.
	// @return true if the maze was solved, false otherwise
	bool solveMaze();

	// Now enter an infinite loop - we can re-run the maze as many
	// times as we want to.
	// @return true if the finish line was reached, false otherwise
	bool goToFinishLine();

	// simplifyPath analyzes the path[] array and reduces all the
	// turns. For example: Right turn + Right turn = (1) Back turn.
	void simplifyPath();


};

extern MazeSolver mazeSolver;
