#ifndef DEFINITION
#define DEFINITION

#ifdef __cplusplus
extern "C" {
#endif



#include <stdio.h>
#include "../../Config.h"

#if defined(ARDUINO) || defined(SANUSB)
#define SLAVE
#else
#define MASTER
#endif

#ifdef SLAVE
#define MAX_SIZE_MESSAGE 255
#else
#define MAX_SIZE_MESSAGE 2048
#endif

#ifdef ARDUINO
#include "Arduino.h"
#endif

#ifdef DEBUG
#define log printf
#define LOG   printf("\n\x1B[33m[LOG  ]\033[0m ");
#define ERROR printf("\n\x1B[31m[ERROR]\033[0m ");
#define INFO  printf("\n\x1B[36m[INFO ]\033[0m ");
#else
extern void nothing();
#define log(...) nothing()
#define LOG ;
#define ERROR ;
#define INFO  ;
#endif

/* Arduino */
#ifndef ARDUINO
#define HIGH 1
#define LOW 0
#endif

extern unsigned int DIGITAL_READ(unsigned int pin);
extern void DIGITAL_WRITE(unsigned int pin, unsigned int status);
extern unsigned int ANALOGIC_READ(unsigned int pin);
extern void DELAY(unsigned int timeMs);
/* OOP */
#define public
#define private static
#define abstract
#define Object void
#define Function

/* Auxiliar definitions */
#ifndef ARDUINO
#define byte unsigned char
#endif
#define Pin unsigned int /* SanUSB and Arduino uses int */

#define ErrorIndex int



#ifdef __cplusplus
}
#endif

#endif
