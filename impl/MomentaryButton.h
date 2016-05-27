#ifndef MOMENTARY_BUTTON
#define MOMENTARY_BUTTON

#ifdef __cplusplus
extern "C" {
#endif



#include "../lib/definition/Definition.h"
#include "../lib/device/Actuator.h"

#include "../lib/device/actuator/ButtonActuator.h"

typedef struct {
	ButtonActuator actuator;
	Pin pin;
} MomentaryButton;

extern void MomentaryButton_init(MomentaryButton * actuator, char * name, Pin pin);
extern void MomentaryButton_updateValue(MomentaryButton * button);

#ifdef DEBUG
extern void MomentaryButton_print(MomentaryButton * button);
#endif



#ifdef __cplusplus
}
#endif

#endif
