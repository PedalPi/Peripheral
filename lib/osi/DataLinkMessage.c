#include <stdio.h>
#include <stdbool.h>

#include "../../Config.h"

#include "../definition/Definition.h"
#include "DataLinkSlave.h"
#include "Physical.h"

#include "DataLinkMessage.h"
#include "DataLinkConnection.h"

/***********************************************
 * Messages default/base
 ***********************************************/
/**
 * If necessary, send all attributes NULL and DL_DATA_NULL
 */
public const DataLinkMessage DL_MESSAGE_TRANSMITION_BASE = {
	1, DL_ADDRESS_NULL, DL_TYPE_NACK, DL_DATA_NULL, NULL
};


public const DataLinkMessage DL_MESSAGE_ACK = {
	1, DL_ADDRESS_NULL, DL_TYPE_ACK, DL_DATA_NULL, NULL
};


public const DataLinkMessage DL_MESSAGE_ADDRESS_REQUEST = {
	1, DL_ADDRESS_NULL, DL_TYPE_ADDRESS_REQUEST, DL_DATA_NULL, NULL
};

/**
 * Is necessary attribuite the address in dataLinkMessage->data
 */
public const DataLinkMessage DL_MESSAGE_ADDRESS_ATTRIBUITION = {
	1, DL_ADDRESS_CONNECTION, DL_TYPE_ADDRESS_ATTRIBUITION, DL_DATA_NULL, NULL
};


/***********************************************
 * Initialization
 ***********************************************/
#ifdef SANSUB
#pragma udata data_link_message_section_1
#endif
private RingBuffer_init(buffer, MAX_SIZE_MESSAGE);

#ifdef SANSUB
#pragma udata data_link_message_section_2
#endif
private DataLinkMessage receivedMessage;
private Function void (*dlRouteFunction)(DataLinkMessage * message) = NULL;

public void DataLinkMessage_init(DataLinkMessage_routeFunction function) {
	Physical_init(&DataLinkMessage_putByte);

	receivedMessage.version    = 1;
	receivedMessage.address    = DL_ADDRESS_NULL;
	receivedMessage.type       = DL_TYPE_ACK;
	receivedMessage.headerData = DL_DATA_NULL;

	receivedMessage.message = &buffer;

	dlRouteFunction = function;
}


/***********************************************
 * Message
 ***********************************************/
#ifdef DEBUG
public void DataLinkMessage_print(DataLinkMessage * message) {
	printf("DataLinkMessage\n");
	printf(" - Version: %u\n", message->version);
	printf(" - Address: %u\n", message->address);
	printf(" - Type: %u\n", message->type);
	printf(" - Data: %u\n", message->headerData);

	printf(" - Content: \n");
	RingBuffer_print(message->message);
	printf("\n");
}
#endif

/***********************************************
 * Creating messages
 ***********************************************/
/**
 * Converts PhysicalMessage in DataLinkMessage
 */
private void DataLink_makeHeader(DataLinkMessage * message, RingBuffer * buffer) {
	byte byte1, byte2;
	RingBuffer_read(buffer, &byte1);
	RingBuffer_read(buffer, &byte2);

	message->version = DL_MASK_VERSION(byte1);
	message->address = DL_MASK_ADDRESS(byte1);
	message->type    = DL_MASK_TYPE(byte2);
	message->headerData = DL_MASK_DATA(byte2);
}

/***********************************************
 * Transmition - Send
 ***********************************************/
private void DataLinkMessage_sendHeader(DataLinkMessage * message);
private void DataLinkMessage_sendData(RingBuffer * buffer);

public void DataLinkMessage_sendMessage(DataLinkMessage * message) {
	INFO log(" DL - Sending message: ");

	DataLinkMessage_startStream(message);
	DataLinkMessage_sendData(message->message);
	DataLinkMessage_closeStream();
}



private void DataLinkMessage_sendHeader(DataLinkMessage * message) {
	byte firstByte  = message->version << 5 | (DL_MASK_ADDRESS(message->address));
	byte secondByte = message->type    << 5 | (DL_MASK_DATA(message->headerData));

	Physical_sendByte(firstByte);
	Physical_sendByte(secondByte);
}

private void DataLinkMessage_sendData(RingBuffer * buffer) {
	byte data;

	if (buffer == NULL)
		return;

	while (RING_BUFFER_OK == RingBuffer_read(buffer, &data))
		DataLinkMessage_sendByte(data);
}

public void DataLinkMessage_sendByte(byte data) {
	if (data == DL_ESCAPE || data == DL_DELIMITER)
		Physical_sendByte(DL_ESCAPE);

	Physical_sendByte(data);
}

/***********************************************
 * Stream - For send
 ***********************************************/
/**
 * This start stream:
 *
 * - Start delimiter
 * - DataLink Header
 *
 * This don't send any DataLinkMessage.message data!
 */
public void DataLinkMessage_startStream(DataLinkMessage * message) {
	Physical_sendByte(DL_DELIMITER);

	DataLinkMessage_sendHeader(message);
}

public void DataLinkMessage_closeStream() {
	Physical_sendByte(DL_DELIMITER);
}

/***********************************************
 * Transmition - Receive
 ***********************************************/
static bool ESCAPE = false;

static bool PROCESS_MESSAGE = false;


/**
 * Physical calls this sending a byte readed in Serial transmition
 */
public void DataLinkMessage_putByte(byte data) {
	if (!DLConnection_isEstablished())
		return;

	#define isStartMessage (data == DL_DELIMITER &&  RingBuffer_isEmpty(receivedMessage.message))
	#define isEndMessage   (data == DL_DELIMITER && !RingBuffer_isEmpty(receivedMessage.message))
	#define isEscapeTheNext (!ESCAPE && data == DL_ESCAPE)
	#define isEscapeThis    (ESCAPE)

	if (isEscapeTheNext)
		ESCAPE = true;

	else if (isEscapeThis) {
		RingBuffer_write(receivedMessage.message, data);
		ESCAPE = false;

	} else if (isStartMessage)
		return;

	else if (!isEndMessage)
		RingBuffer_write(receivedMessage.message, data);

	else {
		PROCESS_MESSAGE = true;
#ifdef MASTER
		DataLinkMessage_processReceivedData();
#endif
	}
}

/**
 * Call for communication process,
 * in your main loop
 */
public void DataLinkMessage_processReceivedData() {
    if (!PROCESS_MESSAGE)
        return;

#ifdef DEBUG
    INFO log(" DL - Received message: ");
    RingBuffer_print(receivedMessage.message);
#endif
    DataLink_makeHeader(&receivedMessage, receivedMessage.message);
    dlRouteFunction(&receivedMessage);
    RingBuffer_clear(receivedMessage.message);

    PROCESS_MESSAGE = false;
}

/***********************************************
 * Data comparation
 ***********************************************/
public bool DataLinkMessage_isMessageForMe(DataLinkMessage * message) {
	bool newConnectionForMe = dataLinkDevice.address == DL_ADDRESS_NULL && message->address == DL_ADDRESS_CONNECTION;
	bool forMe     = dataLinkDevice.address != DL_ADDRESS_NULL && message->address == dataLinkDevice.address;
	bool broadcast = message->address == DL_ADDRESS_BROADCAST;

	return newConnectionForMe || forMe || broadcast;
}

