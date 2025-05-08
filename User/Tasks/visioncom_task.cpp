//
// Created by mjw on 2023/1/12.
//

#include "packet.hpp"
#include "crc.h" //在这里调用会重定义出错
#include "visioncom_task.h"
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#include "INS_task.h"
#include "drv_can.h"
#include "debugc.h"
#include "main.h"
#include "usbd_cdc_if.h"
#include "usbd_cdc.h"
#include "chassisc.h"
/*
	* @name   VisionChattingLoop
	* @brief  视觉通信循环
	* @param  mode 自瞄模式
	* @retval None
*/
SendPacket send_packet;
uint8_t Buf[sizeof(SendPacket)];
void VisionChattingLoop()
{
	//这里放包
	for (int i = 0; i < 4; ++i)
	{
		send_packet.imu.quaternion[i] = INS.q[i];
	}
	for (int i = 0; i < 3; ++i)
	{
		send_packet.imu.accelerometer[i] = INS.Accel[i];
		send_packet.imu.gyroscope[i] = INS.Gyro[i];

	}
	send_packet.imu.rpy[0] = INS.Roll;
	send_packet.imu.rpy[1] = INS.Pitch;
	send_packet.imu.rpy[2] = INS.Yaw;

	for (int i = 0; i < Dog.Motor_Num; ++i)
	{
		send_packet.motorState[i].q = Dog.Motors[i].Motor_AngleRad;
		send_packet.motorState[i].dq = Dog.Motors[i].Motor_SpeedRadSec;
	}

	send_packet.tick++;
	//
	std::copy(reinterpret_cast<const uint8_t*>(&send_packet),
		reinterpret_cast<const uint8_t*>(&send_packet) + sizeof (SendPacket),Buf);
	Append_CRC16_Check_Sum(Buf, sizeof(SendPacket));
	CDC_Transmit_FS(Buf, sizeof(SendPacket));
//	usart_printf("%d\r\n",sizeof(SendPacket));
}

/*
	* @name   VisionComTask
	* @brief  视觉通信任务,1ms1次
	* @param  None
	* @retval None
*/
void VisionComTask(void const* argument)
{
	/* USER CODE BEGIN VisionComTask */
	portTickType CurrentTime;
	/* Infinite loop */
	for (;;)
	{
		CurrentTime = xTaskGetTickCount();
		VisionChattingLoop();//5ms一帧
		Dog.SafeChecker();
		vTaskDelayUntil(&CurrentTime, 2 / portTICK_RATE_MS);
	}
	/* USER CODE END VisionComTask */
}