#include "ButtonActuator.h"

#include <stdio.h>
#include <stdbool.h>

#include "../../definition/Definition.h"

private ErrorIndex ButtonActuator_assign(Object * self, RingBuffer * buffer);

private size_t ButtonActuator_getTypeSize();
private void * ButtonActuator_getValue(Object * self);

#ifdef DEBUG
private void ButtonActuator_print(Object * self);
#endif

public void ButtonActuator_init(ButtonActuator * actuator, char * name) {
	Actuator_init(&actuator->actuator, name, ACTUATOR_TYPE_BUTTON);

	actuator->actuator.assign = &ButtonActuator_assign;
	actuator->actuator.getTypeSize = &ButtonActuator_getTypeSize;
	actuator->actuator.getValue = &ButtonActuator_getValue;

	actuator->state = false;

#ifdef DEBUG
	actuator->actuator.print = &ButtonActuator_print;
#endif
}

private void * ButtonActuator_getValue(Object * self) {
	ButtonActuator * button = (ButtonActuator *) self;

	return &button->state;
}

private size_t ButtonActuator_getTypeSize() {
	return sizeof(bool);
}


private ErrorIndex ButtonActuator_assign(Object * self, RingBuffer * buffer) {
	ButtonActuator * actuator = (ButtonActuator *) self;

	return RingBuffer_read(buffer, (byte *) &actuator->state);
}


#ifdef DEBUG
private void ButtonActuator_print(Object * self) {
	ButtonActuator * button = (ButtonActuator *) self;

	printf(
		"(B) %s - State %d",
		button->actuator.name,
		(int) button->state
	);
}
#endif
