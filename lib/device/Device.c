#include <stdio.h>
#include "Device.h"

#include "../definition/Definition.h"
#include "../osi/DataLinkMessage.h"
#include "../osi/Protocol.h"

#ifdef DEBUG
public void Device_print(Object * self);
#endif


public void Device_addActuator(Object * self, ActuatorImpl * actuator);
private void processMessageFunction(DataLinkMessage * message);
private void communicate();
private void setOnConnectListener(DeviceListener_onConnectListener listener);

private Device * theDevice;

public void Device_init(Device * device, char * url, char * label) {
	int i = 0;
	device->uri = url;
	device->label = label;

	device->totalActuators = 0;

    for (i = 0; i < ACTUATOR_MAX; i++)
        device->actuators[i] = NULL;

    device->addActuator = &Device_addActuator;
    device->communicate = &communicate;
    device->setOnConnectListener = &setOnConnectListener;

#ifdef DEBUG
    device->print = &Device_print;
#endif

    theDevice = device;
    Protocol_init(&processMessageFunction);
}

public void Device_addActuator(Object * self, ActuatorImpl * actuator) {
	Device * device = (Device *) self;
	int index = device->totalActuators;

	if (device->totalActuators < ACTUATOR_MAX) {
		device->totalActuators = index+1;
		device->actuators[index] = actuator;
		actuator->actuator.index = index;

	} else {
		ERROR log("Actuators limit overflow!");
	}
}


#ifdef DEBUG
public void Device_print(Object * self) {
	Device * device = (Device *) self;
	ActuatorImpl * actuator = NULL;

	unsigned char i = 0;

	printf("Device: %s\n", device->label);
	printf(" - URL: %s\n", device->uri);
	printf(" - Actuators: \n");

	for (i=0; i<device->totalActuators; i++) {
		actuator = device->actuators[i];
		printf(" --- ");
		actuator->actuator.print(actuator);
		printf("\n");
	}
}
#endif

/***********************************************
 * Device processing messages
 ***********************************************/
/**
 * Call for communication process,
 * in your main loop
 */
private void communicate() {
	DataLinkMessage_processReceivedData();
}

private ErrorIndex controlAssignment(DataLinkMessage * message, Device * device);


private void processMessageFunction(DataLinkMessage * message) {
	ErrorIndex error = DEVICE_OK;
	INFO log(" Device - Processing message");
	LOG log(" Device - Header data: %d ", message->headerData);

	if (message->headerData == PROTOCOL_DEVICE_DESCRIPTOR) {
		Protocol_sendDeviceDescriptorResponse(theDevice);

	} else if (message->headerData == PROTOCOL_ACTUATORS_DATA) {
		Protocol_sendActuatorsDataResponse(theDevice);

	} else if (message->headerData == PROTOCOL_CONTROL_ASSIGNMENT) {
		error = controlAssignment(message, theDevice);
		if (error != DEVICE_ERROR)
			Protocol_sendControlAssignmentResponse();
		else {
			ERROR log(" Wrong Assignment");
			Protocol_sendErrorResponse();
		}

	} else if (message->headerData == PROTOCOL_CONTROL_UNASSIGNMENT) {
		Protocol_sendControlUnassignmentResponse();

	} else {
		ERROR log(" ? ");
	}
}

private ErrorIndex controlAssignment(DataLinkMessage * message, Device * device) {
	byte index, type;
	ActuatorImpl * actuator;

	RingBuffer_read(message->message, &index);
	RingBuffer_read(message->message, &type);

	if (index >= device->totalActuators)
		return DEVICE_ERROR;

	actuator = device->actuators[index];

	if (actuator->actuator.type != type)
		return DEVICE_ERROR;

	return actuator->actuator.assign(actuator, message->message) == RING_BUFFER_ERROR ? DEVICE_ERROR : DEVICE_OK;
}

/***********************************************
 * Utils
 ***********************************************/
private void setOnConnectListener(DeviceListener_onConnectListener listener) {
	dataLinkDevice.onConnectListener = listener;
}
