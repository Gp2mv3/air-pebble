/*
 * leds.c
 *
 *  Created on: Oct 21, 2020
 *      Author: gp2mv3
 */

#include "leds.h"


#include "system.h"
#include "altera_avalon_pio_regs.h"
#include "sleep.h"

alt_u32 LEDS = 0;

void displayOnLeds(alt_u32 value) {
	if(value < 1100) LEDS = 4;
	else if(value < 3000) LEDS = 2;
	else LEDS = 1;

	IOWR_ALTERA_AVALON_PIO_SET_BITS(PIO_1_BASE, LEDS);
}

// bit 0 = R, bit 1 = Y, bit 2 = G
void blinkOnLeds(alt_u8 value) {
	IOWR_ALTERA_AVALON_PIO_SET_BITS(PIO_1_BASE, value);
	mysleep(500);

	IOWR_ALTERA_AVALON_PIO_SET_BITS(PIO_1_BASE, 0);
	mysleep(500);

	IOWR_ALTERA_AVALON_PIO_SET_BITS(PIO_1_BASE, LEDS);
}

void clearLeds() {
	LEDS = 0;
	IOWR_ALTERA_AVALON_PIO_SET_BITS(PIO_1_BASE, LEDS);
}
