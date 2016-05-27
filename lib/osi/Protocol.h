#ifndef PROTOCOL
#define PROTOCOL

#ifdef __cplusplus
extern "C" {
#endif

#include "DataLinkSlave.h"
#include "DataLinkMessage.h"

#include "../util/RingBuffer.h"
#include "../device/Device.h"

typedef enum {
	PROTOCOL_DEVICE_DESCRIPTOR    = 0b00001,

	PROTOCOL_CONTROL_ASSIGNMENT   = 0b00010,
	PROTOCOL_CONTROL_UNASSIGNMENT = 0b00011,
	PROTOCOL_ACTUATORS_DATA       = 0b00100,

	PROTOCOL_ERROR                = 0b11111
} ProtocolType;


extern void Protocol_init(Protocol_processDLMessageFunction function);

extern void Protocol_sendMessage(Address address, ProtocolType protocolType, RingBuffer * message);

extern void Protocol_sendDeviceDescriptorResponse(Device * device);
extern void Protocol_sendActuatorsDataResponse(Device * device);
extern void Protocol_sendControlAssignmentResponse();
extern void Protocol_sendControlUnassignmentResponse();

extern void Protocol_sendErrorResponse();


#ifdef __cplusplus
}
#endif

#endif
