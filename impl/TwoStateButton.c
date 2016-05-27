#include "TwoStateButton.h"

#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>

#include "../lib/definition/Definition.h"
#include "../lib/device/Actuator.h"
#include "../lib/device/actuator/ButtonActuator.h"


public void TwoStateButton_init(TwoStateButton * button, char * name, Pin pin, bool defaultState) {
	ButtonActuator_init(&button->actuator, name);

	button->pin = pin;
	button->defaultState = defaultState;
}

public void TwoStateButton_updateValue(TwoStateButton * button) {
	if (DIGITAL_READ(button->pin) != button->defaultState)
		button->actuator.state = !button->actuator.state;
}

#ifdef DEBUG
public void TwoStateButton_print(TwoStateButton * button) {
	printf(
		"TwoStateButton %s - Pin %d - State %d",
		button->actuator.actuator.name,
		(int) button->pin,
		(int) button->actuator.state
	);
}
#endif
