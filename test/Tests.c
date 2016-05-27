#include <stdio.h>

#include "minunit.h"

#include "../lib/definition/Definition.h"

#include "util/RingBufferTest.h"
#include "osi/DataLinkMessageTest.h"
#include "osi/ProtocolTest.h"
#include "device/DeviceTest.h"
#include "device/DeviceMasterTest.h"

int tests_run = 0;

int execute(char * title, char * (* tests)());

void HEADER(char * data) {
	INFO;
	log("\033[40m");
	log("%s", data);
	log("\033[0m");
}

void executeTests() {
	printf(" TESTS: \n");
	execute("RingBuffer",      &test_RingBuffer);
	execute("DataLinkMessage", &test_DataLinkMessage);
	execute("Protocol",        &test_Protocol);
	execute("Device",          &test_Device);
	execute("DeviceMaster",    &test_DeviceMaster);
}

int execute(char * title, char * (* tests)()) {
	printf(" - %s", title);
	char * result = tests();
	if (result != 0) {
		ERROR log(" %s", result);
	} else {
		INFO log(" [PASSED] ");
	}

	printf("\n   Tests run: %d\n", tests_run);

	return result != 0;
}
