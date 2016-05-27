#include "Definition.h"

#include <stdio.h>

#ifndef DEBUG
void nothing() {}

#if (!defined(ARDUINO) && !defined(RASPBERRY) && !defined(SANUSB))
unsigned int DIGITAL_READ(unsigned int pin) { return 0; }
void DIGITAL_WRITE(unsigned int pin, unsigned int state) {}
unsigned int ANALOGIC_READ(unsigned int pin) { return 0; }
void DELAY(unsigned int timeMs) {}
#endif

#else

#if !defined(ARDUINO) && !defined(RASPBERRY) && !defined(SANUSB)
unsigned int DIGITAL_READ(unsigned int pin) {
	LOG log("> Vou fingir que o pino %u lido como HIGH\n", pin);
	return HIGH;
}

void DIGITAL_WRITE(unsigned int pin, unsigned int state) {
	LOG log("> Pino %u foi atribuido como '%u'\n", pin, state);
}

unsigned int ANALOGIC_READ(unsigned int pin) {
	LOG log("> Lendo pino '%u'\n", pin);
	return 255;
}

void DELAY(unsigned int timeMs) {

}
#endif

#endif
