#include "DataTransmitionUtil.h"

/**************************************
 * Sending data
 **************************************/
public void putDelimiter() {
	DataLinkMessage_putByte(DL_DELIMITER);
}

public void putByte(byte data) {
	if (data == DL_ESCAPE || data == DL_DELIMITER)
		DataLinkMessage_putByte(DL_ESCAPE);
	DataLinkMessage_putByte(data);
}

public void putFloat(float value) {
	unsigned char i;

	byte * address = (byte *) &value;
	for (i = 0; i < sizeof(float); i++)
		putByte(address[i]);
}

public void putArray(byte * data, size_t size) {
	int i = 0;
	for (i = 0; i < size; i++)
		putByte(data[i]);
}

public void putString(char * string) {
	int i = 0;
	char data = ' ';

	do {
		data = string[i];
		putByte(data);
		i++;
	} while(data != '\0');
}

/**************************************
 * Sending data
 **************************************/
public void DL_HEADER(unsigned char version, Address address, DataLinkType type, unsigned char headerData) {
	putByte((version << 5) | address);
	putByte((type << 5) | headerData);
}

public void SET_ADDRESS_TO(Address address) {
	putDelimiter();
	DL_HEADER(1, DL_ADDRESS_CONNECTION, DL_TYPE_ADDRESS_ATTRIBUITION, address);
	putDelimiter();

	DataLinkMessage_processReceivedData();
}

public void START_PROTOCOL(Address address, ProtocolType type) {
	putDelimiter();
	DL_HEADER(1, address, DL_TYPE_PROTOCOL, type);
}


/**************************************
 * Control assignment
 **************************************/
public void HEADER_ASSIGNMENT(Address deviceAddress, unsigned char actuatorIndex, ActuatorType type) {
	START_PROTOCOL(deviceAddress, PROTOCOL_CONTROL_ASSIGNMENT);

	putByte(actuatorIndex);
	putByte(type);
}

public void RANGE_ASSIGNMENT(Address deviceAddress, unsigned char actuatorIndex, float min, float max, float current) {
	HEADER_ASSIGNMENT(deviceAddress, actuatorIndex, ACTUATOR_TYPE_RANGE);

	putFloat(min);
	putFloat(max);
	putFloat(current);

	putDelimiter();
	DataLinkMessage_processReceivedData();
}

public void BUTTON_ASSIGNMENT(Address deviceAddress, unsigned char actuatorIndex, bool value) {
	HEADER_ASSIGNMENT(deviceAddress, actuatorIndex, ACTUATOR_TYPE_BUTTON);

	putByte(value);

	putDelimiter();
	DataLinkMessage_processReceivedData();
}

/**************************************
 * Actuator data
 **************************************/
public void ACTUATOR_DATA_REQUEST(Address deviceAddress) {
	START_PROTOCOL(deviceAddress, PROTOCOL_ACTUATORS_DATA);
	putDelimiter();
	DataLinkMessage_processReceivedData();
}

public void DEVICE_DESCRIPTOR_RESPONSE(Address deviceAddress) {
	START_PROTOCOL(deviceAddress, PROTOCOL_DEVICE_DESCRIPTOR);
	putDelimiter();
	DataLinkMessage_processReceivedData();
}
