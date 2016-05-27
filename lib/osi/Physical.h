#ifndef PHYSICAL
#define PHYSICAL

#ifdef __cplusplus
extern "C" {
#endif


#include "../definition/Definition.h"
#include "../util/RingBuffer.h"

/**
 * Send data to upper protocol layer
 */
typedef Function void (* Physical_putDataFunction)(byte data);

extern void Physical_init(Physical_putDataFunction function);

extern void Physical_sendByte(byte data);
extern void Physical_sendData(RingBuffer * buffer);


/*******************************
 * For device implementation
 *******************************/
extern void PhysicalDevice_init(unsigned long int speedTransmition);
extern void PhysicalDevice_sendByte(byte data);

extern void Physical_putData(byte data);

#ifdef __cplusplus
}
#endif

#endif
