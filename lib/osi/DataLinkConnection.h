#ifndef DATALINK_CONNECTION
#define DATALINK_CONNECTION

#ifdef __cplusplus
extern "C" {
#endif



#include <stdbool.h>

#include "../definition/Definition.h"

typedef struct {
	/** Permite this node communicate with master */
	Pin nodeInput;
	/** Permite the next node communicate with master */
	Pin nodeOutput;
} DLConnection;

extern void DLConnection_init();
extern void DLConnection_connect();
extern bool DLConnection_isEstablished();



#ifdef __cplusplus
}
#endif

#endif
