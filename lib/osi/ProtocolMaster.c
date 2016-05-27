#include <stdio.h>
#include <stdbool.h>

#include "../definition/Definition.h"

#include "Protocol.h"
#include "ProtocolMaster.h"


public void ProtocolMaster_init(Protocol_processDLMessageFunction function, DataLinkMaster_onConnectCallback onConnect) {
	DataLinkMaster_init(function, onConnect);
}

/***********************************************
 * Address Request
 ***********************************************/
public void Protocol_sendMessageAddressRequest() {
	if (DataLinkMaster_hasMaxSlavesConnected(&dataLinkMaster)) {
		ERROR log(" DL - Max slaves registred\n!");
	} else {
		DataLinkMaster_sendMessageAddressRequest(&dataLinkMaster);
	}
}


/***********************************************
 * Device Descriptor
 ***********************************************/
public void Protocol_sendDeviceDescriptorRequest(Address address) {
	Protocol_sendMessage(address, PROTOCOL_DEVICE_DESCRIPTOR, NULL);
}

/***********************************************
 * Actuators Data
 ***********************************************/
public void Protocol_sendActuatorsDataRequest(Address address) {
	Protocol_sendMessage(address, PROTOCOL_ACTUATORS_DATA, NULL);
}

/***********************************************
 * Control Assignment
 ***********************************************/
public void Protocol_sendControlAssignmentRequest(Address address, unsigned char actuatorIndex, ActuatorType actuatorType, byte * values, size_t size) {
	RingBuffer_init(buffer, size+2);
	RingBuffer_write(&buffer, actuatorIndex);
	RingBuffer_write(&buffer, actuatorType);
	RingBuffer_writeArray(&buffer, values, size);

	Protocol_sendMessage(address, PROTOCOL_CONTROL_ASSIGNMENT, &buffer);
}

/***********************************************
 * Control Assignment
 ***********************************************/
public void Protocol_sendControlUnassignmentRequest(unsigned char actuatorIndex) {
	RingBuffer_init(buffer, 1);
	RingBuffer_write(&buffer, actuatorIndex);

	Protocol_sendMessage(DL_ADDRESS_MASTER, PROTOCOL_CONTROL_UNASSIGNMENT, &buffer);
}
