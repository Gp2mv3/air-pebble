/*
 * sleep.c
 *
 *  Created on: Oct 21, 2020
 *      Author: gp2mv3
 */

#include "sleep.h"


#include "sys/alt_alarm.h"

alt_u32 last_sleep_tick = 0;
void mysleep(alt_u32 ms) {
	if (!last_sleep_tick) {
		last_sleep_tick = alt_nticks();
	}
	alt_u32 end = last_sleep_tick + alt_ticks_per_second() * ms / 1000;
	while (alt_nticks() <= end)
		;
	last_sleep_tick = end;
}
