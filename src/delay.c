#include <lpc214x.h>
#include "delay.h"

void delay_init() {
    T0PR = 15 - 1; // PCLK = 15MHz -> 1us per tick
    T0TCR = 2;     // Reset Timer
    T0TCR = 1;     // Enable Timer
}

void delay_us(unsigned int us) {
    T0TC = 0;
    while (T0TC < us);
}

void delay_ms(unsigned int ms) {
    while (ms--) delay_us(1000);
}
