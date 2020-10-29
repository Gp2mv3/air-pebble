/*
 * "Hello World" example.
 *
 * This example prints 'Hello from Nios II' to the STDOUT stream. It runs on
 * the Nios II 'standard', 'full_featured', 'fast', and 'low_cost' example
 * designs. It runs with or without the MicroC/OS-II RTOS and requires a STDOUT
 * device in your system's hardware.
 * The memory footprint of this hosted application is ~69 kbytes by default
 * using the standard reference design.
 *
 * For a reduced footprint version of this template, and an explanation of how
 * to reduce the memory footprint for a given application, see the
 * "small_hello_world" template.
 *
 */

#include <stdio.h>


#include "leds.h"
#include "accelerometer.h"
#include "co2.h"
#include "sleep.h"


#include "system.h"
#include "sys/alt_alarm.h"

#define DEBUG_MAIN

enum State {airQuality, pomodoroWorking, pomodoroWaiting, pomodoroBreak, pomodoroEnd};


alt_u16 fetchCO2(ALT_AVALON_I2C_DEV_t* i2c_dev) {
	ALT_AVALON_I2C_STATUS_CODE status;

	uint8_t CO2State = printRegister(i2c_dev, 0x00); // Status

	//pointer to instance structure
	alt_u8 txbuffer[0x20];
	alt_u8 rxbuffer[0x20];

	if(CO2State & 0x01) {
#ifdef DEBUG_MAIN
		printf("Error in sensor \n");
#endif
		txbuffer[0] = 0xE0;
		status=alt_avalon_i2c_master_tx_rx(i2c_dev, txbuffer, 0x1, rxbuffer, 0x1, ALT_AVALON_I2C_NO_INTERRUPTS);
#ifdef DEBUG_MAIN
		if (status!=ALT_AVALON_I2C_SUCCESS) printf("Error reading ! %d\n", status);
		printf("0x%02x: 0x%02x \n", txbuffer[0], rxbuffer[0]);
#endif
	}

	if (CO2State & 0x8) { // New CO2 data !
		txbuffer[0] = 0x02;
		status=alt_avalon_i2c_master_tx_rx(i2c_dev, txbuffer, 0x1, rxbuffer, 0x8, ALT_AVALON_I2C_NO_INTERRUPTS);
#ifdef DEBUG_MAIN
		if (status!=ALT_AVALON_I2C_SUCCESS) printf("Error reading ! %d\n", status);
#endif
		alt_u16 eco2 = bufferToU16(rxbuffer);
		alt_u16 etvoc = bufferToU16(rxbuffer + 2);
		// printf("eCO2: %d; etvoc: %d \n", eco2, etvoc);

		return eco2;
	}

	return 0;
}


int main()
{
	displayOnLeds(10000);

	ALT_AVALON_I2C_DEV_t* i2c_dev = initCO2();
	initAccelerometer();

	uint16_t eco2 = 0;
	uint32_t timer = 0;
	uint32_t timerStart = 0;

	enum State state = airQuality;

	while(1) {
#ifdef DEBUG_MAIN
		printf("State: %d, Timer %d\n", state, timer/(alt_ticks_per_second()));
#endif

		enum AccelEvent eventFromAccel = accelFetchEvents();
		uint16_t tmp = fetchCO2(i2c_dev);
		if (tmp > 0) {
			eco2 = tmp;
		}
		if (state == airQuality) {
			if (eventFromAccel == accelDoubleTap) { // Start timer
				state = pomodoroWorking;
				timerStart = alt_nticks();
			}

			displayOnLeds(eco2);
		} else { // In pomodoro
			if (eventFromAccel == accelSingleTap) {
				displayOnLeds(eco2);
				mysleep(3000); // Displays air quality for 3 seconds
			}

			timer = alt_nticks() - timerStart;

			if (state == pomodoroWorking) {
				blinkOnLeds(1); // Red

				if (eventFromAccel == accelDoubleTap) state = airQuality; // Timer canceled
				else if (timer >= 25*60*alt_ticks_per_second()) state = pomodoroWaiting; // Need to start break
			} else if (state == pomodoroWaiting) {

				blinkOnLeds(2); // Yellow

				if (eventFromAccel == accelDoubleTap) {
					state = pomodoroBreak; // User starts break
					timerStart = alt_nticks(); // Reinit counter for break
				}
			} else if (state == pomodoroBreak) {

				blinkOnLeds(4); // Green

				if (eventFromAccel == accelDoubleTap) state = airQuality; // Timer canceled
				else if (timer >= 5*60*alt_ticks_per_second()) state = pomodoroEnd; // Break is ended
			} else if (state == pomodoroEnd) {
				clearLeds();

				if (eventFromAccel == accelSingleTap || timer >= 10*60*alt_ticks_per_second()) state = airQuality; // Go back to air quality
			}
		}

		mysleep(100); // Wait 100ms
	}

	return 0;
}
