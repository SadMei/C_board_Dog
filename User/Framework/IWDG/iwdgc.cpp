//
// Created by mjw on 2022/12/3.
//
/* Includes ------------------------------------------------------------------*/
#include "iwdgc.h"

/* Private define-------------------------------------------------------------*/

/* Private variables----------------------------------------------------------*/

/* Private function prototypes------------------------------------------------*/
//static void FeedDog(void);
extern IWDG_HandleTypeDef hiwdg;
/* Public variables-----------------------------------------------------------*/
//MyIWDG_t MyIWDG =
//		{
//				TRUE,
//
//				FeedDog
//		};
/*
	* @name   FeedDog
	* @brief  喂狗
	* @param  None
	* @retval None
*/
void FeedDog(void)
{
	HAL_IWDG_Refresh(&hiwdg); //喂狗
}
/********************************************************
  End Of File
********************************************************/