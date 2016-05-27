#ifndef CONFIG
#define CONFIG

/* Please, don't change */
#define SPEED_TRANSMITION 9600
#define ACTUATOR_MAX 6
#define DEBUG

/* Please, change! */

/** For enable this node communicates with master */
#define DL_CONNECTION_PIN_PREVIOUS  0
/** For enable next node communicates with master */
#define DL_CONNECTION_PIN_NEXT      1


/*
#define ARDUINO
#define SANUSB
*/
#define RASPBERRY

#ifdef ARDUINO
#include "Arduino.h"
#endif

#ifdef SANUSB
#include "sanusb/SanUSB1.h"
#endif

#endif
