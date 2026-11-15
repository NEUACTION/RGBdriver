#include "main.h"
#include "UART.h"
volatile uint8_t byte_a;
uint8_t byte_b[30];
uint8_t count = 0;
volatile uint8_t rush=0;
extern UART_HandleTypeDef huart1;

extern uint8_t wheel1_R;
extern uint8_t wheel1_G;
extern uint8_t wheel1_B;

extern uint8_t wheel2_R;
extern uint8_t wheel2_G;
extern uint8_t wheel2_B;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart->Instance == USART1)
	{
	rush=1;
	HAL_UART_Receive_IT(&huart1, (uint8_t *)&byte_a, 1);
	}
}

void receive()
{
		__disable_irq();
		if(rush==0)
		{
			__enable_irq();
			return;
		}
		rush=0;
		byte_b[count]=byte_a;
		__enable_irq();
		count++;
		if ( byte_b[count-1] == '\n')
    {
      byte_b[count] = '\0';
			if(byte_b[0] == 'I' && byte_b[1] == 'O' && count >= 9)
			{
				wheel1_R = byte_b[2];
				wheel1_G = byte_b[3];
				wheel1_B = byte_b[4];
			
				wheel2_R = byte_b[5];
				wheel2_G = byte_b[6];
				wheel2_B = byte_b[7];
				
				count=0;
			}
			else
			count=0;
		}
		
}
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
  if (huart->Instance == USART1)
  {
    __HAL_UART_CLEAR_FLAG(huart, UART_CLEAR_OREF | UART_CLEAR_FEF |
                                UART_CLEAR_NEF  | UART_CLEAR_PEF);
		count = 0;
    HAL_UART_Receive_IT(&huart1, (uint8_t *)&byte_a, 1);
  }
}