#ifndef _UART_H_
#define _UART_H_
#include "main.h"
extern uint8_t byte_a[10];
extern uint8_t byte_b[10];
extern uint8_t count;
extern uint8_t flag_1ms;
void receive();
extern volatile uint8_t rush;
#endif