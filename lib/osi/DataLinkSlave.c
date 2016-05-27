#include <stdio.h>

#include "../definition/Definition.h"

#include "Physical.h"
#include "DataLinkMessage.h"
#include "DataLinkConnection.h"

#include "DataLinkSlave.h"

public DataLinkDevice dataLinkDevice;

private void DataLink_dataSlaveRoute(DataLinkMessage * message);

private Protocol_processDLMessageFunction protocolFunction = NULL;

public void DataLink_init(Protocol_processDLMessageFunction function) {
	DLConnection_init();
	DataLinkMessage_init(&DataLink_dataSlaveRoute);

	dataLinkDevice.address = DL_ADDRESS_NULL;
	dataLinkDevice.onConnectListener = NULL;

	protocolFunction = function;
}

/***********************************************
 * Routering
 ***********************************************/
/**
 * Route the message for use it correctly
 */
private void DataLink_dataSlaveRoute(DataLinkMessage * message) {
	if (!DataLinkMessage_isMessageForMe(message)) {
		LOG log(" DL Slave - MESSAGE IS NOT FOR ME ");
		return;
	}

	if (message->type == DL_TYPE_ADDRESS_REQUEST) {
		if (dataLinkDevice.address != DL_ADDRESS_NULL)
			return;

		DataLink_slaveAddressRequest(message);

	} else if (message->type == DL_TYPE_ADDRESS_ATTRIBUITION) {
		DataLink_slaveAddressAttribuition(message);
		DLConnection_connect();

	} else if (message->type == DL_TYPE_PROTOCOL)
		protocolFunction(message);

	else {
		ERROR log(" Is for me, but... message->type != DL_TYPE_PROTOCOL");
		INFO log(" ok... I will ignore't");
	}
}


public void DataLink_slaveAddressRequest(DataLinkMessage * request) {
	DataLinkMessage response;

	response = DL_MESSAGE_ADDRESS_REQUEST;
	response.address = DL_ADDRESS_MASTER;

	LOG log(" DL - SENDING REQUEST ADDRESS ");

	DataLinkMessage_sendMessage(&response);
}

public void DataLink_slaveAddressAttribuition(DataLinkMessage * request) {
	/*DataLinkMessage response = DL_MESSAGE_ACK;*/

	dataLinkDevice.address = request->headerData;
	LOG log(" DL - Device address is: %u ", dataLinkDevice.address);

	LOG log(" DL - NOT SEND ACK ");

	/*DataLinkMessage_sendMessage(&response);*/

	if (dataLinkDevice.onConnectListener != NULL)
		dataLinkDevice.onConnectListener();
}
