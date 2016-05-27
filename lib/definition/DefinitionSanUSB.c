#include "Definition.h"

#include <stdio.h>

#ifdef SANUSB

#include "sanusb/SanUSB1.h"

unsigned int DIGITAL_READ(unsigned int pin) {
	return entrada_pino(pin);
}

void DIGITAL_WRITE(unsigned int pin, unsigned int state) {
	saida_pino(pin, state);
}

unsigned int ANALOGIC_READ(unsigned int pin) {
	return le_AD10bits(pin);
}

void DELAY(unsigned int timeMs) {
	/* Not necessary for slaves */
}

#endif
