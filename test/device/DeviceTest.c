#include <stdio.h>
#include <stdbool.h>

#include "../minunit.h"
#include "../Tests.h"

#include "DeviceTest.h"

#include "../../lib/device/Device.h"

#include "../../impl/Knob.h"
#include "../../impl/MomentaryButton.h"

#include "../DataTransmitionUtil.h"

/**************************************
 * Tests
 **************************************/
static void onConnect() {
	LOG log(" Device connected!");
}

static char * deviceOnConnectEvent() {
	Address address = 2;

	Device device;
	Knob knob1;

	Knob_init(&knob1, "Knob 1", 1);

	Device_init(&device, "http://SrMouraSilva.github.io/", "Test");
	device.addActuator(&device, (ActuatorImpl *) &knob1.actuator);

	device.setOnConnectListener(&onConnect);

	SET_ADDRESS_TO(address);

	return 0;
}


static char * controlAssignmentResponse() {
	Address address = 2;
	unsigned char indexKnob1 = 0;
	unsigned char indexKnob2 = 1;
	unsigned char indexButton = 2;

	/*                    MIN    MAX  CURRENT */
	float valuesKnob1[] = { 0,   100,     50};
	float valuesKnob2[] = {10, 10000,    555};
	bool buttonValue = true;

	Device device;
	Knob knob1;
	Knob knob2;
	MomentaryButton button;

	Knob_init(&knob1, "Knob 1", 1);
	Knob_init(&knob2, "Knob 2", 2);
	MomentaryButton_init(&button, "Button On/OFF", 3);

	Device_init(&device, "http://SrMouraSilva.github.io/", "Test");
	device.addActuator(&device, (ActuatorImpl *) &knob1.actuator);
	device.addActuator(&device, (ActuatorImpl *) &knob2.actuator);
	device.addActuator(&device, (ActuatorImpl *) &button.actuator);

	device.setOnConnectListener(&onConnect);

	SET_ADDRESS_TO(address);
	RANGE_ASSIGNMENT(address, indexKnob1, valuesKnob1[0], valuesKnob1[1], valuesKnob1[2]);
	RANGE_ASSIGNMENT(address, indexKnob2, valuesKnob2[0], valuesKnob2[1], valuesKnob2[2]);
	BUTTON_ASSIGNMENT(address, indexButton, buttonValue);


	mu_assert("Knob 1 min", knob1.actuator.min == valuesKnob1[0]);
	mu_assert("Knob 1 max", knob1.actuator.max == valuesKnob1[1]);
	mu_assert("Knob 1 current", knob1.actuator.current == valuesKnob1[2]);

	mu_assert("Knob 2 min", knob2.actuator.min == valuesKnob2[0]);
	mu_assert("Knob 2 max", knob2.actuator.max == valuesKnob2[1]);
	mu_assert("Knob 2 current (ESCAPE problem)", knob2.actuator.current == valuesKnob2[2]);

	mu_assert("Button value", button.actuator.state == buttonValue);

#ifdef DEBUG
	printf("\n");
	printf("\n");
	Knob_print(&knob1);
	printf("\n");
	Knob_print(&knob2);
	printf("\n");
	MomentaryButton_print(&button);
	printf("\n");
#endif
	return 0;
}


static char * controlAssignmentWrongResponse() {
	Address address = 2;
	unsigned char indexKnob1 = 0;
	unsigned char indexButton = 1;

	float valuesKnob1[] = {0, 100, 50};
	bool buttonValue = true;

	Device device;
	Knob knob;
	MomentaryButton button;

	Knob_init(&knob, "Knob 1", 1);
	MomentaryButton_init(&button, "Button On/OFF", 2);

	Device_init(&device, "http://SrMouraSilva.github.io/", "Test");
	device.addActuator(&device, (ActuatorImpl *) &knob.actuator);
	device.addActuator(&device, (ActuatorImpl *) &button.actuator);

	SET_ADDRESS_TO(address);
	RANGE_ASSIGNMENT(address, /* --> */ indexButton, valuesKnob1[0], valuesKnob1[1], valuesKnob1[2]);
	BUTTON_ASSIGNMENT(address, /* --> */ indexKnob1, buttonValue);


	mu_assert("Knob 1 min", knob.actuator.min == 0);
	mu_assert("Knob 1 max", knob.actuator.max == 0);
	mu_assert("Knob 1 current", knob.actuator.current == 0);

	mu_assert("Button value", button.actuator.state == false);

#ifdef DEBUG
	printf("\n  ");
	Knob_print(&knob);
	printf("\n  ");
	MomentaryButton_print(&button);
	printf("\n");
#endif

	return 0;
}


static char * actuatorsDataResponse() {
	Address address = 2;

	Device device;
	Knob knob1;
	Knob knob2;
	MomentaryButton button;

	Knob_init(&knob1, "Knob 1", 1);
	Knob_init(&knob2, "Knob 2", 2);
	MomentaryButton_init(&button, "Button On/OFF", 3);

	Device_init(&device, "http://SrMouraSilva.github.io/", "Test");
	device.addActuator(&device, (ActuatorImpl *) &knob1.actuator);
	device.addActuator(&device, (ActuatorImpl *) &knob2.actuator);
	device.addActuator(&device, (ActuatorImpl *) &button.actuator);

	knob1.actuator.current = 10;
	knob2.actuator.current = 20;
	button.actuator.state = true;

	SET_ADDRESS_TO(address);
	ACTUATOR_DATA_REQUEST(address);

	return 0;
}


static char * deviceDescriptorResponse() {
	Address address = 2;

	Device device;
	Knob knob1;
	Knob knob2;
	MomentaryButton button;

	Knob_init(&knob1, "Knob 1", 1);
	Knob_init(&knob2, "Knob 2", 2);
	MomentaryButton_init(&button, "Button On/OFF", 3);

	Device_init(&device, "http://SrMouraSilva.github.io/", "Test");
	device.addActuator(&device, (ActuatorImpl *) &knob1.actuator);
	device.addActuator(&device, (ActuatorImpl *) &knob2.actuator);
	device.addActuator(&device, (ActuatorImpl *) &button.actuator);

	SET_ADDRESS_TO(address);
	DEVICE_DESCRIPTOR_RESPONSE(address);

	return 0;
}

static char * deviceDescriptorEmptyActuatorResponse() {
	Address address = 2;

	Device device;
	Device_init(&device, "http://SrMouraSilva.github.io/", "Test");

	SET_ADDRESS_TO(address);
	DEVICE_DESCRIPTOR_RESPONSE(address);

	return 0;
}

char * test_Device() {
	HEADER("deviceOnConnectEvent");
	mu_run_test(deviceOnConnectEvent);

	HEADER("controlAssignmentResponse");
	mu_run_test(controlAssignmentResponse);

	HEADER("controlAssignmentWrongResponse");
	mu_run_test(controlAssignmentWrongResponse);

	HEADER("actuatorsDataResponse");
	mu_run_test(actuatorsDataResponse);

	HEADER("deviceDescriptorResponse");
	mu_run_test(deviceDescriptorResponse);

	HEADER("deviceDescriptorNullResponse");
	mu_run_test(deviceDescriptorEmptyActuatorResponse);

    return 0;
}
