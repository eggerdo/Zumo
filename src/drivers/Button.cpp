/**
 * 456789------------------------------------------------------------------------------------------------------------120
 *
 * @brief:
 * @file: Buton.cpp
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
 * @date:        Mar 21, 2015
 * @project:    Zumo
 * @company:     Distributed Organisms B.V.
 */

#include "Button.h"

Pushbutton* _button;

void setupButton(int button) {
	_button = new Pushbutton(button);
	pinMode(LED, OUTPUT);
}

bool lightOn = false;

bool checkForButton() {
//	LOGi("test");
//	  do
//	  {
//		  _button->waitForPress();
////	    while (!_button->isPressed());  // wait for button to be pressed
//	    delay(10);                    // debounce the button press
//		LOGi("isPressed");
//	  }
//	  while (!_button->isPressed());    // if button isn't still pressed, loop

	bool buttonPressed = _button->getSingleDebouncedPress();
	if (buttonPressed)
	{
		LOGi("button pressed");
		if (!lightOn) {
			digitalWrite(LED, HIGH);
		} else {
			digitalWrite(LED, LOW);
		}
		lightOn = !lightOn;
	}

	return buttonPressed;
}
