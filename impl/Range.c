#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>

#include "../lib/definition/Definition.h"
#include "../lib/device/Actuator.h"
#include "../lib/device/actuator/RangeActuator.h"

#include "Range.h"

public void Range_init(Range * range, char * name) {
	RangeActuator_init(&range->actuator, name);
}

public void Range_updateValue(Range * range, float value) {
	if (range->actuator.min <= value && value <= range->actuator.max)
		range->actuator.current = value;
}


#ifdef DEBUG
public void Range_print(Range * range) {
	printf(
		"Range: %s (%f [%f..%f])",
		range->actuator.actuator.name,
		range->actuator.current,
		range->actuator.min,
		range->actuator.max
	);
}
#endif
