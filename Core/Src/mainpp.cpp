
#include "BSP.h"
//#include "usart_new.h"
#include "crc.h"
#include "packet.hpp"
#include "chassisc.h"
/**
  * @brief  The application entry point.
  * @retval int
  */

extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2;
void User_Init()
{
	DEBUGC_UartInit();
	CAN_All_Init();
	TIM5_IT_Init();

	delay_init();
	DWT_Init(168);
	Dog.Init();
	PID_initialize();
	usart_printf("Program start!\r\n");
	osKernelStart();
}

int main(void)
{
	BSP_Init();
	User_Init();
	/* USER CODE BEGIN WHILE */
	while (1)
	{
		/* USER CODE END WHILE */

		usart_printf("wrong run freertos\r\n");
		HAL_Delay(500);
		/* USER CODE BEGIN 3 */
	}
	/* USER CODE END 3 */
}