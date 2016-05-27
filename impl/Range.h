#ifndef RANGE
#define RANGE

#ifdef __cplusplus
extern "C" {
#endif



#include "../lib/definition/Definition.h"
#include "../lib/device/Actuator.h"

#include "../lib/device/actuator/RangeActuator.h"

typedef struct {
	RangeActuator actuator;
} Range;

extern void Range_init(Range * range, char * name);
extern void Range_updateValue(Range * range, float value);

#ifdef DEBUG
extern void Range_print(Range * range);
#endif



#ifdef __cplusplus
}
#endif

#endif
