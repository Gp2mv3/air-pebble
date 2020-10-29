/*
 * accelerometer.c
 *
 *  Created on: Oct 21, 2020
 *      Author: gp2mv3
 */


#include "accelerometer.h"
#include "system.h"
#include "altera_avalon_spi.h"

#define ACCEL_READ_FLAG 	0x80
#define ACCEL_INC_ADDR_FLAG 0x40

#define SPI_ADDR SPI_0_BASE

#define DEBUG_ACCEL

alt_u32 write_to_accel(alt_u8 addr, alt_u8 data) {
	  alt_u8 write_buf[2] = {addr, data};
	  alt_u8 read;

	  int status = alt_avalon_spi_command(SPI_ADDR, 0, 2, write_buf, 0, &read, 0);
#ifdef DEBUG_ACCEL
	  printf("Status tx_rx: %d\n", status);
#endif

	  alt_u8 a = ACCEL_READ_FLAG | addr;
	  status = alt_avalon_spi_command(SPI_ADDR, 0, 1, &a, 1, &read, 0);

	  return (read != data);
}

alt_u32 read_from_accel(alt_u8 addr, alt_u8 len, alt_u8* buffer) {
	alt_u8 a = ACCEL_READ_FLAG | ACCEL_INC_ADDR_FLAG | addr;
	return alt_avalon_spi_command(SPI_ADDR, 0, 1, &a, len, buffer, 0);
}

alt_u32 accel_to_int(alt_u8 *buff)
{
	return ((*buff & 0x0F)<<8) + (*(buff +1 )& 0xFF);
}

alt_u8 initAccelerometer() {
	alt_u8 addr = 0xB5;
	read_from_accel(0x0F, 1, &addr);
	if(addr != 0x33) {
#ifdef DEBUG_ACCEL
		printf("Who am I is wrong: %02x!\n", addr);
#endif
		return 1;
	}

	// Enable temp sensor
	if(write_to_accel(0x1F, 0xC0)) return 1; // TEMP

	if(write_to_accel(0x20, 0x77)) return 2; // REG1
	if(write_to_accel(0x21, 0x08)) return 2; // REG2
	if(write_to_accel(0x22, 0x80)) return 2; // REG3
	if(write_to_accel(0x23, 0x80)) return 2; // REG4
	if(write_to_accel(0x24, 0x0A)) return 2; // REG5
	if(write_to_accel(0x25, 0x00)) return 2; // REG6

	if(write_to_accel(0x30, 0x00)) return 3; // Int1_cfg
	if(write_to_accel(0x38, 0x3F)) return 3; // Click_CFG
	if(write_to_accel(0x3A, 0x88)) return 3; // Click_threshold
	if(write_to_accel(0x3B, 0x33)) return 3; // Time limit
	if(write_to_accel(0x3C, 0x15)) return 3; // Time latency
	if(write_to_accel(0x3D, 0x42)) return 3; // Time_Window

	return 0;
}

enum AccelEvent accelFetchEvents() {
	alt_u8 irq = 0;
	read_from_accel(0x31, 1, &irq);

	alt_u8 irq2 = 0;
	read_from_accel(0x39, 1, &irq2);

#ifdef DEBUG_ACCEL
	printf("IRQ %02x %02x\n", irq, irq2);
#endif

	if (irq2 & 0x10) return accelSingleTap;
	if (irq2 & 0x20) return accelDoubleTap;


	return noEvent;
}
