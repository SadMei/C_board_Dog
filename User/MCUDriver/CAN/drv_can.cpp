//
// Created by LENOVO on 2022/9/6.
//
//云台CAN通信反馈信息处理
#include <cstring>
#include "drv_can.h"
#include "debugc.h"
#include "visioncom_task.h"
#include "chassisc.h"
#include "string.h"
//CAN_TxHeaderTypeDef TxMeg;
//drv_canC motor1(0x01),motor2(0x02),motor3(0x03);
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef* hcan)  //接收回调函数
{
	//回调函数,读取各个电机信息
	for (int i = 0; i < Dog.Motor_Num; ++i)
	{
		Dog.Motors[i].GetMotorMsg(hcan);
	}
}

void CAN_Filter_Init(CAN_HandleTypeDef* hcan)
{
	CAN_FilterTypeDef sFilterConfig;
	HAL_StatusTypeDef HAL_Status;
	sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK; //工作在标识符屏蔽位模式
	sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;//滤波器位宽为单个32位

	sFilterConfig.FilterIdHigh = 0X0000;
	sFilterConfig.FilterIdLow = 0X0000;
	//过滤屏蔽码
	sFilterConfig.FilterMaskIdHigh = 0X0000;
	sFilterConfig.FilterMaskIdLow = 0X0000;

	sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
	if (hcan->Instance == CAN1)
	{
		sFilterConfig.FilterBank = 0;
	}
	else if (hcan->Instance == CAN2)
	{
		sFilterConfig.FilterBank = 14;
	}
	sFilterConfig.FilterActivation = ENABLE;
	HAL_Status = HAL_CAN_ConfigFilter(hcan, &sFilterConfig);
	if (HAL_Status != HAL_OK)
	{

	}
}

void CAN_All_Init(void)
{
	CAN_Filter_Init(&hcan1);
	HAL_CAN_Start(&hcan1);
	__HAL_CAN_ENABLE_IT(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);
	HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);

	CAN_Filter_Init(&hcan2);
	HAL_CAN_Start(&hcan2);
	__HAL_CAN_ENABLE_IT(&hcan2, CAN_IT_RX_FIFO0_MSG_PENDING);
	HAL_CAN_ActivateNotification(&hcan2, CAN_IT_RX_FIFO0_MSG_PENDING);
}

//以下为瓴新电机测试用
void drv_canC::GetMotorMsg(CAN_HandleTypeDef* hcan)
{
	uint8_t recvData[8];
	CAN_RxHeaderTypeDef RxMeg;
	HAL_StatusTypeDef HAL_Status = HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxMeg, recvData);
	if (HAL_Status == HAL_OK && RxMeg.StdId >> 5 == Motor_ID && hcan == Motor_CanLine)
	{
		switch (RxMeg.StdId & 0x01F)//取低五位判断收消息的类别
		{
			case Heartbeat:
			{
				break;
			}
			case Mit_Control:
			{
//				Motor_AngleRad = ((int16_t)(recvData[1] << 8 + recvData[2])) * 25.0 / 65535.0 - 12.5;
//				Motor_SpeedRadSec = (recvData[3] << 8 + recvData[4] & 0xf0) * 130 / 4095 - 65;
//				Motor_Torque = ((recvData[4] & 0x0f) << 8 + recvData[5]) * 100 / 4095 - 50;
				break;
			}
			case Get_Torques:
			{
//				usart_printf("1");
				memcpy(&Motor_Torque,recvData+4,4);
				break;
			}
			case Get_Encoder_Estimates:
			{
				memcpy(&Motor_AngleRad,recvData,4);
				memcpy(&Motor_SpeedRadSec,recvData+4,4);
				break;
			}
		}
	}
}

//void drv_canC::MITControl()
//{
//	tx_msg.StdId = SetID(Motor_ID, Mit_Control);
//	tx_msg.IDE = CAN_ID_STD;
//	tx_msg.RTR = CAN_RTR_DATA;
//	tx_msg.DLC = 0x08;
//	send_data[0] = 0x00;
//	send_data[1] = 0x00;
//
//	send_data[2] = 0x00;
//	send_data[3] = 0x00;
//
//	send_data[4] = 0x00;
//	send_data[5] = 0x00;
//
//	send_data[6] = 0x00;
//	send_data[7] = 0x00;
//	CAN_TxMessage(&Motor_CanLine, &tx_msg, send_data);
//}

void drv_canC::GetTorques()
{
	tx_msg.StdId = SetID(Motor_ID, Get_Torques);
	tx_msg.IDE = CAN_ID_STD;
	tx_msg.RTR = CAN_RTR_DATA;
	tx_msg.DLC = 0x08;
	send_data[0] = 0x00;
	send_data[1] = 0x00;

	send_data[2] = 0x00;
	send_data[3] = 0x00;

	send_data[4] = 0x00;
	send_data[5] = 0x00;

	send_data[6] = 0x00;
	send_data[7] = 0x00;
	CAN_TxMessage(Motor_CanLine, &tx_msg, send_data);
}

void drv_canC::Get_EncoderEstimates()
{
	tx_msg.StdId = SetID(Motor_ID, Get_Encoder_Estimates);
	tx_msg.IDE = CAN_ID_STD;
	tx_msg.RTR = CAN_RTR_DATA;
	tx_msg.DLC = 0x08;
	send_data[0] = 0x00;
	send_data[1] = 0x00;

	send_data[2] = 0x00;
	send_data[3] = 0x00;

	send_data[4] = 0x00;
	send_data[5] = 0x00;

	send_data[6] = 0x00;
	send_data[7] = 0x00;
	CAN_TxMessage(Motor_CanLine, &tx_msg, send_data);
}

void drv_canC::RebootMotor()
{
	tx_msg.StdId = SetID(Motor_ID, Reboot);
	tx_msg.IDE = CAN_ID_STD;
	tx_msg.RTR = CAN_RTR_DATA;
	tx_msg.DLC = 0x08;
	send_data[0] = 0x00;
	send_data[1] = 0x00;

	send_data[2] = 0x00;
	send_data[3] = 0x00;

	send_data[4] = 0x00;
	send_data[5] = 0x00;

	send_data[6] = 0x00;
	send_data[7] = 0x00;
	CAN_TxMessage(Motor_CanLine, &tx_msg, send_data);
}

void drv_canC::ClearMotorError()
{
	tx_msg.StdId = SetID(Motor_ID, Clear_Errors);
	tx_msg.IDE = CAN_ID_STD;
	tx_msg.RTR = CAN_RTR_DATA;
	tx_msg.DLC = 0x08;
	send_data[0] = 0x00;
	send_data[1] = 0x00;

	send_data[2] = 0x00;
	send_data[3] = 0x00;

	send_data[4] = 0x00;
	send_data[5] = 0x00;

	send_data[6] = 0x00;
	send_data[7] = 0x00;
	CAN_TxMessage(Motor_CanLine, &tx_msg, send_data);
}

void drv_canC::SetMotorMode(uint8_t ControlMode,uint8_t InputMode)
{
	tx_msg.StdId = SetID(Motor_ID, Set_Controller_Mode);
	tx_msg.IDE = CAN_ID_STD;
	tx_msg.RTR = CAN_RTR_DATA;
	tx_msg.DLC = 0x08;
	send_data[0] = ControlMode;
	send_data[1] = 0x00;

	send_data[2] = 0x00;
	send_data[3] = 0x00;

	send_data[4] = InputMode;
	send_data[5] = 0x00;

	send_data[6] = 0x00;
	send_data[7] = 0x00;
	CAN_TxMessage(Motor_CanLine, &tx_msg, send_data);
}

void drv_canC::SetMotorState(uint8_t MotorState)
{
	tx_msg.StdId = SetID(Motor_ID, Set_Axis_State);
	tx_msg.IDE = CAN_ID_STD;
	tx_msg.RTR = CAN_RTR_DATA;
	tx_msg.DLC = 0x08;

	send_data[0] = MotorState;
	send_data[1] = 0x00;

	send_data[2] = 0x00;
	send_data[3] = 0x00;

	send_data[4] = 0x00;
	send_data[5] = 0x00;

	send_data[6] = 0x00;
	send_data[7] = 0x00;
	CAN_TxMessage(Motor_CanLine, &tx_msg, send_data);
}

void drv_canC::SetMotorPosition(float Target_Position)
{
	tx_msg.StdId = SetID(Motor_ID, Set_Input_Pos); //位置控制
	tx_msg.IDE = CAN_ID_STD;
	tx_msg.RTR = CAN_RTR_DATA;
	tx_msg.DLC = 0x08;
	Target_Position = Target_Position *  3.1415926f / 180.0f;
	float_transfer = *(uint32_t*)&Target_Position;//指针方式
	send_data[0] = float_transfer;
	send_data[1] = float_transfer >> 8;

	send_data[2] = float_transfer >> 16;
	send_data[3] = float_transfer >> 24; //位置目标值

	send_data[4] = 0x00;
	send_data[5] = 0x00;//速度前馈

	send_data[6] = 0x00;
	send_data[7] = 0x00;//力矩前馈
	float_transfer = 0;
	CAN_TxMessage(Motor_CanLine, &tx_msg, send_data);
}

