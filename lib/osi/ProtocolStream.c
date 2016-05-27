#include "ProtocolStream.h"


public void ProtocolStream_sendStartStream(Address address, ProtocolType protocolType) {
	DataLinkMessage dlMessage = DL_MESSAGE_TRANSMITION_BASE;

	dlMessage.address    = address;
	dlMessage.type       = DL_TYPE_PROTOCOL;
	dlMessage.headerData = protocolType;
	dlMessage.message    = NULL;


	DataLinkMessage_startStream(&dlMessage);
}

public void ProtocolStream_sendDataByte(byte data) {
	DataLinkMessage_sendByte(data);
}

public void ProtocolStream_sendDataString(char * string) {
	int index = 0;
	char data = ' ';

	do {
		data = string[index];
		ProtocolStream_sendDataByte(data);
		index++;
	} while (data != '\0');
}

public void ProtocolStream_sendEndStreamMessage() {
	DataLinkMessage_closeStream();
}

