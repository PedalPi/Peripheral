#ifndef PROTOCOL_MASTER
#define PROTOCOL_MASTER

#ifdef __cplusplus
extern "C" {
#endif


#include "../util/RingBuffer.h"

#include "../device/Device.h"
#include "../device/Actuator.h"
#include "DataLinkSlave.h"
#include "DataLinkMaster.h"

extern void ProtocolMaster_init(Protocol_processDLMessageFunction function, DataLinkMaster_onConnectCallback onConnect);

extern void Protocol_sendMessageAddressRequest();

extern void Protocol_sendDeviceDescriptorRequest(Address address);
extern void Protocol_sendActuatorsDataRequest(Address address);
extern void Protocol_sendControlAssignmentRequest(Address address, unsigned char actuatorIndex, ActuatorType actuatorType, byte * values, size_t size);
extern void Protocol_sendControlUnassignmentRequest(unsigned char actuatorIndex);




#ifdef __cplusplus
}
#endif

#endif
