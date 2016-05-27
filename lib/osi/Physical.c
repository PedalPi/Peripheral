#include <stdio.h>
#include <stdbool.h>

#include "../../Config.h"

#include "Physical.h"

private Function void (*putData)(byte data) = NULL;

/**************************************
 * Init
 **************************************/

public void Physical_init(Physical_putDataFunction function) {
	putData = function;

	PhysicalDevice_init(SPEED_TRANSMITION);
}

/**************************************
 * Sending data
 **************************************/

public void Physical_sendByte(byte data) {
#if defined(MASTER) && defined(DEBUG)
	printf("0x%.2x ", data);
#endif

	PhysicalDevice_sendByte(data);
}

public void Physical_sendData(RingBuffer * buffer) {
	byte data;

	if (buffer == NULL)
		return;

	while (RING_BUFFER_OK == RingBuffer_read(buffer, &data))
		Physical_sendByte(data);
}

/**************************************
 * To put data
 **************************************/
public void Physical_putData(byte data) {
	if (putData != NULL)
		putData(data);
}

/**************************************
 * Getting data
 **************************************/
#if defined (DEBUG) && (!defined(ARDUINO) && !defined(RASPBERRY) && !defined(SANUSB))
public void PhysicalDevice_init(unsigned long int speedTransmition) {

}

public void PhysicalDevice_sendByte(byte data) {

}
#endif
