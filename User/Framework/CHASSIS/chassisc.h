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
						 { 0x03, small, &hcan1 , -0.4, 0.72, -1, 0, 142.08},//{CANID,TYPE,CANLINE,LOW,UP,SIGNER} //
						 { 0x02, small, &hcan1 , -3.5, 1.6, 1, 0.35, -11.83},
						 { 0x01, small, &hcan1 , -0.8, 0.2, -1, -2 * 1.3, -40.00},//

						 { 0x04, small, &hcan1 , -0.72, 0.4, -1, 0, -138.93},
						 { 0x05, small, &hcan1 , -1.6, 3.5, 1, -0.35, 166.03},
						 { 0x06, small, &hcan1 , -0.2, 0.8, -1, 2 * 1.3, 54.56},

						 { 0x03, small, &hcan2 , -0.72, 0.4, -1, 0, -4.65},
						 { 0x02, small, &hcan2 , -3.5, 1.6, 1, 0.35, -20.39},
						 { 0x01, small, &hcan2 , -0.8, 0.2, -1, -2 * 1.3, -83.50},

						 { 0x04, small, &hcan2 , -0.4, 0.72, -1, 0, 25.75},
						 { 0x05, small, &hcan2 , -1.6, 3.5, -1, 0.35, -23.43},
						 { 0x06, small, &hcan2 , -0.2, 0.8, -1, 2 * 1.3, -17.50},
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
