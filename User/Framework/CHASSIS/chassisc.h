//
// Created by 24316 on 2025/4/15.
//
#ifndef STM32F407IGH6TR_BOARD_USER_FRAMEWORK_CHASSIS_CHASSISC_H_
#define STM32F407IGH6TR_BOARD_USER_FRAMEWORK_CHASSIS_CHASSISC_H_
#include "stm32f4xx_hal.h"
#include "drv_can.h"
class chassisC
{
 public:
	drv_canC Motors[3];//°üº¬Àà
	chassisC() : Motors
					 {
						 { 0x01, small, &hcan1 },//{CANID,TYPE,CANLINE}
						 { 0x02, small, &hcan1 },
						 { 0x03, small, &hcan1 },
					 }{};

	void Controlloop();
	void Printf_Test();
	void Init();
	uint8_t Motor_Num = 3;
 private:

};
extern chassisC Dog;
#endif //STM32F407IGH6TR_BOARD_USER_FRAMEWORK_CHASSIS_CHASSISC_H_
