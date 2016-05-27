#ifndef LIB_OSI_PROTOCOLSTREAM_H_
#define LIB_OSI_PROTOCOLSTREAM_H_


#ifdef __cplusplus
extern "C" {
#endif



#include "../definition/Definition.h"
#include "DataLinkSlave.h"
#include "Protocol.h"

/**
 * Uses ProtocolStream if the data is biggest for
 * buffer limitation (MaxStackSize, MaxArrayLimit)
 */

extern void ProtocolStream_sendStartStream(Address address, ProtocolType protocolType);

extern void ProtocolStream_sendDataByte(byte data);
extern void ProtocolStream_sendDataString(char * data);

extern void ProtocolStream_sendEndStreamMessage();



#ifdef __cplusplus
}
#endif


#endif /* LIB_OSI_PROTOCOLSTREAM_H_ */
