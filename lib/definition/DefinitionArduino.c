#include "Definition.h"

#include <stdio.h>

#if defined ARDUINO

#include "Arduino.h"

unsigned int DIGITAL_READ(unsigned int pin) {
	return digitalRead(pin);
}

void DIGITAL_WRITE(unsigned int pin, unsigned int state) {
	digitalWrite(pin, state);
}

unsigned int ANALOGIC_READ(unsigned int pin) {
	return analogRead(pin);
}

/**
 * Delay in ms
 */
void DELAY(unsigned int timeMs) {
	/* Not necessary for slaves */
}

#endif
