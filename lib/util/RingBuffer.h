#ifndef RING_BUFFER
#define RING_BUFFER

#ifdef __cplusplus
extern "C" {
#endif


#include <stdbool.h>

#include "../definition/Definition.h"

#define RING_BUFFER_ERROR -1
#define RING_BUFFER_OK 1

typedef struct {
    byte * buffer;
    unsigned int bufferSize;
    unsigned int start;
    unsigned int end;
    unsigned int amountUsed;
} RingBuffer;

/**
 * Inicialize a RingBuffer. They exists in your context
 */
#define RingBuffer_init(ringBuffer, lenght) byte x##_space[lenght]; RingBuffer ringBuffer = { x##_space, lenght, 0, 0, 0}

extern void RingBuffer_clear(RingBuffer * buffer);

extern ErrorIndex RingBuffer_read(RingBuffer * buffer, byte * target);
extern ErrorIndex RingBuffer_readFloat(RingBuffer * buffer, float * target);
extern ErrorIndex RingBuffer_readString(RingBuffer * buffer, char * string);
public ErrorIndex RingBuffer_readArray(RingBuffer * buffer, byte * array, size_t size);

extern ErrorIndex RingBuffer_write(RingBuffer * buffer, byte data);
extern ErrorIndex RingBuffer_writeString(RingBuffer * buffer, char * string);
extern ErrorIndex RingBuffer_writeArray(RingBuffer * buffer, byte * array, size_t size);


extern bool RingBuffer_isEmpty(RingBuffer * buffer);
extern bool RingBuffer_isFull(RingBuffer * buffer);

#ifdef DEBUG
extern void RingBuffer_print(RingBuffer * self);
#endif


#ifdef __cplusplus
}
#endif

#endif
