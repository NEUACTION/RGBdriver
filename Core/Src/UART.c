#include "main.h"
#include "UART.h"
uint8_t byte_a[10];
uint8_t byte_b[10];
uint8_t count = 10;
uint8_t flag_1ms = 0;
//volatile uint8_t rush=0;
extern UART_HandleTypeDef huart1;

extern volatile uint8_t wheel1_R;
extern volatile uint8_t wheel1_G;
extern volatile uint8_t wheel1_B;

extern volatile uint8_t wheel2_R;
extern volatile uint8_t wheel2_G;
extern volatile uint8_t wheel2_B;


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM16)
    {
				flag_1ms = 1;
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart->Instance == USART1)
	{
		HAL_UART_Receive_IT(&huart1, (uint8_t *)&byte_a, 10);
	}
}

void receive()
{
		//if (  byte_a[count-1] == '\n')
		if ( byte_a[count-2] == '\r' && byte_a[count-1] == '\n' && byte_a[0] == 'I' && byte_a[1] == 'O')
    {
				wheel1_R = byte_a[2];
				wheel1_G = byte_a[3];
				wheel1_B = byte_a[4];
			
				wheel2_R = byte_a[5];
				wheel2_G = byte_a[6];
				wheel2_B = byte_a[7];	
		}
				
				
		
		
}
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
  if (huart->Instance == USART1)
  {
			volatile uint32_t err = huart->ErrorCode;
    __HAL_UART_CLEAR_FLAG(huart, UART_CLEAR_OREF | UART_CLEAR_FEF |
                                UART_CLEAR_NEF  | UART_CLEAR_PEF);
    HAL_UART_Receive_IT(&huart1, (uint8_t *)&byte_a, 10);
  }
}