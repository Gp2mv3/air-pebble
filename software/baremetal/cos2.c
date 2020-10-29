/*
 * cos2.c
 *
 *  Created on: Oct 21, 2020
 *      Author: gp2mv3
 */


#include "system.h"
#include "co2.h"

#include "sleep.h"

// #define DEBUG_CO2

ALT_AVALON_I2C_DEV_t* initCO2() {
	  //pointer to instance structure
	  alt_u8 txbuffer[0x20];
	  alt_u8 rxbuffer[0x20];

	  int i;
	  ALT_AVALON_I2C_STATUS_CODE status;


	//get a pointer to the avalon i2c instance
	  ALT_AVALON_I2C_DEV_t* i2c_dev = alt_avalon_i2c_open("/dev/i2c_0");
	  if (NULL==i2c_dev)  {
#ifdef DEBUG_CO2
		  printf("Error: Cannot find /dev/i2c_0\n");
#endif
		  return NULL;
	  }

	  //set the address of the device using
	  alt_avalon_i2c_master_target_set(i2c_dev,0x5A);

	  txbuffer[0] = 0xFF;
	  txbuffer[1] = 0x11;
	  txbuffer[2] = 0xE5;
	  txbuffer[3] = 0x72;
	  txbuffer[4] = 0x8A;

	  status=alt_avalon_i2c_master_tx(i2c_dev, txbuffer, 0x5, ALT_AVALON_I2C_NO_INTERRUPTS);
#ifdef DEBUG_CO2
	  if (status!=ALT_AVALON_I2C_SUCCESS) printf("Error reset ! %d\n", status);
#endif
	  mysleep(500);


	  printRegister(i2c_dev, 0x00); // Status
	  printRegister(i2c_dev, 0x20); // Hardware ID
	  printRegister(i2c_dev, 0x21); // Hardware version

	  printRegister(i2c_dev, 0x23); // Bootloader version
	  printRegister(i2c_dev, 0x24); // App version


	  txbuffer[0] = 0xF4; // Start App
	  status=alt_avalon_i2c_master_tx(i2c_dev, txbuffer, 0x1, ALT_AVALON_I2C_NO_INTERRUPTS);
#ifdef DEBUG_CO2
	  if (status!=ALT_AVALON_I2C_SUCCESS) printf("Error writing ! %d\n", status);
#endif
	  mysleep(500);

	  // Set mode
	  txbuffer[0] = 0x01;
	  txbuffer[1] = 0x10; // Every second
	  status=alt_avalon_i2c_master_tx(i2c_dev, txbuffer, 0x2, ALT_AVALON_I2C_NO_INTERRUPTS);
#ifdef DEBUG_CO2
	  if (status!=ALT_AVALON_I2C_SUCCESS) printf("Error writing ! %d\n", status);
#endif
	  mysleep(500);

	  return i2c_dev;
}


alt_u8 printRegister(ALT_AVALON_I2C_DEV_t *i2c_dev, alt_u8 addr) {
	alt_u8 rxbuffer;

	ALT_AVALON_I2C_STATUS_CODE status=alt_avalon_i2c_master_tx_rx(i2c_dev, &addr, 0x1, &rxbuffer, 0x1, ALT_AVALON_I2C_NO_INTERRUPTS);
#ifdef DEBUG_CO2
	if (status!=ALT_AVALON_I2C_SUCCESS) printf("Error reading ! %d\n", status);
	printf("0x%02x: 0x%02x \n", addr, rxbuffer);
#endif

	return rxbuffer;
}

alt_u16 bufferToU16(alt_u8* buff) {
	return buff[0]*256 + buff[1];
}
