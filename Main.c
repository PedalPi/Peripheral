#include <stdio.h>
#include <stdbool.h>

#include "lib/osi/Protocol.h"
#include "lib/osi/ProtocolMaster.h"

#include "impl/Knob.h"
#include "impl/MomentaryButton.h"
#include "impl/TwoStateButton.h"

#include "lib/device/Device.h"
#include "lib/device/DeviceMaster.h"
#include "test/Tests.h"

#include "master/Master.h"

static Master master;


static void callbackDevice(Address address, char * uri, char * label, int totalActuators) {
	printf("\n");
	printf("Address:        %d\n", address);
	printf("Uri:            %s\n", uri);
	printf("Label:          %s\n", label);
	printf("TotalActuators: %d\n", totalActuators);

	Master_registerSlave(&master, address, uri, label, totalActuators);
}

static void callbackActuatorDescription(byte address, int index, char type, char * label) {
	printf(" - Actuator %d: %c - %s\n", index, type, label);

	Master_registerActuator(&master, address, index, (ActuatorType) type, label);
}

static void callbackActuatorsData(byte address, int index, int dataSize, byte * data) {
	printf("\n");
	printf("DeviceId:   %d\n", (int) address);
	printf("ActuatorId: %d\n", (int) index);
	printf("DataSize:   %d\n", dataSize);

	if (dataSize == 4) {
		float * value = (float *) data;
		printf("Data:       %f\n", *value);
		Master_setActuatorValueFloat(&master, address, index, *value);

	} else if (dataSize == 1) {
		printf("Data:       %d\n", *data);
		Master_setActuatorValueBoolean(&master, address, index, *data == 1);
	}
}


static void assign();

int main() {
	bool connected = false;

	/*executeTests();*/
	printf("======================================\n");
	while (1==1) {
		printf("\n-----------------------------------\n");
		printf(" Select a option: \n");
		printf("-----------------------------------\n");
		printf("\n");
		printf(" 1 - init() \n");
		printf(" 2 - Connect new device (if possible) \n");
		printf(" 3 - Show info \n");
		printf(" 4 - Assign \n");
		printf(" 5 - Request data \n");
		printf(" 6 - Show device info \n");
		printf(" 9 - Bye \n");

		/*printf(" 4 - DeviceMaster_sendMessageAddressRequest() \n");
		printf(" 5 - DeviceMaster_sendControlAssigmentRequest() \n");
		printf(" 6 - DeviceMaster_sendActuatorsDataRequest() \n");
		printf(" 7 - Protocol_sendDeviceDescriptorRequest() \n");*/

		printf("\nOption: ");
		fflush(stdin);
		char c = getchar();
		fflush(stdin);

		if (c == '1') {
			//Master_startManagementComunication(&master);

			/**/
			Master_init(&master);
			DeviceMaster_init();
			DeviceMaster_setDeviceDescriptorCallback(&callbackDevice);
			DeviceMaster_setActuatorDescriptorCallback(&callbackActuatorDescription);
			DeviceMaster_setActuatorDataCallback(&callbackActuatorsData);
			/**/
			connected = true;
			/*Master_registerSlave(&master, 2, "http://teste", "Simple Test", 2);
			Master_registerActuator(&master, 2, 0, ACTUATOR_TYPE_RANGE, "Range");
			Master_registerActuator(&master, 2, 1, ACTUATOR_TYPE_BUTTON, "Button");
			*/

		} else if (c == '9') {
			printf("Bye\n");
			break;
			/*Master_clear();*/

		} else {
			if (!connected) {
				printf("NOT CONNECTED! \n");
				continue;
			}

			if (c == '2')
				DeviceMaster_sendMessageAddressRequest();

			else if (c == '3')
				Master_print(&master);

			else if (c == '4')
				assign();

			else if (c == '5') {
				int address;
				printf(" Address: ");

				fflush(stdin);
				scanf("%d", &address);
				fflush(stdin);

				DeviceMaster_sendActuatorsDataRequest((Address) address);

			} else if (c == '6') {
				int address;
				printf(" Address: ");

				fflush(stdin);
				scanf("%d", &address);
				fflush(stdin);

				Protocol_sendDeviceDescriptorRequest(address);
			}
		}

		/*c = getchar();  ENTER */
	}

	return 0;
}


private void assignButton(Slave * slave, unsigned char actuatorId);
private void assignRange(Slave * slave, unsigned char actuatorId);

void assign() {
	printf("\n-----------------------------------\n");
	printf(" Assignment \n");
	printf("-----------------------------------\n");

	int addressInt = 0;
	Address address;

	fflush(stdin);
	printf(" Slave address: ");
	scanf("%d", &addressInt);
	fflush(stdin);

	address = (Address) addressInt;

	Slave * slave = Master_getSlave(&master, address);
	if (slave == NULL) {
		printf(" Invalid slave address\n ");
		return;
	}

	int actuatorId = 0;

	fflush(stdin);
	printf(" Actuator: ");
	scanf("%d", &actuatorId);
	fflush(stdin);

	if (actuatorId < 0 || actuatorId >= ACTUATOR_MAX || slave->actuators[actuatorId] == NULL) {
		printf(" Invalid actuator\n ");
		return;
	}

	printf("\n");

	SimpleActuator * actuator = slave->actuators[actuatorId];
	if (actuator->type == ACTUATOR_TYPE_BUTTON)
		assignButton(slave, actuatorId);
	else
		assignRange(slave, actuatorId);
}

private void assignButton(Slave * slave, unsigned char actuatorId) {
	printf(" - Default value: Ligado/Desligado (L/D): ");

	char stateChar = 'D';
	fflush(stdin);
	scanf("%c", &stateChar);
	fflush(stdin);

	byte on = stateChar == 'L' ? 0x01 : 0x00;
	DeviceMaster_sendControlAssigmentRequest(slave->address, actuatorId, ACTUATOR_TYPE_BUTTON, &on, sizeof(bool));
}

private void toBuffer(byte * array, byte * data, size_t size);
private float getFloat();

private void assignRange(Slave * slave, unsigned char actuatorId) {
	/*LOG log(" For Slave address %d", addressSlave);*/
	printf("  - Min: ");
	float min = getFloat();
	printf("  - Max: ");
	float max = getFloat();
	printf("  - Default: ");
	float defaultValue = getFloat();


	byte data[3*sizeof(float)];
	/* = {
		0x00, 0x00, 0x00, 0x00, / * Min 0 * /
		0x00, 0x00, 0xc8, 0x42, / * Max 100 * /
		0x00, 0x00, 0x48, 0x42  / * Current 50 * /
	};*/

	toBuffer(data, (byte *) &min, sizeof(float));
	toBuffer(&data[4], (byte *) &max, sizeof(float));
	toBuffer(&data[8], (byte *) &defaultValue, sizeof(float));

	DeviceMaster_sendControlAssigmentRequest(slave->address, actuatorId, ACTUATOR_TYPE_RANGE, data, sizeof(data));
}

private float getFloat() {
	float value = 0;
	fflush(stdin);
	scanf("%f", &value);
	fflush(stdin);

	return value;
}

private void toBuffer(byte * array, byte * data, size_t size) {
	size_t i = 0;

	while (i != size) {
		array[i] = data[i];
		i++;
	}
}
