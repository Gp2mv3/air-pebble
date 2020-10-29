/*
 * co2.h
 *
 *  Created on: Oct 21, 2020
 *      Author: gp2mv3
 */

#ifndef CO2_H_
#define CO2_H_

#include "altera_avalon_i2c.h"
#include "alt_types.h"

ALT_AVALON_I2C_DEV_t* initCO2(void);

alt_u8 printRegister(ALT_AVALON_I2C_DEV_t *i2c_dev, alt_u8 addr);
alt_u16 bufferToU16(alt_u8* buff);


#endif /* CO2_H_ */
