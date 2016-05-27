#ifndef KNOB
#define KNOB

#ifdef __cplusplus
extern "C" {
#endif



#include "../lib/definition/Definition.h"
#include "../lib/device/Actuator.h"

#include "../lib/device/actuator/RangeActuator.h"

typedef struct {
	RangeActuator actuator;
	Pin pin;
} Knob;

extern void Knob_init(Knob * knob, char * name, Pin pin);
extern void Knob_updateValue(Knob * knob);

#ifdef DEBUG
extern void Knob_print(Knob * knob);
#endif



#ifdef __cplusplus
}
#endif

#endif
