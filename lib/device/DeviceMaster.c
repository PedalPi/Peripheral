#include <stdbool.h>

#include "DeviceMaster.h"

#include "../definition/Definition.h"

#include "../osi/DataLinkMessage.h"
#include "../osi/DataLinkMaster.h"
#include "../osi/DataLinkSlave.h"

#include "../osi/ProtocolMaster.h"
#include "../osi/Protocol.h"

private void processMessageFunction(DataLinkMessage * message);
private void DeviceMaster_onConnectDevice(Address address);

#define lastAddress (dataLinkMaster.totalDevicesRegistred-1 + 2)


typedef struct {
	Address addressComunicating;
	DeviceMaster_DeviceDescriptorCallback callbackDeviceDescriptor;
	DeviceMaster_ActuatorDescriptorCallback callbackActuatorDescriptor;
	DeviceMaster_ActuatorDataCallback callbackActuatorData;
} DeviceMaster;

private DeviceMaster deviceMaster;

public void DeviceMaster_init() {
	deviceMaster.addressComunicating = DL_ADDRESS_NULL;
	deviceMaster.callbackDeviceDescriptor = NULL;
	deviceMaster.callbackActuatorDescriptor = NULL;
	deviceMaster.callbackActuatorData = NULL;

	ProtocolMaster_init(&processMessageFunction, &DeviceMaster_onConnectDevice);
}


/***********************************************
 * Messages sending
 ***********************************************/
public void DeviceMaster_sendMessageAddressRequest() {
	deviceMaster.addressComunicating = DL_ADDRESS_NULL;
	Protocol_sendMessageAddressRequest();
}

public void DeviceMaster_sendActuatorsDataRequest(Address deviceAddress) {
	deviceMaster.addressComunicating = deviceAddress;
	Protocol_sendActuatorsDataRequest(deviceAddress);
}

public void DeviceMaster_sendControlAssigmentRequest(Address deviceAddress, unsigned char actuatorIndex, byte actuatorType, byte * values, size_t size) {
	deviceMaster.addressComunicating = deviceAddress;
	Protocol_sendControlAssignmentRequest(deviceAddress, actuatorIndex, (ActuatorType) actuatorType, values, size);
}

/***********************************************
 * On connect device
 ***********************************************/

private void DeviceMaster_onConnectDevice(Address address) {
	DELAY(20);
	Protocol_sendDeviceDescriptorRequest(address);
}

/***********************************************
 * Callbacks
 ***********************************************/
public void DeviceMaster_setDeviceDescriptorCallback(DeviceMaster_DeviceDescriptorCallback callback) {
	deviceMaster.callbackDeviceDescriptor = callback;
}

public void DeviceMaster_setActuatorDescriptorCallback(DeviceMaster_ActuatorDescriptorCallback callback) {
	deviceMaster.callbackActuatorDescriptor = callback;
}

public void DeviceMaster_setActuatorDataCallback(DeviceMaster_ActuatorDataCallback callback) {
	deviceMaster.callbackActuatorData = callback;
}
/***********************************************
 * Processing messages recepted
 ***********************************************/
private void callCallbackDeviceDescriptor(DataLinkMessage * message, Address address);
private void callCallbacksActuatorData(DataLinkMessage * message, Address device);
private void callCallbackActuatorDescriptor(Address address, int totalActuators, RingBuffer * message);

private void processMessageFunction(DataLinkMessage * message) {
	INFO log(" DeviceMaster - Processing message");
	LOG log(" DeviceMaster - Header data: %d ", message->headerData);

	if (message->headerData == PROTOCOL_DEVICE_DESCRIPTOR) {
		callCallbackDeviceDescriptor(message, lastAddress);

	} else if (message->headerData == PROTOCOL_ACTUATORS_DATA) {
		callCallbacksActuatorData(message, deviceMaster.addressComunicating);

	} else {
		ERROR log(" ? ");
	}
}

private void callCallbackDeviceDescriptor(DataLinkMessage * message, Address address) {
	char uri[255];
	char label[100];
	int totalActuators = 0;

	RingBuffer_readString(message->message, uri);
	RingBuffer_readString(message->message, label);

	RingBuffer_read(message->message, (byte *) &totalActuators);

	if (deviceMaster.callbackDeviceDescriptor != NULL) {
		deviceMaster.callbackDeviceDescriptor(address, uri, label, totalActuators);
		callCallbackActuatorDescriptor(address, totalActuators, message->message);
	}
}

private void callCallbackActuatorDescriptor(Address address, int totalActuators, RingBuffer * message) {
	if (deviceMaster.callbackActuatorDescriptor == NULL)
		return;

	int index = 0;
	byte type = ' ';
	char label[100];

	for (index=0; index<totalActuators; index++) {
		RingBuffer_read(message, &type);
		RingBuffer_readString(message, label);

		deviceMaster.callbackActuatorDescriptor(address, index, type, label);
	}
}

private void callCallbackActuatorData(Address device, char idActuator, char dataSize, RingBuffer * buffer);

private void callCallbacksActuatorData(DataLinkMessage * message, Address device) {
	char idActuator = 0;
	char dataSize = 0;
	ErrorIndex error = RING_BUFFER_OK;

	while (true) {
		error = RingBuffer_read(message->message, (byte*) &dataSize);
		if (error == RING_BUFFER_ERROR)
			return;

		callCallbackActuatorData(device, idActuator, dataSize, message->message);
		idActuator++;
	}
}

private void callCallbackActuatorData(Address device, char idActuator, char dataSize, RingBuffer * buffer) {
	byte data[dataSize];

	RingBuffer_readArray(buffer, (byte *) &data, dataSize);

	if (deviceMaster.callbackActuatorData != NULL)
		deviceMaster.callbackActuatorData(device, idActuator, dataSize, data);
}
