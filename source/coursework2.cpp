#include "MicroBit.h"

#define GPIO_P0 0x50000000
#define GPIO_P1 0x50000300
#define OUT 0x504
#define DIR 0x514

#define GPIO_P0_OUT (GPIO_P0 + OUT)
#define GPIO_P0_DIR (GPIO_P0 + DIR)
#define GPIO_P1_OUT (GPIO_P1 + OUT)
#define GPIO_P1_DIR (GPIO_P1 + DIR)

#define NUM_OF_COLUMNS 5
#define COL1 1<<28
#define COL2 1<<11
#define COL3 1<<31
#define COL4 1<<5 // p1
#define COL5 1<<30
#define ROW1 1<<21
#define ROW2 1<<22
#define ROW3 1<<15
#define ROW4 1<<24
#define ROW5 1<<19

#define CPU_CLOCK_SPEED 16000000
#define LED_DISPLAY_INTERVAL 0.0005

volatile uint32_t *p0_out = (uint32_t *) GPIO_P0_OUT;
volatile uint32_t *p0_dir = (uint32_t *) GPIO_P0_DIR;
volatile uint32_t *p1_out = (uint32_t *) GPIO_P1_OUT;
volatile uint32_t *p1_dir = (uint32_t *) GPIO_P1_DIR;

long ROWS[] = {ROW1, ROW2, ROW3, ROW4, ROW5};
long COLS[] = {COL1, COL2, COL3, COL4, COL5};

int SMILEY_FACE[5][5] = {
    {0, 1, 0, 1, 0},
    {0, 1, 0, 1, 0},
    {0, 0, 0, 0, 0},
    {1, 0, 0, 0, 1},
    {0, 1, 1, 1, 0},
};

int NUMBERS[10][5][5] = {
    { // 0
    {0, 1, 1, 1, 0},
    {1, 0, 0, 0, 1},
    {1, 0, 0, 0, 1},
    {1, 0, 0, 0, 1},
    {0, 1, 1, 1, 0},
    },
    { // 1
    {0, 1, 1, 0, 0},
    {1, 0, 1, 0, 0},
    {0, 0, 1, 0, 0},
    {0, 0, 1, 0, 0},
    {1, 1, 1, 1, 1},
    },
    { // 2
    {0, 1, 1, 1, 0},
    {1, 0, 0, 0, 1},
    {0, 0, 1, 1, 0},
    {0, 1, 0, 0, 0},
    {1, 1, 1, 1, 1},
    },
    { // 3
    {1, 1, 1, 1, 1},
    {0, 0, 0, 0, 1},
    {0, 0, 1, 1, 0},
    {0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1},
    },
    { // 4
    {1, 0, 0, 0, 0},
    {1, 0, 1, 0, 0},
    {1, 0, 1, 0, 0},
    {1, 1, 1, 1, 1},
    {0, 0, 1, 0, 0},
    },
    { // 5
    {1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0},
    {1, 1, 1, 1, 1},
    {0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1},
    },
    { // 6
    {1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0},
    {1, 1, 1, 1, 1},
    {1, 0, 0, 0, 1},
    {1, 1, 1, 1, 1},
    },
    { // 7
    {1, 1, 1, 1, 1},
    {0, 0, 0, 1, 0},
    {0, 0, 1, 0, 0},
    {0, 1, 0, 0, 0},
    {1, 0, 0, 0, 0},
    },
    { // 8
    {0, 1, 1, 1, 0},
    {1, 0, 0, 0, 1},
    {0, 1, 1, 1, 0},
    {1, 0, 0, 0, 1},
    {0, 1, 1, 1, 0},
    },
    { // 9
    {1, 1, 1, 1, 1},
    {1, 0, 0, 0, 1},
    {1, 1, 1, 1, 1},
    {0, 0, 0, 0, 1},
    {0, 0, 0, 0, 1},
    },
};

void sleep(float secs) {
    int count = (int) CPU_CLOCK_SPEED * secs;
    for (volatile int i = 0; i < count; i++) {
        ;
    }
}

void clearLEDs() {
    *p0_out = 0;
    *p0_dir = 0;
    *p1_out = 0;
    *p1_dir = 0;
}

void setLEDRow(int row, int array[5][5]) {
    *p0_out = ROWS[row];
    *p0_dir = ROWS[row];
    for (int i = 0; i < NUM_OF_COLUMNS; i++) {
        if (array[row][i] == 1) {
            if (i == 3) { // index 3 is col 4
                *p1_out = ROWS[row];
                *p1_dir = ROWS[row] + COLS[i];
            } else {
                *p0_dir += COLS[i];
            }
        }
    }
}

void beHappy() {
    int row = 0;
    while (true) {
        clearLEDs();
        setLEDRow(row, SMILEY_FACE);
        sleep(LED_DISPLAY_INTERVAL);
        row = (row + 1) % 5;
    }
}

void beVeryHappy() {
    NRF_TIMER1->TASKS_START = 1;
    NRF_TIMER1->BITMODE = 0;
    NRF_TIMER1->CC[0] = CPU_CLOCK_SPEED * LED_DISPLAY_INTERVAL;
    int row = 0;

    while (true) {
        if (NRF_TIMER1->EVENTS_COMPARE[0] == 1) {
            clearLEDs();
            setLEDRow(row, SMILEY_FACE);
            row = (row + 1) % 5;
            NRF_TIMER1->TASKS_CLEAR;
        }
    }
}

void showNumber(int n) {
    NRF_TIMER1->TASKS_START = 1;
    NRF_TIMER1->BITMODE = 0;
    NRF_TIMER1->CC[0] = CPU_CLOCK_SPEED * LED_DISPLAY_INTERVAL;
    int row = 0;

    while (true) {
        if (NRF_TIMER1->EVENTS_COMPARE[0] == 1) {
            clearLEDs();
            setLEDRow(row, NUMBERS[n]);
            row = (row + 1) % 5;
            NRF_TIMER1->TASKS_CLEAR;
        }
    }
}

int main() {
    beVeryHappy();
}