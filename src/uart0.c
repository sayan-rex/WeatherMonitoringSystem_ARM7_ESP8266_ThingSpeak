#include <lpc214x.h>
#include "uart0.h"

void UART0_Init() {
    PINSEL0 |= 0x00000005; // P0.0 = TXD, P0.1 = RXD
    U0LCR = 0x83;          // 8-bit, 1 stop bit, no parity, DLAB=1
    U0DLM = 0x00;
    U0DLL = 97;            // 9600 baud rate at 15MHz PCLK
    U0LCR = 0x03;          // DLAB = 0
}

void UART0_TxChar(char ch) {
    while (!(U0LSR & 0x20));
    U0THR = ch;
}

void UART0_SendString(const char* str) {
    while (*str) {
        UART0_TxChar(*str++);
    }
}
