//
// Created by 24316 on 2025/4/15.
//
#include "chassisc.h"
#include "iwdgc.h"
#include "BSP.h"
chassisC Dog;

void chassisC::Init()
{
	HAL_Delay(2000);
	for (int i = 0; i < Motor_Num; ++i)
	{
//		Dog.Motors[i].RebootMotor();
//		Dog.Motors[i].ClearMotorError();

		Dog.Motors[i].SetMotorMode(Position,Position_Filt);
		HAL_Delay(10);
		Dog.Motors[i].SetMotorState(Run);
		HAL_Delay(100);
	}

//	HAL_Delay(100);
//	for (int i = 0; i < Motor_Num; ++i)
//	{
//		Dog.Motors[i].SetMotorState(Idle);
//		HAL_Delay(100);
//	}

}
//float Target = 360;
void chassisC::Controlloop()
{
	FeedDog();
	Dog.Motors[0].Motor_AngleTarget = 55;
	Dog.Motors[1].Motor_AngleTarget = -36;
	Dog.Motors[2].Motor_AngleTarget = 12;

	Dog.Motors[3].Motor_AngleTarget = 30;
	Dog.Motors[4].Motor_AngleTarget = -10;
	Dog.Motors[5].Motor_AngleTarget = -27;//

	Dog.Motors[6].Motor_AngleTarget = -10;
	Dog.Motors[7].Motor_AngleTarget = 58;
	Dog.Motors[8].Motor_AngleTarget = 0;

	Dog.Motors[9].Motor_AngleTarget = 5;
	Dog.Motors[10].Motor_AngleTarget = -50;
	Dog.Motors[11].Motor_AngleTarget = -80;

	//设置每个电机的位置,以下为CAN1
	Dog.Motors[0].SetMotorPosition(	Dog.Motors[0].Motor_AngleTarget );
	Dog.Motors[1].SetMotorPosition(	Dog.Motors[1].Motor_AngleTarget );
	Dog.Motors[2].SetMotorPosition( Dog.Motors[2].Motor_AngleTarget );
	Dog.Motors[3].SetMotorPosition(	Dog.Motors[3].Motor_AngleTarget );
	Dog.Motors[4].SetMotorPosition(	Dog.Motors[4].Motor_AngleTarget );
	Dog.Motors[5].SetMotorPosition( Dog.Motors[5].Motor_AngleTarget );
	//以下为CAN2
	Dog.Motors[6].SetMotorPosition(	Dog.Motors[6].Motor_AngleTarget );
	Dog.Motors[7].SetMotorPosition(	Dog.Motors[7].Motor_AngleTarget );
	Dog.Motors[8].SetMotorPosition( Dog.Motors[8].Motor_AngleTarget );
	Dog.Motors[9].SetMotorPosition(	Dog.Motors[9].Motor_AngleTarget );
	Dog.Motors[10].SetMotorPosition(Dog.Motors[10].Motor_AngleTarget );
	Dog.Motors[11].SetMotorPosition(Dog.Motors[11].Motor_AngleTarget );
}

void chassisC::Printf_Test()
{
//	usart_printf("%f,%f,%f\r\n",Dog.Motors[2].Motor_AngleRad*180/3.1415926,Dog.Motors[2].Motor_SpeedRadSec,Dog.Motors[2].Motor_Torque);
//	usart_printf("%d\r\n",Dog.Motors[2].Motor_ID);
//	usart_printf("%f,%f,%f\r\n",Dog.Motors[0].Motor_AngleRad*180/3.1415926,Dog.Motors[1].Motor_AngleRad*180/3.1415926,Dog.Motors[2].Motor_AngleRad*180/3.1415926);
//	usart_printf("%f,%f,%f\r\n",Dog.Motors[3].Motor_AngleRad*180/3.1415926,Dog.Motors[4].Motor_AngleRad*180/3.1415926,Dog.Motors[5].Motor_AngleRad*180/3.1415926);
//	usart_printf("%f,%f,%f\r\n",Dog.Motors[6].Motor_AngleRad*180/3.1415926,Dog.Motors[7].Motor_AngleRad*180/3.1415926,Dog.Motors[8].Motor_AngleRad*180/3.1415926);
//	usart_printf("%f,%f,%f\r\n",Dog.Motors[9].Motor_AngleRad*180/3.1415926,Dog.Motors[10].Motor_AngleRad*180/3.1415926,Dog.Motors[11].Motor_AngleRad*180/3.1415926);
	usart_printf("%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f\r\n",Dog.Motors[0].Motor_AngleRad*180/3.1415926,Dog.Motors[1].Motor_AngleRad*180/3.1415926,Dog.Motors[2].Motor_AngleRad*180/3.1415926,
																	Dog.Motors[3].Motor_AngleRad*180/3.1415926,Dog.Motors[4].Motor_AngleRad*180/3.1415926,Dog.Motors[5].Motor_AngleRad*180/3.1415926,
																	Dog.Motors[6].Motor_AngleRad*180/3.1415926,Dog.Motors[7].Motor_AngleRad*180/3.1415926,Dog.Motors[8].Motor_AngleRad*180/3.1415926,
																	Dog.Motors[9].Motor_AngleRad*180/3.1415926,Dog.Motors[10].Motor_AngleRad*180/3.1415926,Dog.Motors[11].Motor_AngleRad*180/3.1415926);

}