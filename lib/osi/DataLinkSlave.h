#ifndef DATA_LINK_SLAVE
#define DATA_LINK_SLAVE

#ifdef __cplusplus
extern "C" {
#endif

#include "DataLinkMessage.h"

#define DATA_LINK_ERROR -1
#define DATA_LINK_OK 1

/* 3 bits - 1 (0b000 - find device) - 1 (0b111 - broadcast) - 1 (0b001 Master address)
 */
#define DATA_LINK_MAX_SLAVES 13

#define Address unsigned char

typedef struct {
	Address address;
	void (* onConnectListener)();
} DataLinkDevice;

extern DataLinkDevice dataLinkDevice;


extern void DataLink_init(Protocol_processDLMessageFunction function);

extern void DataLink_slaveAddressRequest(DataLinkMessage * request);
extern void DataLink_slaveAddressAttribuition(DataLinkMessage * request);


#ifdef __cplusplus
}
#endif

#endif
