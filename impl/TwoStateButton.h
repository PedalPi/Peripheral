#ifndef TWO_STATE_BUTTON
#define TWO_STATE_BUTTON

#ifdef __cplusplus
extern "C" {
#endif



#include <stdbool.h>

#include "../lib/definition/Definition.h"
#include "../lib/device/Actuator.h"

#include "../lib/device/actuator/ButtonActuator.h"

typedef struct {
	ButtonActuator actuator;
	Pin pin;
	bool defaultState;

} TwoStateButton;

extern void TwoStateButton_init(TwoStateButton * actuator, char * name, Pin pin, bool defaultState);
extern void TwoStateButton_updateValue(TwoStateButton * button);

#ifdef DEBUG
extern void TwoStateButton_print(TwoStateButton * button);
#endif



#ifdef __cplusplus
}
#endif

#endif
