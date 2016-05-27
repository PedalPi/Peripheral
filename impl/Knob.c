#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>

#include "../lib/definition/Definition.h"
#include "../lib/device/Actuator.h"
#include "../lib/device/actuator/RangeActuator.h"

#include "Knob.h"

public void Knob_init(Knob * knob, char * name, Pin pin) {
	RangeActuator_init(&knob->actuator, name);

	knob->pin = pin;
}

public void Knob_updateValue(Knob * knob) {
	knob->actuator.current = (knob->actuator.max * ANALOGIC_READ(knob->pin))/1023;
}


#ifdef DEBUG
public void Knob_print(Knob * knob) {
	printf(
		"Knob: %s (%f [%f..%f])",
		knob->actuator.actuator.name,
		knob->actuator.current,
		knob->actuator.min,
		knob->actuator.max
	);
}
#endif
