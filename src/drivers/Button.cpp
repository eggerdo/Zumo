/**
 * 456789------------------------------------------------------------------------------------------------------------120
 *
 * @brief:
 * @file: Buton.cpp
 *
 *
 * @author:        Dominik Egger
 * @date:        Mar 21, 2015
 * @project:    Zumo
 *
 */

#include "Button.h"

Pushbutton* _button;

void initButton(int button) {
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
