#ifndef DEVICE_MASTER
#define DEVICE_MASTER

#ifdef __cplusplus
extern "C" {
#endif



#include "../definition/Definition.h"

#include "../osi/DataLinkSlave.h"

extern void DeviceMaster_init();

extern void DeviceMaster_sendMessageAddressRequest();
extern void DeviceMaster_sendActuatorsDataRequest(Address address);
extern void DeviceMaster_sendControlAssigmentRequest(Address deviceAddress, unsigned char actuatorIndex, byte actuatorType, byte * values, size_t size);

typedef Function void (*DeviceMaster_DeviceDescriptorCallback)(byte address, char * uri, char * label, int totalActuators);
typedef Function void (*DeviceMaster_ActuatorDescriptorCallback)(byte address, int index, char actuatorType, char * label);
typedef Function void (*DeviceMaster_ActuatorDataCallback)(byte address, int index, int dataSize, byte * data);


extern void DeviceMaster_setDeviceDescriptorCallback(DeviceMaster_DeviceDescriptorCallback callback);
extern void DeviceMaster_setActuatorDescriptorCallback(DeviceMaster_ActuatorDescriptorCallback callback);
extern void DeviceMaster_setActuatorDataCallback(DeviceMaster_ActuatorDataCallback callback);



#ifdef __cplusplus
}
#endif

#endif
