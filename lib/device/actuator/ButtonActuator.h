#ifndef BUTTON_ACTUATOR
#define BUTTON_ACTUATOR

#ifdef __cplusplus
extern "C" {
#endif


#include <stdbool.h>

#include "../Actuator.h"

typedef struct {
	Actuator actuator;

	bool state;
} ButtonActuator;

extern void ButtonActuator_init(ButtonActuator * actuator, char * name);



#ifdef __cplusplus
}
#endif

#endif
