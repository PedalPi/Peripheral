#include <stdio.h>
#include <stdbool.h>
#include "Actuator.h"

public void Actuator_init(Actuator * actuator, char * name, ActuatorType type) {
	actuator->index = ACTUATOR_UNINDEXED;
	actuator->name = name;
	actuator->type = type;

	actuator->assign = NULL;
	actuator->getTypeSize = NULL;
	actuator->getValue = NULL;
	actuator->print = NULL;
}
