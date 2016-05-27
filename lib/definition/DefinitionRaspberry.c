#include "Definition.h"

#include <stdio.h>

#ifdef RASPBERRY

#include <wiringPi.h>

unsigned int DIGITAL_READ(unsigned int pin) {
	return 1; /* For master, it's ever true */
}

void DIGITAL_WRITE(unsigned int pin, unsigned int state) {
	digitalWrite(pin, state);
}

unsigned int ANALOGIC_READ(unsigned int pin) {
	/* Master don't uses. Rasp can't. */
	return 0;
}

void DELAY(unsigned int timeMs) {
	delay(timeMs);
}
#endif
