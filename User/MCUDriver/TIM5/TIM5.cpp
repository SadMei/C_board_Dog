//
// Created by LENOVO on 2022/9/22.
//
#include "TIM5.h"
#include "drv_can.h"
#include "debugc.h"
#include "INS_task.h"
extern TIM_HandleTypeDef htim5;
//extern TIM_HandleTypeDef htim1;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
{
	if (htim == &htim5)
	{
//		Gimbal_ControlLoop();
	}
}

void TIM5_IT_Init(void)
{
	HAL_TIM_Base_Start_IT(&htim5);
	//HAL_TIM_Base_Start_IT(&htim1);
}