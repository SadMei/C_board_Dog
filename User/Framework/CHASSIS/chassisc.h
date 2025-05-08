//
// Created by 24316 on 2025/4/15.
//
#ifndef STM32F407IGH6TR_BOARD_USER_FRAMEWORK_CHASSIS_CHASSISC_H_
#define STM32F407IGH6TR_BOARD_USER_FRAMEWORK_CHASSIS_CHASSISC_H_
#include "stm32f4xx_hal.h"
#include "drv_can.h"
#define SAFE 0
#define MOTION 1
class chassisC
{
 public:
	drv_canC Motors[12];//°üº¬Àà
	chassisC() : Motors
					 {
						 { 0x01, small, &hcan1 , 0, 0},//{CANID,TYPE,CANLINE,UP,LOW}
						 { 0x02, small, &hcan1 , 0, 0},
						 { 0x03, small, &hcan1 , 0, 0},

						 { 0x04, small, &hcan1 , 0, 0},//{CANID,TYPE,CANLINE,UP,LOW}
						 { 0x05, small, &hcan1 , 0, 0},
						 { 0x06, small, &hcan1 , 0, 0},

						 { 0x01, small, &hcan2 , 0, 0},//{CANID,TYPE,CANLINE,UP,LOW}
						 { 0x02, small, &hcan2 , 0, 0},
						 { 0x03, small, &hcan2 , 0, 0},

						 { 0x04, small, &hcan2 , 0, 0},//{CANID,TYPE,CANLINE,UP,LOW}
						 { 0x05, small, &hcan2 , 0, 0},
						 { 0x06, small, &hcan2 , 0, 0},
					 }{};

	void Controlloop();
	void Printf_Test();
	void Init();
	void SafeChecker();
	void SafeBuilder();
	uint16_t online_timer = 0;
	uint8_t is_online = 0;

	uint8_t Motor_Num = 12;
	uint8_t mode = SAFE, last_mode = SAFE, permit_communication = 0;


 private:

};
extern chassisC Dog;
#endif //STM32F407IGH6TR_BOARD_USER_FRAMEWORK_CHASSIS_CHASSISC_H_
