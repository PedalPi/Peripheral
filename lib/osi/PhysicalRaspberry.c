#include <stdio.h>
#include <stdbool.h>

#include "../../Config.h"

#include "Physical.h"

/**************************************
 * Getting data
 **************************************/
#ifdef RASPBERRY

#include <errno.h>
#include <string.h>

#include <wiringPi.h>
#include <wiringSerial.h>

#include <time.h>

private int serialFd = -1;

PI_THREAD(getData) {
	(void) piHiPri(0);
	char data = ' ';

	INFO log("Reading channel");

	int avaliable = -1;
	struct timespec delay;
	delay.tv_sec = 0;
	delay.tv_nsec = 100;

	while(1) {
		avaliable = serialDataAvail(serialFd);

		while (avaliable > 0) {
			INFO log(" Data: %d", avaliable);
	
			data = serialGetchar(serialFd);
			#if defined(MASTER) && defined(DEBUG)
				printf(" 0x%.2x ", data);
			#endif

			Physical_putData(data);
			avaliable--;
		}
		if (nanosleep(&delay, NULL) < 0) {
			ERROR log(" nanosleep error");
		}
		
	}
}


public void PhysicalDevice_init(unsigned long int speedTransmition) {
	if (serialFd > -1) {
		INFO log("Connect another time? ¬¬'");
		return;
	}

	if (wiringPiSetup() == -1) {
		ERROR printf("Unable to start wiringPi: %s\n", strerror(errno));
		return;
	}

	pinMode(DL_CONNECTION_PIN_NEXT, OUTPUT);
	serialFd = serialOpen("/dev/ttyAMA0", speedTransmition);
	serialFlush(serialFd);

	if (serialFd < 0) {
		ERROR printf("Unable to open serial device: %s\n", strerror(errno));
	} else {
		INFO log("Communication established. Channel: %d Speed: %lu", serialFd, speedTransmition);
	}

	piThreadCreate(getData);
}

public void PhysicalDevice_sendByte(byte data) {
	serialPutchar(serialFd, data);
}
#endif
