#ifndef DATA_TRANSMITION_UTIL
#define DATA_TRANSMITION_UTIL

#include "../lib/definition/Definition.h"
#include "../lib/osi/DataLinkMessage.h"
#include "../lib/osi/DataLinkSlave.h"

#include "../lib/osi/Protocol.h"

#include "../lib/device/Device.h"


extern void putDelimiter();
extern void putByte(byte data);
extern void putFloat(float value);
extern void putArray(byte * data, size_t size);
extern void putString(char * string);

extern void DL_HEADER(unsigned char version, Address address, DataLinkType type, unsigned char headerData);
extern void SET_ADDRESS_TO(Address address);
extern void START_PROTOCOL(Address address, ProtocolType type);

extern void HEADER_ASSIGNMENT(Address deviceAddress, unsigned char actuatorIndex, ActuatorType type);
extern void RANGE_ASSIGNMENT(Address deviceAddress, unsigned char actuatorIndex, float min, float max, float current);
extern void BUTTON_ASSIGNMENT(Address deviceAddress, unsigned char actuatorIndex, bool value);

extern void ACTUATOR_DATA_REQUEST(Address deviceAddress);
extern void DEVICE_DESCRIPTOR_RESPONSE(Address deviceAddress);

#endif
