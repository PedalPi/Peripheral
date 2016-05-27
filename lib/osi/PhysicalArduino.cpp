#include <stdio.h>
#include <stdbool.h>

#include "../../Config.h"

#include "Physical.h"

/**************************************
 * Getting data
 **************************************/
#ifdef ARDUINO

#include "Arduino.h"
void PhysicalDevice_init(unsigned long int speedTransmition) {
	Serial.begin(speedTransmition);
}

void PhysicalDevice_sendByte(byte data) {
	Serial.write(data);
}

void serialEvent() {
	while (Serial.available()) {
		Physical_putData((byte) Serial.read());
	}
}
#endif
