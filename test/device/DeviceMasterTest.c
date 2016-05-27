#include <stdio.h>
#include <stdbool.h>

#include "../minunit.h"

#include "DeviceMasterTest.h"

#include "../DataTransmitionUtil.h"

#include "../../lib/device/Device.h"
#include "../../lib/device/DeviceMaster.h"

#include "../../impl/Knob.h"
#include "../../impl/MomentaryButton.h"

static void callbackDeviceDescription(byte address, char * uri, char * label, int totalActuators) {
	printf("\n");
	printf("Address:        %d\n", address);
	printf("Uri:            %s\n", uri);
	printf("Label:          %s\n", label);
	printf("TotalActuators: %d\n", totalActuators);

	printf("\nActuators:\n");
}

static void callbackActuatorDescription(byte address, int index, char type, char * label) {
	printf(" - Actuator %d: %c - %s\n", index, type, label);
}


static void callbackActuatorsData(byte deviceId, int actuatorId, int dataSize, byte * data) {
	printf("\n");
	printf("DeviceId:   %d\n", deviceId);
	printf("ActuatorId: %d\n", actuatorId);
	printf("DataSize:   %d\n", dataSize);
	if (dataSize == 4) {
		float * value = (float *) data;
		printf("Data:       %f\n", *value);
	} else if (dataSize == 1)
		printf("Data:       %d\n", *data);
}

/**************************************
 * Tests
 **************************************/
static char * complete() {
	Address addressSlave = 2;

	INFO log(" DM - INIT ");
	DeviceMaster_init();
	DeviceMaster_setDeviceDescriptorCallback(&callbackDeviceDescription);
	DeviceMaster_setActuatorDescriptorCallback(&callbackActuatorDescription);
	DeviceMaster_setActuatorDataCallback(&callbackActuatorsData);


	INFO log(" DM - Address attribuition ");
	INFO log(" DM - (Master set address and request description) ");
	DeviceMaster_sendMessageAddressRequest();

	INFO log(" DM - Device informs \"I need address\"");
	putDelimiter();
	DL_HEADER(1, DL_ADDRESS_MASTER, DL_TYPE_ADDRESS_REQUEST, DL_DATA_NULL);
	putDelimiter();
	DataLinkMessage_processReceivedData();

	INFO log(" DM - Device returns description ");
	putDelimiter();
	DL_HEADER(1, DL_ADDRESS_MASTER, DL_TYPE_PROTOCOL, PROTOCOL_DEVICE_DESCRIPTOR);

	putString("http://SrMouraSilva.github.io/");
	putString("Test");

	putByte((byte) 3 /* Actuators */);
	putByte((byte) 'R');
	putString("KnobRange 1");
	putByte((byte) 'R');
	putString("PulseKnobRange 2");
	putByte((byte) 'B');
	putString("Simple button on/off");
	putDelimiter();
	DataLinkMessage_processReceivedData();

	INFO log(" DM - Master attribuition for slave ");
	byte data[] = {
		0x00, 0x00, 0x00, 0x00, /* Min 0 */
		0x00, 0x00, 0xc8, 0x42, /* Max 100 */
		0x00, 0x00, 0x48, 0x42  /* Current 50 */
	};

	DeviceMaster_sendControlAssigmentRequest(addressSlave, 0 /*Actuator index*/, ACTUATOR_TYPE_RANGE, data, sizeof(data));

	INFO log(" DM - Master request values ");
	DeviceMaster_sendActuatorsDataRequest(addressSlave);

	INFO log(" DM - Device returns values ");
	putDelimiter();
	DL_HEADER(1, DL_ADDRESS_MASTER, DL_TYPE_PROTOCOL, PROTOCOL_ACTUATORS_DATA);
	putByte((byte) sizeof(float));
	putFloat(20);
	putByte((byte) sizeof(float));
	putFloat(15.9);
	putByte((byte) sizeof(bool));
	putByte(true);
	putDelimiter();
	DataLinkMessage_processReceivedData();

	return 0;
}


char * test_DeviceMaster() {
	mu_run_test(complete);

	return 0;
}
