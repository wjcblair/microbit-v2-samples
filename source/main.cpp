#include "MicroBit.h"
#include "samples/Tests.h"
MicroBit uBit;

#define P0 4 		// RING0   		P0.02/AIN0
#define P1 8 		// RING1   		P0.03/AIN1
#define P2 16 		// RING2   		P0.04/AIN2
#define P8 1024		// GPIO1   		P0.10/NFC2
#define P9 512		// GPIO2   		P0.09/NFC1
#define P11 8388608	// BTN_B   		P0.23
#define P13 131072	// SPI_EXT_SCK     	P0.17 
#define P14 2		// SPI_EXT_MISO    	P0.01/XL2 #define GPIO_

#define DIR 0x50000514
#define OUT 0x50000504
#define IN 0x50000510
#define GPIO_CNF_13 0x50000734

int bitMasks[] = {128, 64, 32, 16, 8, 4, 2, 1};
uint32_t ledPins[] = {P0, P1, P2, P8, P9, P11, P13, P14};

/**
 * turns on all leds
 */
void turnOn() {
	volatile uint32_t *d = (uint32_t *) DIR;
        volatile uint32_t *p = (uint32_t *) OUT;
	
	uint32_t bits = 0;
	for (int i = 0; i < 8; i++) {
		bits += ledPins[i];
	}

	*d = bits; 
        *p = bits;
}

/**
 * sets leds to display value as an 8bit binary number
 */
void setLEDs(uint8_t value) {
	volatile uint32_t *d = (uint32_t *) DIR; 
	volatile uint32_t *p = (uint32_t *) OUT;
	*d = 0;
	*p = 0;
	uint32_t bits = 0;
	for (int i = 0; i < 8; i++) {
		uint8_t bit = bitMasks[i] & value;
		if (bit > 0) bits += ledPins[i];
	}
	*d = bits;
	*p = bits;
}

/**
 * implements a 'sleep' in milliseconds
 */
void sleep(int ms) {
	for (int i = 0; i < ms; i++) {
		for (volatile int j = 0; j < 6400; j++) {
			;
		}
	}
}

/**
 * displays a full 8 bit binary count in approximately 30 seconds
 */
void rollingCounter() {
	for (int i = 0; i < 256; i++) {
		setLEDs(i);
		sleep(118); // 30/255 seconds per loop
	}
}

/**
 * raises x to the power of y
 */
uint8_t power(int x, int y) {
	uint8_t result = 1;
	for (int i = 0; i < y; i++) {
		result *= x;
	}
	return result;
}

/**
 * scrolls an led from start to end by step amount of leds
 */
void scroll(int start, int end, int step) {
	for (int i = start; step > 0 ? i < end : i > end; i += step) {
		uint8_t bit = power(2, i);
		sleep(170);
		setLEDs(bit);
	}
}

/**
 * scrolls a single led back and forth across the display
 */
void knightRider() {
	while (1) {
		scroll(0, 7, 1);
		scroll(7, 0, -1);	
	}
}

// doesnt work! :(
void countClicks() {
	volatile uint32_t *in = (uint32_t *) IN;
	volatile uint32_t *pin = (uint32_t *) GPIO_CNF_13;
	*pin = 0;
	
	uint32_t counter = 0;
	while (1) {
		if (((*in >> 13) == 0)) {
			counter++;
		}
		setLEDs(counter);
	}
}

int main() {
	rollingCounter();
}
