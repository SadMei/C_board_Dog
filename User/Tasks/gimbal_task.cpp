//
// Created by mjw on 2022/9/19.
//
//
#include "main.h"
#include "gimbal_task.h"
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#include "debugc.h"
#include "iwdgc.h"
#include "chassisc.h"


void GimbalControlTask(void const* argument)
{
	/* USER CODE BEGIN GimbalControlTask */
	portTickType CurrentTime;
	/* Infinite loop */
	for (;;)
	{
		CurrentTime = xTaskGetTickCount();
		//º”»ÎµÙœﬂºÏ≤‚
		Dog.SafeBuilder();

		vTaskDelayUntil(&CurrentTime, 10 / portTICK_RATE_MS);
	}
	/* USER CODE END GimbalControlTask */
}
