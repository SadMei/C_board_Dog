//
// Created by LENOVO on 2022/9/6.
//

#ifndef ROBO_TEST_CORE_INC_BSP_CAN_H_
#define ROBO_TEST_CORE_INC_BSP_CAN_H_

#include "stm32f4xx_hal.h"

void CAN_All_Init(void);
void CAN_Filter_Init(CAN_HandleTypeDef* hcan);

extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2;
typedef enum
{
	Heartbeat = 0x01,
	Get_Encoder_Estimates = 0x09,
	Get_Encoder_Count = 0x0A,
	Get_Iq = 0x14,
	Get_Bus_Voltage_Current = 0x17,
	Get_Torques = 0x1C,
	Get_Powers = 0x1D,
} eRcv_ID;

typedef enum
{
	Estop = 0x02,
	Set_Axis_Node_ID = 0x06,
	Set_Axis_State = 0x07,
	Set_Controller_Mode = 0x0B,
	Set_Input_Pos = 0x0C,
	Set_Input_Vel = 0x0D,
	Set_Input_Torque = 0x0E,
	Set_Limits = 0x0F,
	Start_Anticogging = 0x10,
	Set_Traj_Vel_Limit = 0x11,
	Set_Traj_Accel_Limits = 0x12,
	Set_Traj_Inertia = 0x13,
	Reboot = 0x16,
	Clear_Errors = 0x18,
	Set_Linear_Count = 0x19,
	Set_Pos_Gain = 0x1A,
	Set_Vel_Gains = 0x1B,
	Disable_Can = 0x1E,
	Save_Configuration = 0X1F,
} eCmd_ID;


typedef enum
{
	Get_Error = 0x03,
	RxSdo = 0x04,
	TxSdo = 0x05,
	Mit_Control = 0x08,
} eCmd2Rcv_ID;

typedef enum
{
	Idle = 0x01,
	Run = 0x08,

	MotorError = 1,
	EncoderError = 2,
	ControllerError = 3,
	SystemError = 4,
	TrajectoryError = 5,
	CommunicateError = 6,
} eMotorState;

typedef enum
{
	small = 0,
	big = 1
} eMotor_Type;

typedef enum
{
	Voltage = 0,
	Torque = 0x01,
	Velocity = 0x02,
	Position = 0x03,

	None = 0,
	Straight = 0x01,
	Velocity_Ramp = 0x02,
	Position_Filt = 0x03,
	Trapezoidal_Curve = 0x05,
	Torque_Ramp = 0x06,
	MIT = 0x09,
} eMotor_Mode;

class drv_canC
{
 public:
	//输出给电机
	void RebootMotor();
	void ClearMotorError();
	void SetMotorMode(uint8_t ControlMode,uint8_t InputMode);
	void SetMotorState(uint8_t MotorState);
	void SetMotorPosition(float Target_Position);

//	void MITControl();
	void GetTorques();
	void Get_EncoderEstimates();

	//收到的电机信息
	void GetMotorMsg(CAN_HandleTypeDef* hcan);
	float Motor_AngleRad;
	float Motor_SpeedRadSec;
	float Motor_Torque;
	uint8_t Motor_State;

	//初始化时的信息
	uint8_t Motor_Type = small; //这里要写判断对应减速比等等
	uint8_t Motor_ID; //电机对应的id
	CAN_HandleTypeDef* Motor_CanLine = &hcan1;
	drv_canC(uint8_t Motor_ID, uint8_t Motor_Type, CAN_HandleTypeDef* Motor_CanLine): Motor_ID(Motor_ID), Motor_Type(Motor_Type), Motor_CanLine(Motor_CanLine){}; //初始化列表
 private:
	uint8_t send_data[8];
	CAN_TxHeaderTypeDef tx_msg;
	uint32_t float_transfer = 0; //float转uint32用变量
	///工具函数
	uint16_t SetID(uint8_t Motor_id, uint8_t CMD_id)
	{
		return (Motor_id << 5) + CMD_id;
	}
	///CAN自动选择邮箱发送
	int CAN_TxMessage(CAN_HandleTypeDef* hcan, CAN_TxHeaderTypeDef* pHeader, uint8_t aData[])
	{
		uint8_t mailbox = 0;
		uint32_t pTxMailbox[mailbox];
		if (hcan->Instance->TSR & (1 << 26)) mailbox = 0;       //邮箱0为空
		else if (hcan->Instance->TSR & (1 << 27)) mailbox = 1;  //邮箱1为空
		else if (hcan->Instance->TSR & (1 << 28)) mailbox = 2;  //邮箱2为空
		else return 0xFF;                                   //无空邮箱,无法发送
		hcan->Instance->sTxMailBox[mailbox].TIR = 0;        //清除之前的设置
		HAL_CAN_AddTxMessage(hcan, pHeader, aData, pTxMailbox);
		return mailbox;
	}
};

//extern drv_canC motor1,motor2,motor3;
#endif //ROBO_TEST_CORE_INC_BSP_CAN_H_
