//
// Created by 24316 on 2025/4/15.
//
#include "chassisc.h"
#include "iwdgc.h"
#include "BSP.h"
#include "packet.hpp"
#include "INS_task.h"

chassisC Dog;
void chassisC::Init()
{
	HAL_Delay(2000);
	for (int i = 0; i < Motor_Num; ++i)
	{
		Motors[i].stand_angle_rad = Motors[i].stand_angle * 3.1415926 / 180.0f;
		Motors[i].lay_angle_rad = Motors[i].lay_angle * 3.1415926 / 180.0f;
		if (mode == SAFE)
		{
			while (Motors[i].Motor_AngleRad == 0)
			{
				Motors[i].SetMotorMode(Torque, Torque_Ramp);
				HAL_Delay(10);
				Motors[i].SetMotorState(Run);
				HAL_Delay(10);
				Motors[i].SetMotorState(Idle);
				HAL_Delay(10);
			}
		}
		else if (mode == MOTION)
		{
			while (Motors[i].Motor_AngleRad == 0)
			{
				Motors[i].SetMotorMode(Position, Position_Filt);
				HAL_Delay(10);

				Motors[i].SetMotorState(Run);
				HAL_Delay(10);
			}
		}
	}
}

void chassisC::SafeChecker()
{
	//某个电机掉线也可以加上

	//
	if (CDC_Checker == Last_CDC_Checker)
	{
		online_timer++;
	}
	else
	{
		is_online = 1;
		online_timer = 0;
	}

	if (online_timer > 500)
	{
		is_online = 0;
		online_timer = 0;
	}
	Last_CDC_Checker = CDC_Checker;

	//此处可能需要更改
	mode = is_online;
}

void chassisC::SafeBuilder()
{
	if (mode == SAFE)
	{
		for (int i = 0; i < Motor_Num; ++i)
		{
			//while
			while (Motors[i].Motor_State != 1)
			{
				Motors[i].SetMotorState(Idle);
				osDelay(1);
			}
		}
		permit_communication = 0;
	}
	else if (mode == MOTION && last_mode == SAFE)
	{
		for (int i = 0; i < Motor_Num; ++i)
		{
			Motors[i].Motor_AngleTarget = Motors[i].stand_angle_rad;
		}
		//腰
		for (int i = 0; i < Motor_Num; ++i)
		{
			//while
			if (i == 0 || i == 3 || i == 6 || i == 9)
			{
				while (Motors[i].Motor_State != 8 || Motors[i].Motor_AngleRad == 0)
				{
					Motors[i].SetMotorMode(Position, Trapezoidal_Curve);
					osDelay(1);
					Motors[i].SetMotorState(Run);
					osDelay(1);
					Motors[i].SetMotorMode(Position, Trapezoidal_Curve);
					osDelay(1);
					Motors[i].SetMotorState(Run);
					osDelay(1);
				}
			}
		}
		for (int i = 0; i < Motor_Num; ++i)
		{
			if (i == 0 || i == 3 || i == 6 || i == 9)
			{
				for (int j = 0; j < 10; ++j)
				{
					Motors[i].SetMotorPosition(Motors[i].Motor_AngleTarget);
					osDelay(2);
				}
			}
		}
		osDelay(2000);
		//小腿
		for (int i = 0; i < Motor_Num; ++i)
		{
			if (i == 2 || i == 5 || i == 8 || i == 11)
			{
				while (Motors[i].Motor_State != 8 || Motors[i].Motor_AngleRad == 0)
				{
					Motors[i].SetMotorMode(Position, Trapezoidal_Curve);
					osDelay(1);
					Motors[i].SetMotorState(Run);
					osDelay(1);
					Motors[i].SetMotorMode(Position, Trapezoidal_Curve);
					osDelay(1);
					Motors[i].SetMotorState(Run);
					osDelay(1);
				}
			}
		}
		for (int i = 0; i < Motor_Num; ++i)
		{
			if (i == 2 || i == 5 || i == 8 || i == 11)
			{
				for (int j = 0; j < 10; ++j)
				{
					Motors[i].SetMotorPosition(Motors[i].Motor_AngleTarget);
					osDelay(2);
				}
			}
		}
		osDelay(1000);
		//大腿
		for (int i = 0; i < Motor_Num; ++i)
		{
			//while
			if (i == 1 || i == 4 || i == 7 || i == 10)
			{
				while (Motors[i].Motor_State != 8 || Motors[i].Motor_AngleRad == 0)
				{
					Motors[i].SetMotorMode(Position, Trapezoidal_Curve);
					osDelay(1);
					Motors[i].SetMotorState(Run);
					osDelay(1);
					Motors[i].SetMotorMode(Position, Trapezoidal_Curve);
					osDelay(1);
					Motors[i].SetMotorState(Run);
					osDelay(1);
				}
			}
		}
		for (int i = 0; i < Motor_Num; ++i)
		{
			if (i == 1 || i == 4 || i == 7 || i == 10)
			{
				for (int j = 0; j < 10; ++j)
				{
					Motors[i].SetMotorPosition(Motors[i].Motor_AngleTarget);
					osDelay(2);
				}
			}
		}
		osDelay(4000);
		//这里的延时可能还要随时回来检查mode是否为safe
		permit_communication = 0;
	}
	else if (mode == MOTION && permit_communication == 0)
	{
		for (int i = 0; i < Motor_Num; ++i)
		{
			Motors[i].SetMotorMode(Position, Position_Filt);
			osDelay(1);
			Motors[i].SetMotorState(Run);
			osDelay(1);
			while (Motors[i].Motor_State != 8 || Motors[i].Motor_AngleRad == 0)
			{
				Motors[i].SetMotorMode(Position, Position_Filt);
				osDelay(1);
				Motors[i].SetMotorState(Run);
				osDelay(1);
			}
		}
		permit_communication = 1;
	}
	last_mode = mode;
}

bool flag = 0;
void chassisC::Controlloop()
{
//	FeedDog();
	if (permit_communication && is_online)
	{
		if( flag == 0 )
		{
			flag = 1;
			for (int i = 0; i < Motor_Num; ++i)
			{
				if( i == 0 || i == 1 || i == 2 || i == 6 || i == 7 || i == 8 )
				{
					if(i == 2 || i == 8) vision_pkt.motorCmd[i].q = vision_pkt.motorCmd[i].q *1.553;
					Motors[i].Motor_AngleTarget = Motors[i].stand_angle_rad + Motors[i].signer * vision_pkt.motorCmd[i].q;
					Motors[i].SetMotorPosition(Motors[i].Motor_AngleTarget);
				}

			}
		}
		else if( flag )
		{
			flag = 0;
			for (int i = 0; i < Motor_Num; ++i)
			{
				if( i == 3 || i == 4 || i == 5 || i == 9 || i == 10 || i == 11 )
				{
					if(i == 5 || i == 11) vision_pkt.motorCmd[i].q = vision_pkt.motorCmd[i].q *1.553;
					Motors[i].Motor_AngleTarget = Motors[i].stand_angle_rad + Motors[i].signer * vision_pkt.motorCmd[i].q;
					Motors[i].SetMotorPosition(Motors[i].Motor_AngleTarget);
				}
			}
		}
	}
}

void chassisC::Printf_Test()
{
//	usart_printf("%f,%f,%f\r\n",Motors[2].Motor_AngleRad*180/3.1415926,Motors[2].Motor_SpeedRadSec,Motors[2].Motor_Torque);
//	usart_printf("%d\r\n",Motors[2].Motor_ID);
//	usart_printf("%f,%f,%f\r\n",Motors[0].Motor_AngleRad*180/3.1415926,Motors[1].Motor_AngleRad*180/3.1415926,Motors[2].Motor_AngleRad*180/3.1415926);
//	usart_printf("%f,%f,%f\r\n",Motors[3].Motor_AngleRad*180/3.1415926,Motors[4].Motor_AngleRad*180/3.1415926,Motors[5].Motor_AngleRad*180/3.1415926);
//	usart_printf("%f,%f,%f\r\n",Motors[6].Motor_AngleRad*180/3.1415926,Motors[7].Motor_AngleRad*180/3.1415926,Motors[8].Motor_AngleRad*180/3.1415926);
//	usart_printf("%f,%f,%f\r\n",Motors[9].Motor_AngleRad*180/3.1415926,Motors[10].Motor_AngleRad*180/3.1415926,Motors[11].Motor_AngleRad*180/3.1415926);
	usart_printf("%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%d,%d,%d\r\n",
		Motors[0].Motor_AngleRad * 180.0f / 3.1415926,Motors[1].Motor_AngleRad * 180.0f / 3.1415926,
		Motors[2].Motor_AngleRad * 180.0f / 3.1415926,Motors[3].Motor_AngleRad * 180.0f / 3.1415926,
		Motors[4].Motor_AngleRad * 180.0f / 3.1415926,Motors[5].Motor_AngleRad * 180.0f / 3.1415926,
		Motors[6].Motor_AngleRad * 180.0f / 3.1415926,Motors[7].Motor_AngleRad * 180.0f / 3.1415926,
		Motors[8].Motor_AngleRad * 180.0f / 3.1415926,Motors[9].Motor_AngleRad * 180.0f / 3.1415926,
		Motors[10].Motor_AngleRad * 180.0f / 3.1415926,Motors[11].Motor_AngleRad * 180.0f / 3.1415926,
		mode, CDC_Checker ,permit_communication);
//	usart_printf("%d\r\n",mode);
//	usart_printf(
//		"%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%d\r\n",
//		vision_pkt.motorCmd[0].q, vision_pkt.motorCmd[1].q,
//		vision_pkt.motorCmd[2].q, vision_pkt.motorCmd[3].q,
//		vision_pkt.motorCmd[4].q, vision_pkt.motorCmd[5].q,
//		vision_pkt.motorCmd[6].q, vision_pkt.motorCmd[7].q,
//		vision_pkt.motorCmd[8].q, vision_pkt.motorCmd[9].q,
//		vision_pkt.motorCmd[10].q, vision_pkt.motorCmd[11].q, mode
//	);
//	usart_printf("%f,%f,%f\r\n",INS.Gyro[0],INS.Gyro[1],INS.Gyro[2]);
}