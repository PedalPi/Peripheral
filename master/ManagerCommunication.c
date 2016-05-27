#include <stdio.h>
#include <stdbool.h>

#include <wiringPi.h>
#include <time.h>

#include "Master.h"
#include "../lib/device/DeviceMaster.h"

static bool RECEIVED_DATA = false;

PI_THREAD(getData);

private void init(Master * master);

private void assignment();
private void requestData(Master * master);
private void findNewDevice();

static Master * master = NULL;

PI_THREAD(comunicationThread) {

	init(master);

	while (true) {
		assignment();
		requestData(master);
		findNewDevice();
	}
}

public void Master_startManagementComunication(Master * theMaster) {
	master = theMaster;
	piThreadCreate(comunicationThread);
}


/**************************************
 * Callbacks
 **************************************/
private void callbackDevice(Address address, char * uri, char * label, int totalActuators) {
	Master_registerSlave(master, address, uri, label, totalActuators);
	printf(" Device registred. Address %d - %s \n", (int) address, label);
	RECEIVED_DATA = true;
}

private void callbackActuatorDescription(byte address, int index, char type, char * label) {
	Master_registerActuator(master, address, index, (ActuatorType) type, label);
	printf(" Actuator registred. Index %d - %s \n", index, label);

	RECEIVED_DATA = true;
}

private void callbackActuatorsData(byte address, int index, int dataSize, byte * data) {
	/*
	printf("\n");
	printf("DeviceId:   %d\n", (int) address);
	printf("ActuatorId: %d\n", (int) index);
	printf("DataSize:   %d\n", dataSize);
	*/

	if (dataSize == 4) {
		float * value = (float *) data;
		//printf("Data:       %f\n", *value);
		Master_setActuatorValueFloat(master, address, index, *value);

	} else if (dataSize == 1) {
		//printf("Data:       %d\n", *data);
		Master_setActuatorValueBoolean(master, address, index, *data == 1);
	}

	RECEIVED_DATA = true;
}

/**************************************
 * Protocol
 **************************************/
private void waitTimeout();

private void init(Master * master) {
	Master_init(master);

	DeviceMaster_init();

	DeviceMaster_setDeviceDescriptorCallback(&callbackDevice);
	DeviceMaster_setActuatorDescriptorCallback(&callbackActuatorDescription);
	DeviceMaster_setActuatorDataCallback(&callbackActuatorsData);
}


private void assignment() {}

private void requestData(Master * master) {
	Slave * slave = NULL;
	int i=0;

	for (i=0; i<master->totalSlaves; i++) {
		slave = master->slaves[i];

		DeviceMaster_sendActuatorsDataRequest(slave->address);
		waitTimeout();
	}
}

private void findNewDevice() {
	DeviceMaster_sendMessageAddressRequest();
	waitTimeout();
}

/**************************************
 * Timeout
 **************************************/

/**
 * Timeout is 500 ms
 */
private void waitTimeout() {
	static struct timespec TIMEOUT;
	TIMEOUT.tv_sec = 0;
	TIMEOUT.tv_nsec = 10000000;

	int nTimes = 0;

	while (!RECEIVED_DATA && nTimes < 50) {
		if (nanosleep(&TIMEOUT, NULL) < 0)
			printf("??");

		nTimes++;
	}

	RECEIVED_DATA = false;
}