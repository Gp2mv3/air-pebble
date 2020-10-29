/*
 * accelerometer.h
 *
 *  Created on: Oct 21, 2020
 *      Author: gp2mv3
 */

#ifndef ACCELEROMETER_H_
#define ACCELEROMETER_H_

#import "alt_types.h"

enum AccelEvent {noEvent, accelSingleTap, accelDoubleTap};

alt_u32 write_to_accel(alt_u8 addr, alt_u8 data);

alt_u32 read_from_accel(alt_u8 addr, alt_u8 len, alt_u8* buffer);

alt_u32 accel_to_int(alt_u8 *buff);

alt_u8 initAccelerometer();

enum AccelEvent accelFetchEvents();

#endif /* ACCELEROMETER_H_ */
