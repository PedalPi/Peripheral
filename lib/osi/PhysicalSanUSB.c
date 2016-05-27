#include <stdio.h>
#include <stdbool.h>

#include "../../Config.h"

#include "Physical.h"

/**************************************
 * Getting data
 **************************************/
#ifdef SANUSB
#include "sansub/SanUsb1.h"
#include "evento/InterrupcaoEventos.h"

private void onReceaveByte(void * null);

private void PhysicalDevice_init(unsigned long int speedTransmition) {
	taxa_serial(speedTransmition);

    Interrupcoes_init();
	naInterrupcao(SERIAL, &onReceaveByte, NULL);
}

private void onReceaveByte(void * null) {
    Physical_putData(le_serial());
}

private void PhysicalDevice_sendByte(byte data) {
	swputc(data);
}

#endif
