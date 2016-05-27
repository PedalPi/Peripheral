#include <stdio.h>

#include "../definition/Definition.h"

#include "Physical.h"
#include "DataLinkMessage.h"
#include "DataLinkConnection.h"

#include "DataLinkMaster.h"


public void DataLink_dataMasterRoute(DataLinkMessage * message);

private Protocol_processDLMessageFunction protocolFunction = NULL;

public void DataLinkMaster_init(Protocol_processDLMessageFunction function, DataLinkMaster_onConnectCallback onConnect) {
	DLConnection_init();
	DataLinkMessage_init(&DataLink_dataMasterRoute);

	int i = 0;
	dataLinkMaster.device = &dataLinkDevice;
	dataLinkMaster.device->address = DL_ADDRESS_MASTER;
	dataLinkMaster.totalDevicesRegistred = 0;

	for (i=0; i<DATA_LINK_MAX_SLAVES; i++)
		dataLinkMaster.slavesAddress[i] = DL_ADDRESS_NULL;

	protocolFunction = function;

	dataLinkMaster.onConnect = onConnect;

	DLConnection_connect();
}

/***********************************************
 * Routering
 ***********************************************/
private void DataLinkMaster_sendMessageAddressAttribuition(DataLinkMaster * master);

public void DataLink_dataMasterRoute(DataLinkMessage * message) {
	if (!DataLinkMessage_isMessageForMe(message)) {
		LOG log(" DL Master - MESSAGE IS NOT FOR ME ");
		return;
	}

	if (message->type == DL_TYPE_ADDRESS_REQUEST) {
		DataLinkMaster_sendMessageAddressAttribuition(&dataLinkMaster);

	} else if (message->type == DL_TYPE_PROTOCOL)
		protocolFunction(message);

	else {
		ERROR log(" Is for me, but... message->type != DL_TYPE_PROTOCOL");
		INFO log(" ok... I will ignore't");
	}
}

private Address DataLinkMaster_nextFreeAddress(DataLinkMaster * master);
private void DataLinkMaster_registerDevice(DataLinkMaster * master, Address address);

private void DataLinkMaster_sendMessageAddressAttribuition(DataLinkMaster * master) {
	DataLinkMessage request = DL_MESSAGE_ADDRESS_ATTRIBUITION;

	Address address = DataLinkMaster_nextFreeAddress(master);
	request.headerData = address;

	DataLinkMessage_sendMessage(&request);

	DataLinkMaster_registerDevice(master, address);
	if (master->onConnect != NULL)
		master->onConnect(address);
}

private Address DataLinkMaster_nextFreeAddress(DataLinkMaster * master) {
	return master->totalDevicesRegistred + 2; /* 0 - Search; 1 - Master */
}

/**
 * Register Address slave in Master device addresses
 */
private void DataLinkMaster_registerDevice(DataLinkMaster * master, Address address) {
	master->slavesAddress[master->totalDevicesRegistred] = address;
	master->totalDevicesRegistred++;
}

/***********************************************
 * Messages
 ***********************************************/
public void DataLinkMaster_sendMessageAddressRequest() {
	DataLinkMessage request;
	request = DL_MESSAGE_ADDRESS_REQUEST;

	DataLinkMessage_sendMessage(&request);
}

public bool DataLinkMaster_hasMaxSlavesConnected(DataLinkMaster * master) {
	return master->totalDevicesRegistred == DATA_LINK_MAX_SLAVES;
}
