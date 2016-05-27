#ifndef MASTER_MASTER_H_
#define MASTER_MASTER_H_

#include "../lib/device/Actuator.h"
#include "../lib/osi/DataLinkSlave.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct {
	ActuatorType type;
	char * label;
	bool buttonData;
	float rangeData;
} SimpleActuator;

typedef struct {
	Address address;
	char * label;
	SimpleActuator * actuators[ACTUATOR_MAX];
	int totalActuators;
	int maxActuators;
} Slave;

typedef struct {
	int totalSlaves;
	Slave *slaves[DATA_LINK_MAX_SLAVES];
} Master;

extern void Master_init(Master * master);
extern void Master_registerSlave(Master * master, Address address, char * uri, char * label, int totalActuators);
extern void Master_registerActuator(Master * master, Address address, int index, ActuatorType type, char * label);
extern Slave * Master_getSlave(Master * master, Address address);
extern void Master_print(Master * master);

extern Slave * Slave_new();

extern SimpleActuator * SimpleActuator_new(ActuatorType type);
extern void SimpleActuator_print(SimpleActuator * actuator);

extern void Master_setActuatorValueFloat(Master * master, Address address, int index, float value);
extern void Master_setActuatorValueBoolean(Master * master, Address address, int index, bool value);

extern void Master_startManagementComunication(Master * theMaster);

#ifdef __cplusplus
}
#endif

#endif
