#include <stdio.h>
#include <stddef.h>

#include "RangeActuator.h"

private ErrorIndex RangeActuator_assign(Object * self, RingBuffer * buffer);

private size_t RangeActuator_getTypeSize();
private void * RangeActuator_getValue(Object * self);

#ifdef DEBUG
private void RangeActuator_print(Object * self);
#endif

public void RangeActuator_init(RangeActuator * actuator, char * name) {
	Actuator_init(&actuator->actuator, name, ACTUATOR_TYPE_RANGE);

	actuator->actuator.assign = &RangeActuator_assign;
	actuator->actuator.getTypeSize = &RangeActuator_getTypeSize;
	actuator->actuator.getValue = &RangeActuator_getValue;

	actuator->min = 0;
	actuator->max = 0;
	actuator->current = 0;

#ifdef DEBUG
	actuator->actuator.print = &RangeActuator_print;
#endif
}


private void * RangeActuator_getValue(Object * self) {
	RangeActuator * range = (RangeActuator *) self;

	return &range->current;
}

private size_t RangeActuator_getTypeSize() {
	return sizeof(float);
}

private ErrorIndex RangeActuator_assign(Object * self, RingBuffer * buffer) {
	RangeActuator * actuator = (RangeActuator *) self;

	if (RingBuffer_readFloat(buffer, &actuator->min) == RING_BUFFER_ERROR)
		return RING_BUFFER_ERROR;
	if (RingBuffer_readFloat(buffer, &actuator->max) == RING_BUFFER_ERROR)
		return RING_BUFFER_ERROR;
	if (RingBuffer_readFloat(buffer, &actuator->current) == RING_BUFFER_ERROR)
		return RING_BUFFER_ERROR;

	return RING_BUFFER_OK;
}


#ifdef DEBUG
private void RangeActuator_print(Object * self) {
	RangeActuator * range = (RangeActuator *) self;

	printf(
		"(R) %s (%f [%f..%f])",
		range->actuator.name,
		range->current,
		range->min,
		range->max
	);
}
#endif
