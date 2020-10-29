/*
 * leds.h
 *
 *  Created on: Oct 21, 2020
 *      Author: gp2mv3
 */

#ifndef LEDS_H_
#define LEDS_H_

#include "alt_types.h"

void displayOnLeds(alt_u32 value);

void blinkOnLeds(alt_u8 value);

void clearLeds();


#endif /* LEDS_H_ */
