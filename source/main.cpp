#include "MicroBit.h"
#include "samples/Tests.h"
MicroBit uBit;

#define P0 4 		// RING0   P0.02/AIN0
#define P1 8 		// RING1   P0.03/AIN1
#define P2 16 		// RING2   P0.04/AIN2
#define P8 1024		// GPIO1   P0.10/NFC2
#define P9 512		// GPIO2   P0.09/NFC1
#define P11 8388608	// BTN_B   P0.23
#define P13 131072	// SPI_EXT_SCK     P0.17
#define P14 2		// SPI_EXT_MISO    P0.01/XL2
#define P15 8192 	// SPI_EXT_MOSI    P0.13

int bitMasks[] = {128, 64, 32, 16, 8, 4, 2, 1};
uint32_t ledPins[] = {P0, P1, P2, P8, P9, P11, P13, P14};

// function that returns the bits for turned on leds according to input 
uint32_t sumPins(int *input, int size) {
	uint32_t result = 0;
	for (int i = 0; i < size; i++) {
		if (*input == 1) {
			result += ledPins[i];
		}
		input++;
	}
	return result;
}

void turnOn() {
	volatile uint32_t *d = (uint32_t *) 0x50000514;
        volatile uint32_t *p = (uint32_t *) 0x50000504;
	
	int input[] = {1, 1, 1, 1, 1, 1, 1, 1};
	uint32_t bits = sumPins(input, 8);

	*d = bits; 
        *p = bits;
}


void setLEDs(uint8_t value) {
	volatile uint32_t *d = (uint32_t *) 0x50000514;
        volatile uint32_t *p = (uint32_t *) 0x50000504;
	uint32_t bits = 0;
	for (int i = 0; i < 8; i++) {
		uint8_t bit = bitMasks[i] & value;
		if (bit > 0) bits += ledPins[i];
	}
	*d = bits;
	*p = bits;
}

void sleep(int milliSeconds) {
	for (int i = 0; i < milliSeconds; i++) {
		for (int j = 0; j < 64000; j++);
	}
}

void rollingCounter() {
	for (int i = 0; i < 256; i++) {
		setLEDs(i);
		sleep(118);
	}
}

int main()
{
	turnOn();
	rollingCounter();
}

