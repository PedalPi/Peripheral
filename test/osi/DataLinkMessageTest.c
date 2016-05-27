#include <stdio.h>

#include "../minunit.h"
#include "../Tests.h"
#include "../../lib/osi/DataLinkMessage.h"

#include "DataLinkMessageTest.h"
#include "../../lib/osi/DataLinkSlave.h"

static void putDataLinkHeader(unsigned char version, Address address, DataLinkType type, unsigned char headerData) {
	DataLinkMessage_putByte((version << 5) | address);
	DataLinkMessage_putByte((type << 5) | headerData);
}

static void setAddressTo(Address address) {
	DataLinkMessage_putByte(DL_DELIMITER);
	putDataLinkHeader(1, DL_ADDRESS_CONNECTION, DL_TYPE_ADDRESS_ATTRIBUITION, address);
	DataLinkMessage_putByte(DL_DELIMITER);
}

static void protocolDataFunction() {
	INFO log(" Called Protocol function");
}


static char * dataSlaveEmpty() {
	DataLink_init(&protocolDataFunction);

	DataLinkMessage_putByte(DL_DELIMITER);
	DataLinkMessage_putByte(DL_DELIMITER);

	DataLinkMessage_processReceivedData();

	return 0;
}

static char * dataSlaveAddressRequest() {
	DataLink_init(&protocolDataFunction);

	DataLinkMessage_putByte(DL_DELIMITER);
	putDataLinkHeader(1, DL_ADDRESS_CONNECTION, DL_TYPE_ADDRESS_REQUEST, DL_DATA_NULL);
	DataLinkMessage_putByte(DL_DELIMITER);

	DataLinkMessage_processReceivedData();

	return 0;
}

static char * dataSlaveAddressAttribuition() {
	DataLink_init(&protocolDataFunction);

	setAddressTo(2);
	DataLinkMessage_processReceivedData();

	return 0;
}

static char * dataSlaveMessageNotForMe() {
	Address address = 2, anotherAddress = 3;
	DataLink_init(&protocolDataFunction);

	setAddressTo(address);
	DataLinkMessage_processReceivedData();

	/* Sending a message for another */
	DataLinkMessage_putByte(DL_DELIMITER);
	putDataLinkHeader(1, DL_ADDRESS_CONNECTION, DL_TYPE_ADDRESS_ATTRIBUITION, anotherAddress);
	DataLinkMessage_putByte(DL_DELIMITER);

	DataLinkMessage_processReceivedData();

	return 0;
}

static char * dataSlaveProtocol() {
	Address address = 2;
	DataLink_init(&protocolDataFunction);

	/* Set the address to 2 */
	setAddressTo(address);
	DataLinkMessage_processReceivedData();

	/* Sending a message for me */
	DataLinkMessage_putByte(DL_DELIMITER);
	putDataLinkHeader(1, address, DL_TYPE_PROTOCOL, DL_DATA_NULL);
	DataLinkMessage_putByte(DL_DELIMITER);

	DataLinkMessage_processReceivedData();

	return 0;
}

char * test_DataLinkMessage() {
	HEADER("DATA SLAVE EMPTY");
	mu_run_test(dataSlaveEmpty);

	HEADER("DATA SLAVE Address Request");
	mu_run_test(dataSlaveAddressRequest);

	HEADER("DATA SLAVE Address Attribuition");
	mu_run_test(dataSlaveAddressAttribuition);

	HEADER("DATA SLAVE Message Not For Me");
	mu_run_test(dataSlaveMessageNotForMe);

	HEADER("DATA SLAVE PROTOCOL");
	mu_run_test(dataSlaveProtocol);

    return 0;
}
