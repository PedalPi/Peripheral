#include <stdio.h>

#include "RingBuffer.h"


void RingBuffer_clear(RingBuffer * buffer) {
	buffer->start = 0;
	buffer->end = 0;
	buffer->amountUsed = 0;
}

/**
 * Read the data value from RingBuffer and put it in target
 */
public ErrorIndex RingBuffer_read(RingBuffer * buffer, byte * target) {
	if (RingBuffer_isEmpty(buffer))
		return RING_BUFFER_ERROR;

	*target = buffer->buffer[buffer->start];

	buffer->start = (buffer->start+1) % buffer->bufferSize;
	buffer->amountUsed--;

    return RING_BUFFER_OK;
}

public ErrorIndex RingBuffer_readFloat(RingBuffer * buffer, float * target) {
	unsigned char i = 0;
	byte * targetByte = (byte *) target;
	ErrorIndex error = RING_BUFFER_OK;

	while (i < sizeof(float) && error != RING_BUFFER_ERROR) {
		error = RingBuffer_read(buffer, &targetByte[i]);
		i++;
	}

    return error;
}

public ErrorIndex RingBuffer_readString(RingBuffer * buffer, char * string) {
	unsigned int index = 0;
	unsigned char character = 'a';
	ErrorIndex error = RING_BUFFER_OK;

	while (character != '\0' && error != RING_BUFFER_ERROR) {
		error = RingBuffer_read(buffer, &character);
		string[index] = character;
		index++;
	}

    return error;
}

public ErrorIndex RingBuffer_readArray(RingBuffer * buffer, byte * array, size_t size) {
	ErrorIndex errorIndex = RING_BUFFER_OK;
	size_t i = 0;

	while (i != size && errorIndex == RING_BUFFER_OK) {
		errorIndex = RingBuffer_read(buffer, &array[i]);
		i++;
	}

	return errorIndex;
}


/**
 * Write data into RingBuffer
 */
public ErrorIndex RingBuffer_write(RingBuffer * buffer, byte data) {
	if (RingBuffer_isFull(buffer))
		return RING_BUFFER_ERROR;

	buffer->buffer[buffer->end] = data;

	buffer->end = (buffer->end+1) % buffer->bufferSize;
	buffer->amountUsed++;

	return RING_BUFFER_OK;
}

public ErrorIndex RingBuffer_writeString(RingBuffer * buffer, char * data) {
	ErrorIndex errorIndex = RING_BUFFER_OK;
	int i = 0;
	char character = ' ';

	while (character != '\0' && errorIndex == RING_BUFFER_OK) {
		character = data[i];
		errorIndex = RingBuffer_write(buffer, character);
		i++;
	}

	return errorIndex;
}

public ErrorIndex RingBuffer_writeArray(RingBuffer * buffer, byte * array, size_t size) {
	ErrorIndex errorIndex = RING_BUFFER_OK;
	size_t i = 0;

	while (i != size && errorIndex == RING_BUFFER_OK) {
		errorIndex = RingBuffer_write(buffer, array[i]);
		i++;
	}

	return errorIndex;
}

public bool RingBuffer_isEmpty(RingBuffer * buffer) {
	return buffer->amountUsed == 0;
}

public bool RingBuffer_isFull(RingBuffer * buffer) {
	return buffer->amountUsed == buffer->bufferSize;
}

#ifdef DEBUG
public void RingBuffer_print(RingBuffer * buffer) {
	byte data;
	RingBuffer_init(buffer2, buffer->amountUsed);

	while (RING_BUFFER_OK == RingBuffer_read(buffer, &data)) {
		printf("0x%.2x ", data);
		RingBuffer_write(&buffer2, data);
	}

	while (RING_BUFFER_OK == RingBuffer_read(&buffer2, &data))
		RingBuffer_write(buffer, data);
}
#endif
