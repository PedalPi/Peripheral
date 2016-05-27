#ifndef DATA_LINK_MASTER
#define DATA_LINK_MASTER

#ifdef __cplusplus
extern "C" {
#endif



#include <stdbool.h>
#include <stddef.h>

#include "DataLinkMessage.h"
#include "DataLinkSlave.h"

typedef struct {
	/** Master is a DataLinkDevice! */
	DataLinkDevice * device;
	Address slavesAddress[DATA_LINK_MAX_SLAVES];
	unsigned char totalDevicesRegistred;

	Function void (* onConnect)(Address address);
} DataLinkMaster;

DataLinkMaster dataLinkMaster;

typedef Function void (*DataLinkMaster_onConnectCallback)(Address address);


extern void DataLinkMaster_init(Protocol_processDLMessageFunction function, DataLinkMaster_onConnectCallback onConnect);

extern bool DataLinkMaster_hasMaxSlavesConnected(DataLinkMaster * master);
extern void DataLinkMaster_sendMessageAddressRequest();



#ifdef __cplusplus
}
#endif

#endif
