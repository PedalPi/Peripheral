#ifndef DATA_LINK_MESSAGE
#define DATA_LINK_MESSAGE


#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

#include "../util/RingBuffer.h"

#include "Physical.h"

#define DL_ADDRESS_CONNECTION 0b00000
#define DL_ADDRESS_BROADCAST  0b11111
#define DL_ADDRESS_NULL       0b00000
#define DL_ADDRESS_MASTER     0b00001

#define DL_DATA_NULL 0b00000000

#define DL_MASK_VERSION(data) ((data & 0b11100000) >> 5)
#define DL_MASK_ADDRESS(data) (data & 0b00011111)
#define DL_MASK_TYPE(data) (DL_MASK_VERSION(data))
#define DL_MASK_DATA(data) (DL_MASK_ADDRESS(data))

#define DL_DELIMITER '\n'
#define DL_ESCAPE '\\'

typedef enum {
	DL_TYPE_NULL                 = 0b000,
	DL_TYPE_PROTOCOL             = 0b001,
	DL_TYPE_ADDRESS_REQUEST      = 0b010,
	DL_TYPE_ADDRESS_ATTRIBUITION = 0b011,
	DL_TYPE_ACK                  = 0b100,
	DL_TYPE_NACK                 = 0b101
} DataLinkType;

typedef struct {
	unsigned char version    : 3;
	unsigned char address    : 5;
	DataLinkType type        : 3;
	unsigned char headerData : 5;

	RingBuffer * message;
} DataLinkMessage;


extern const DataLinkMessage DL_MESSAGE_TRANSMITION_BASE;
extern const DataLinkMessage DL_MESSAGE_ACK;
extern const DataLinkMessage DL_MESSAGE_ADDRESS_REQUEST;
extern const DataLinkMessage DL_MESSAGE_ADDRESS_ATTRIBUITION;

typedef Function void (*DataLinkMessage_routeFunction)(DataLinkMessage * message);

/**
 * Send data to upper protocol layer
 */
typedef Function void (*Protocol_processDLMessageFunction)(DataLinkMessage * message);

extern void DataLinkMessage_init(DataLinkMessage_routeFunction function);


/**
 * RX - Receive data
 */
extern void DataLinkMessage_putByte(byte data);
extern bool DataLinkMessage_isMessageForMe(DataLinkMessage * device);
extern void DataLinkMessage_processReceivedData();

/**
 * TX - Transmit data
 */
extern void DataLinkMessage_sendMessage(DataLinkMessage * message);
extern void DataLinkMessage_startStream(DataLinkMessage * message);
extern void DataLinkMessage_sendByte(byte data);
extern void DataLinkMessage_closeStream();

#ifdef DEBUG
extern void DataLinkMessage_print(DataLinkMessage * message);
#endif


#ifdef __cplusplus
}
#endif

#endif
