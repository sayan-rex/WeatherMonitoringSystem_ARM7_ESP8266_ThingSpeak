#ifndef __UART0_H
#define __UART0_H

void UART0_Init(void);
void UART0_TxChar(char ch);
void UART0_SendString(const char *str);

#endif
