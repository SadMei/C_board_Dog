//
// Created by 24316 on 2025/4/15.
//
#include "chassisc.h"
#include "iwdgc.h"
#include "BSP.h"
chassisC Dog;

void chassisC::Init()
{
	for (int i = 0; i < Motor_Num; ++i)
	{
//		Dog.Motors[i].RebootMotor();
//		Dog.Motors[i].ClearMotorError();
//		Dog.Motors[i].SetMotorState(Idle);
		Dog.Motors[i].SetMotorMode(Position,Position_Filt);
		Dog.Motors[i].SetMotorState(Run);
	}
}
//float Target = 360;
void chassisC::Controlloop()
{
	FeedDog();
//	Dog.Motors[0].MITControl();
	for (int i = 0; i < Motor_Num; ++i)
	{
		Dog.Motors[i].SetMotorPosition(0);
//		osDelay(1);
	}
}

void chassisC::Printf_Test()
{
	usart_printf("%f,%f,%f\r\n",Dog.Motors[0].Motor_AngleRad*180/3.1415926,Dog.Motors[0].Motor_SpeedRadSec,Dog.Motors[0].Motor_Torque);
//	usart_printf("%d\r\n",Dog.Motors[0].Motor_ID);
}