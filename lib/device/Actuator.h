#ifndef ACTUATOR
#define ACTUATOR

#ifdef __cplusplus
extern "C" {
#endif



#include "../definition/Definition.h"
#include "../util/RingBuffer.h"

#define ACTUATOR_UNINDEXED 0b11111111


typedef enum {
	ACTUATOR_TYPE_RANGE  = 0x52, /* R */
	ACTUATOR_TYPE_BUTTON = 0x42, /* B */
	ACTUATOR_TYPE_MAP    = 0x4d  /* M */
} ActuatorType;

typedef struct {
	unsigned char index;
	char * name;
	ActuatorType type;

	abstract Function ErrorIndex (* assign)(Object * self, RingBuffer * buffer);
	abstract Function size_t (* getTypeSize)();
	abstract Function void * (* getValue)(Object * self);
	abstract Function void (* print)(Object * self);
} Actuator;

/**
 * Actuator Inhehance
 */
typedef struct {
	abstract Actuator actuator;
} ActuatorImpl;


extern void Actuator_init(Actuator * actuator, char * name, ActuatorType type);
extern void ActuatorImpl_init(ActuatorImpl * actuator, char * name, ActuatorType type);



#ifdef __cplusplus
}
#endif

#endif
