#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../lib/osi/Protocol.h"
#include "../lib/osi/ProtocolMaster.h"

#include "Master.h"

public void Master_init(Master * master) {
	master->totalSlaves = 0;
	int i=0;

	for (i=0; i<DATA_LINK_MAX_SLAVES; i++)
		master->slaves[i] = NULL;
}

public void Master_registerSlave(Master * master, Address address, char * uri, char * label, int totalActuators) {
	Slave * slave = Slave_new();

	char * labelCopy = (char*) malloc(strlen(label) + 1);
	strcpy(labelCopy, label);

	slave->address = address;
	slave->label = labelCopy;
	slave->totalActuators = 0;
	slave->maxActuators = totalActuators;

	master->slaves[master->totalSlaves] = slave;
	master->totalSlaves += 1;
}

public void Master_registerActuator(Master * master, Address address, int index, ActuatorType type, char * label) {
	Slave * slave = Master_getSlave(master, address);

	if (slave == NULL) {
		printf("Registring a actuator in a NULL slave\n ");
		return;
	}
	if (slave->totalActuators == slave->maxActuators || index >= slave->maxActuators) {
		printf("Actuator has more slaves then is possible\n ");
		return;
	}

	SimpleActuator * actuator = SimpleActuator_new(type);

	char * labelCopy = (char*) malloc(strlen(label) + 1);
	strcpy(labelCopy, label);

	actuator->label = labelCopy;
	slave->actuators[index] = actuator;
	slave->totalActuators += 1;
}

public Slave * Master_getSlave(Master * master, Address address) {
	Slave * slave = NULL;
	int i=0;

	for (i=0; i < master->totalSlaves; ++i) {
		slave = master->slaves[i];

		if (slave->address == address)
			return slave;
	}

	return NULL;
}

private SimpleActuator * getActuator(Master * master, Address address, int index);

public void Master_setActuatorValueFloat(Master * master, Address address, int index, float value) {
	SimpleActuator * actuator = getActuator(master, address, index);
	if (actuator == NULL) {
		printf("Actuator error\n");
		return;
	}

	actuator->rangeData = value;
}

public void Master_setActuatorValueBoolean(Master * master, Address address, int index, bool value) {
	SimpleActuator * actuator = getActuator(master, address, index);
	if (actuator == NULL) {
		printf("Actuator error\n");
		return;
	}

	actuator->buttonData = value;
}

private SimpleActuator * getActuator(Master * master, Address address, int index) {
	Slave * slave = Master_getSlave(master, address);
	if (slave == NULL) {
		printf("Slave error. Address is possible wrong\n");
		return NULL;
	}
	if (index >= slave->totalActuators) {
		printf("Slave error - Actuator index is > possible\n");
		return NULL;
	}

	return slave->actuators[index];
}

public void Master_print(Master * master) {
	int i = 0, j = 0;
	Slave * slave = NULL;
	SimpleActuator * actuator = NULL;

	printf("Master\n");
	printf(" - Total slaves: %d\n", master->totalSlaves);
	printf(" - Slaves:\n");

	for (i = 0; i < master->totalSlaves; ++i) {
		slave = master->slaves[i];
		printf("   - Slave %d - %s. Address: %d\n", i, slave->label, slave->address);
		printf("     - Actuators: \n");
		for (j = 0; j < ACTUATOR_MAX; ++j) {
			actuator = slave->actuators[j];
			if (actuator == NULL)
				break;

			printf("       - Actuator %d - ", j);
			SimpleActuator_print(actuator);
		}
	}
}



public Slave * Slave_new() {
	Slave * slave = (Slave*) malloc(sizeof(Slave));

	int i=0;
	for (i = 0; i < ACTUATOR_MAX; ++i)
		slave->actuators[i] = NULL;

	slave->address = DL_ADDRESS_NULL;
	slave->label = NULL;

	return slave;
}


public SimpleActuator * SimpleActuator_new(ActuatorType type) {
	SimpleActuator * actuator = (SimpleActuator*) malloc(sizeof(SimpleActuator));

	actuator->type = type;
	actuator->buttonData = false;
	actuator->rangeData = 0;

	return actuator;
}

public void SimpleActuator_print(SimpleActuator * actuator) {
	if (actuator->type == ACTUATOR_TYPE_BUTTON)
		printf("Button");
	else
		printf("Range");

	printf(": Actuator %s. Value - ", actuator->label);

	if (actuator->type == ACTUATOR_TYPE_BUTTON)
		actuator->buttonData ? printf("ON") : printf("OFF");
	else
		printf("%f", actuator->rangeData);

	printf("\n");
}
