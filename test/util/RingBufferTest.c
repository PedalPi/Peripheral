#include <stdio.h>

#include "../minunit.h"
#include "../../lib/util/RingBuffer.h"

#include "RingBufferTest.h"

static char * init() {
	RingBuffer_init(buffer, 5);

	/*mu_assert("Buffer initialized is not null", &buffer != NULL);*/
	mu_assert("Buffer data is not null", buffer.buffer != NULL);

    return 0;
}

static char * isEmpty() {
	byte test;
	RingBuffer_init(buffer, 5);

	mu_assert("Is empty", RingBuffer_isEmpty(&buffer));
	RingBuffer_write(&buffer, 0);

	mu_assert("Is not empty", !RingBuffer_isEmpty(&buffer));

	RingBuffer_write(&buffer, 0);
	RingBuffer_write(&buffer, 0);
	RingBuffer_write(&buffer, 0);
	RingBuffer_write(&buffer, 0);

	mu_assert("(Full) Is not empty", !RingBuffer_isEmpty(&buffer));

	RingBuffer_read(&buffer, &test);
	RingBuffer_read(&buffer, &test);
	RingBuffer_read(&buffer, &test);
	RingBuffer_read(&buffer, &test);
	RingBuffer_read(&buffer, &test);
	mu_assert("(Cleaned) Is empty", RingBuffer_isEmpty(&buffer));

	RingBuffer_write(&buffer, 0);
	RingBuffer_write(&buffer, 0);
	mu_assert("(Repopuled) Is no empty", !RingBuffer_isEmpty(&buffer));

	return 0;
}

static char * isFull() {
	byte test;
	RingBuffer_init(buffer, 5);

	mu_assert("Is not full", !RingBuffer_isFull(&buffer));
	RingBuffer_write(&buffer, 0);
	RingBuffer_write(&buffer, 0);
	RingBuffer_write(&buffer, 0);
	RingBuffer_write(&buffer, 0);

	mu_assert("Is not full (almost)", !RingBuffer_isFull(&buffer));

	RingBuffer_write(&buffer, 0);
	mu_assert("Is full", RingBuffer_isFull(&buffer));

	RingBuffer_read(&buffer, &test);

	mu_assert("(Cleaning) Is not full", !RingBuffer_isFull(&buffer));

	RingBuffer_write(&buffer, 0);

	mu_assert("(Again) Is full", RingBuffer_isFull(&buffer));

	return 0;
}

static char * writeAndRead() {
	byte test;
	unsigned char i = 0;
	RingBuffer_init(buffer, 5);

	for (i = 0; i < 20; ++i) {
		mu_assert("Is empty", RingBuffer_isEmpty(&buffer));
		RingBuffer_write(&buffer, i);

		mu_assert("Is not empty", !RingBuffer_isEmpty(&buffer));
		RingBuffer_read(&buffer, &test);

		mu_assert("Data armazened is correctly armazened", i == test);
	}

	mu_assert("Is empty", RingBuffer_isEmpty(&buffer));

	return 0;
}

static char * writeWhenFull() {
	RingBuffer_init(buffer, 2);

	mu_assert("Ok", RingBuffer_write(&buffer, 1) == RING_BUFFER_OK);
	mu_assert("Ok", RingBuffer_write(&buffer, 2) == RING_BUFFER_OK);

	mu_assert("Not ok", RingBuffer_write(&buffer, 3) == RING_BUFFER_ERROR);

	return 0;
}

static char * readWhenEmpty() {
	byte test;
	RingBuffer_init(buffer, 2);

	mu_assert("Read empty", RingBuffer_read(&buffer, &test) == RING_BUFFER_ERROR);

	RingBuffer_write(&buffer, 1);
	mu_assert("Read not Empty", RingBuffer_read(&buffer, &test) == RING_BUFFER_OK);

	return 0;
}

static char * writeString() {
	RingBuffer_init(buffer, 5);
	byte character = '\0';

	mu_assert("WriteString succesfull", RingBuffer_writeString(&buffer, "Ball") == RING_BUFFER_OK);
	mu_assert("RingBuffer is complete", RingBuffer_isFull(&buffer));

	mu_assert("B", RingBuffer_read(&buffer, &character) == RING_BUFFER_OK);
	mu_assert("B", 'B' == character);

	mu_assert("a", RingBuffer_read(&buffer, &character) == RING_BUFFER_OK);
	mu_assert("a", 'a' == character);

	mu_assert("l", RingBuffer_read(&buffer, &character) == RING_BUFFER_OK);
	mu_assert("l", 'l' == character);

	mu_assert("l", RingBuffer_read(&buffer, &character) == RING_BUFFER_OK);
	mu_assert("l", 'l' == character);

	mu_assert("End string", RingBuffer_read(&buffer, &character) == RING_BUFFER_OK);
	mu_assert("End string", '\0' == character);

	mu_assert("RingBuffer is empty", RingBuffer_isEmpty(&buffer));

	return 0;
}

static char * writeArray() {
	RingBuffer_init(buffer, 5);
	byte character = '\0';

	mu_assert("WriteArray succesfull", RingBuffer_writeArray(&buffer, (byte *) "Ball", 5) == RING_BUFFER_OK);
	mu_assert("RingBuffer is complete", RingBuffer_isFull(&buffer));

	mu_assert("B", RingBuffer_read(&buffer, &character) == RING_BUFFER_OK);
	mu_assert("B", 'B' == character);

	mu_assert("a", RingBuffer_read(&buffer, &character) == RING_BUFFER_OK);
	mu_assert("a", 'a' == character);

	mu_assert("l", RingBuffer_read(&buffer, &character) == RING_BUFFER_OK);
	mu_assert("l", 'l' == character);

	mu_assert("l", RingBuffer_read(&buffer, &character) == RING_BUFFER_OK);
	mu_assert("l", 'l' == character);

	mu_assert("End string", RingBuffer_read(&buffer, &character) == RING_BUFFER_OK);
	mu_assert("End string", '\0' == character);

	mu_assert("RingBuffer is empty", RingBuffer_isEmpty(&buffer));

	return 0;
}

static char * readFloat() {
	float originalValue = 122.25;
	float result = 0;
	RingBuffer_init(buffer, sizeof(float));

	mu_assert("WriteFloat", RingBuffer_writeArray(&buffer, (byte *) &originalValue, sizeof(float)) == RING_BUFFER_OK);
	mu_assert("RingBuffer is full", RingBuffer_isFull(&buffer));

	mu_assert("ReadedFloat", RingBuffer_readFloat(&buffer, &result) == RING_BUFFER_OK);
	mu_assert("RingBuffer is empty", RingBuffer_isEmpty(&buffer));

	mu_assert("Value loaded is equals", originalValue == result);

	return 0;
}

char * test_RingBuffer() {
    mu_run_test(init);
    mu_run_test(isEmpty);
    mu_run_test(isFull);
    mu_run_test(writeAndRead);
    mu_run_test(writeWhenFull);
    mu_run_test(readWhenEmpty);
    mu_run_test(writeString);
    mu_run_test(writeArray);
    mu_run_test(readFloat);

    return 0;
}
