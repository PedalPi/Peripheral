#include <stdio.h>

#include "../minunit.h"

#include "../../lib/osi/DataLinkMessage.h"
#include "../../lib/osi/Protocol.h"

#include "ProtocolTest.h"
#include "../../lib/osi/DataLinkSlave.h"

private void processMessageFunction(DataLinkMessage * message) {
	LOG log(" Processing message...");
	LOG log(" Header data: %d", message->headerData);

	if (message->headerData == PROTOCOL_DEVICE_DESCRIPTOR) {
		LOG log(" PROTOCOL_DEVICE_DESCRIPTOR ");

	} else if (message->headerData == PROTOCOL_ACTUATORS_DATA) {
		LOG log(" PROTOCOL_ACTUATORS_DATA ");

	} else if (message->headerData == PROTOCOL_CONTROL_ASSIGNMENT) {
		LOG log(" PROTOCOL_CONTROL_ASSIGNMENT ");

	} else if (message->headerData == PROTOCOL_CONTROL_UNASSIGNMENT) {
		LOG log(" PROTOCOL_CONTROL_UNASSIGNMENT ");

	} else {
		ERROR log(" ? ");
	}
}


static void putDataLinkHeader(unsigned char version, Address address, DataLinkType type, unsigned char headerData) {
	DataLinkMessage_putByte((version << 5) | address);
	DataLinkMessage_putByte((type << 5) | headerData);
}

static void setAddressTo(Address address) {
	DataLinkMessage_putByte(DL_DELIMITER);
	putDataLinkHeader(1, DL_ADDRESS_CONNECTION, DL_TYPE_ADDRESS_ATTRIBUITION, address);
	DataLinkMessage_putByte(DL_DELIMITER);

	DataLinkMessage_processReceivedData();
}

static void setProtocol(Address address, ProtocolType type) {
	DataLinkMessage_putByte(DL_DELIMITER);
	putDataLinkHeader(1, address, DL_TYPE_PROTOCOL, type);
	DataLinkMessage_putByte(DL_DELIMITER);

	DataLinkMessage_processReceivedData();
}


static char * getProtocolTypeWorks() {
	Protocol_init(&processMessageFunction);

	setAddressTo(2);
	setProtocol(2, PROTOCOL_DEVICE_DESCRIPTOR);
	setProtocol(2, PROTOCOL_CONTROL_ASSIGNMENT);
	setProtocol(2, PROTOCOL_CONTROL_UNASSIGNMENT);
	setProtocol(2, PROTOCOL_ACTUATORS_DATA);

	setProtocol(2, 0b11111);

	return 0;
}

char * test_Protocol() {
	INFO log(">PROTOCOL TYPE WORKS");
	mu_run_test(getProtocolTypeWorks);

    return 0;
}
