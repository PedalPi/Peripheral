#ifndef RANGE_ACTUATOR
#define RANGE_ACTUATOR

#ifdef __cplusplus
extern "C" {
#endif



#include "../../definition/Definition.h"

#include "../Actuator.h"

typedef struct {
	Actuator actuator;

	float min;
	float max;
	float current;
} RangeActuator;

extern void RangeActuator_init(RangeActuator * actuator, char * name);



#ifdef __cplusplus
}
#endif

#endif
