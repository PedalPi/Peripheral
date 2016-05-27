#ifndef DEVICE
#define DEVICE

#ifdef __cplusplus
extern "C" {
#endif



#include "../../Config.h"
#include "../util/RingBuffer.h"
#include "Actuator.h"

#define DEVICE_ERROR -1
#define DEVICE_OK     1

typedef Function void (*DeviceListener_onConnectListener)();

typedef struct {
	char * uri;
	char * label;

	unsigned char totalActuators;
	ActuatorImpl * actuators[ACTUATOR_MAX];

	void (* addActuator)(Object * self, ActuatorImpl * actuator);
	void (* communicate)();
	void (* setOnConnectListener)(DeviceListener_onConnectListener function);

#ifdef DEBUG
	void (* print)(Object * self);
#endif
} Device;


extern void Device_init(Device * device, char * url, char * label);
extern void process();


#ifdef __cplusplus
}
#endif

#endif
