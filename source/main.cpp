#include "MicroBit.h"
#include "samples/Tests.h"

MicroBit uBit;
/*
 * P0	RING0	P0.02/AIN0
 * P1	RING1	P0.03/AIN1	
 * P2	RING2	P0.04/AIN2
 * P8	
 * P9
 * P11
 * P13
 * P14
 * P15	
 * */

#define P0 4
#define P1 8
#define P2 16
#define P8 1024
#define P9 512
#define P11 2048	 
#define P13 131072
#define P14 2
#define P15 8192

void turnOn() {
	volatile uint32_t *d = (uint32_t *) 0x50000514;
        volatile uint32_t *p = (uint32_t *) 0x50000504;
        uint32_t pins = P0 + P1 + P2 + P8 + P9 + P11 + P13 + P14;
	*d = pins;
        *p = pins;
}

int main()
{
	turnOn();
}

