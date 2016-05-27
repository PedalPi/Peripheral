#include "DataLinkConnection.h"

#include <stdio.h>
#include <stdbool.h>

private void blockConnectionNextNode(DLConnection * connection);

private DLConnection connection;

public void DLConnection_init() {
	connection.nodeInput = DL_CONNECTION_PIN_PREVIOUS;
	connection.nodeOutput = DL_CONNECTION_PIN_NEXT;

	blockConnectionNextNode(&connection);
}


/***********************************************
 * Liberate and block nodes connections
 ***********************************************/

private void liberateConnectionNextNode(DLConnection * connection) {
	DIGITAL_WRITE(connection->nodeOutput, HIGH);
}

private void blockConnectionNextNode(DLConnection * connection) {
	DIGITAL_WRITE(connection->nodeOutput, LOW);
}


/**
 * Requisita a conexão do nó e retorna seu endereço
 */
public void DLConnection_connect() {
	liberateConnectionNextNode(&connection);
}

public bool DLConnection_isEstablished() {
	return DIGITAL_READ(connection.nodeInput) == HIGH;
}
