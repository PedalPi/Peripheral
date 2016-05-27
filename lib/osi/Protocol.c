#include <stdio.h>
#include <stdbool.h>

#include "../definition/Definition.h"

#include "ProtocolStream.h"

#include "Protocol.h"


public void Protocol_init(Protocol_processDLMessageFunction function) {
	DataLink_init(function);
}

/***********************************************
 * Message
 ***********************************************/
public void Protocol_sendMessage(Address address, ProtocolType protocolType, RingBuffer * message) {
	DataLinkMessage dlMessage = DL_MESSAGE_TRANSMITION_BASE;

	dlMessage.address    = address;
	dlMessage.type       = DL_TYPE_PROTOCOL;
	dlMessage.headerData = protocolType;
	dlMessage.message    = message;

	DataLinkMessage_sendMessage(&dlMessage);
}


/***********************************************
 * Device Descriptor
 ***********************************************/
public void Protocol_sendDeviceDescriptorResponse(Device * device) {
	unsigned char i = 0;

	ProtocolStream_sendStartStream(DL_ADDRESS_MASTER, PROTOCOL_DEVICE_DESCRIPTOR);

	ProtocolStream_sendDataString(device->uri);
	ProtocolStream_sendDataString(device->label);

	ProtocolStream_sendDataByte(device->totalActuators);

	for (i=0; i<device->totalActuators; i++) {
		ProtocolStream_sendDataByte(device->actuators[i]->actuator.type);
		ProtocolStream_sendDataString(device->actuators[i]->actuator.name);
	}

	ProtocolStream_sendEndStreamMessage();
}


/***********************************************
 * Actuators Data
 ***********************************************/
private void Protocol_prepareDataResponseMessage(Device * device, RingBuffer * buffer);
private void Protocol_prepareActuatorMessage(ActuatorImpl * actuator, RingBuffer * buffer);

public void Protocol_sendActuatorsDataResponse(Device * device) {
	RingBuffer_init(buffer, 40);

	Protocol_prepareDataResponseMessage(device, &buffer);

	Protocol_sendMessage(DL_ADDRESS_MASTER, PROTOCOL_ACTUATORS_DATA, &buffer);
}

private void Protocol_prepareDataResponseMessage(Device * device, RingBuffer * buffer) {
	ActuatorImpl * actuator = NULL;

	unsigned int idDevice = 0;

	for (idDevice=0; idDevice<device->totalActuators; idDevice++) {
		actuator = device->actuators[idDevice];
		Protocol_prepareActuatorMessage(actuator, buffer);
	}
}

private void Protocol_prepareActuatorMessage(ActuatorImpl * actuator, RingBuffer * buffer) {
	unsigned int actuatorSize = actuator->actuator.getTypeSize(actuator);
	void * valueAddress = actuator->actuator.getValue(actuator);

	RingBuffer_write(buffer, (byte) actuatorSize);
	RingBuffer_writeArray(buffer, valueAddress, actuatorSize);
}

/***********************************************
 * Control Assignment
 ***********************************************/
public void Protocol_sendControlAssignmentResponse() {
	Protocol_sendMessage(DL_ADDRESS_MASTER, PROTOCOL_CONTROL_ASSIGNMENT, NULL);
}


/***********************************************
 * Control Assignment
 ***********************************************/
public void Protocol_sendControlUnassignmentResponse() {
	Protocol_sendMessage(DL_ADDRESS_MASTER, PROTOCOL_CONTROL_UNASSIGNMENT, NULL);
}

/***********************************************
 * Error
 ***********************************************/
public void Protocol_sendErrorResponse() {
	Protocol_sendMessage(DL_ADDRESS_MASTER, PROTOCOL_ERROR, NULL);
}
