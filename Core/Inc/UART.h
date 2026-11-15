#ifndef _UART_H_
#define _UART_H_
#include "main.h"
extern volatile uint8_t byte_a;
extern uint8_t byte_b[30];
extern uint8_t count;
void receive();
extern volatile uint8_t rush;
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
#endif