#include "MomentaryButton.h"

#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>

#include "../lib/definition/Definition.h"
#include "../lib/device/Actuator.h"
#include "../lib/device/actuator/ButtonActuator.h"


public void MomentaryButton_init(MomentaryButton * button, char * name, Pin pin) {
	ButtonActuator_init(&button->actuator, name);

	button->pin = pin;
}

public void MomentaryButton_updateValue(MomentaryButton * button) {
	button->actuator.state = DIGITAL_READ(button->pin);
}

#ifdef DEBUG
public void MomentaryButton_print(MomentaryButton * button) {
	printf(
		"Button %s - Pin %d - State %d",
		button->actuator.actuator.name,
		(int) button->pin,
		(int) button->actuator.state
	);
}
#endif
