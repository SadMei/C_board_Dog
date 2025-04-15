//
// Created by mjw on 2023/1/12.
//

#include "packet.hpp"
#include "crc.h" //��������û��ض������
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
int32_t id = 0;

/*
	* @name   VisionChattingLoop
	* @brief  �Ӿ�ͨ��ѭ��
	* @param  mode ����ģʽ
	* @retval None
*/
SendPacket send_packet;
uint8_t Buf[sizeof(SendPacket)];
uint8_t New_Buf[sizeof(Buf)+1];
void VisionChattingLoop()
{
	//

	//
	std::copy(reinterpret_cast<const uint8_t*>(&send_packet),
		reinterpret_cast<const uint8_t*>(&send_packet) + sizeof (SendPacket),Buf);
	Append_CRC16_Check_Sum(Buf, sizeof(SendPacket));
	memcpy(New_Buf, Buf, sizeof(SendPacket));
	New_Buf[sizeof(SendPacket)] = '\n';
	CDC_Transmit_FS(New_Buf, sizeof(Buf)+1);
	id++;
	//����ŷ��͵ĺ���
}

/*
	* @name   VisionComTask
	* @brief  �Ӿ�ͨ������,1ms1��
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
		for (int i = 0; i < Dog.Motor_Num; ++i)
		{
			Dog.Motors[i].GetTorques();
			Dog.Motors[i].Get_EncoderEstimates();
		}
		CurrentTime = xTaskGetTickCount();
		VisionChattingLoop();
		vTaskDelayUntil(&CurrentTime, 10 / portTICK_RATE_MS);
	}
	/* USER CODE END VisionComTask */
}