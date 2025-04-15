//
// Created by 24316 on 2023/10/25.
//

#ifndef GIMBAL_2023_ABOARD_USER_FRAMEWORK_SMC_SLIDINGMODEC_H_
#define GIMBAL_2023_ABOARD_USER_FRAMEWORK_SMC_SLIDINGMODEC_H_
#include "stm32f4xx_hal.h"

#ifndef _SMC_H_
#define _SMC_H_
/*
���2��ϵͳ
�ο����ӣ�
������ϵͳ��ʮ������ģ���ƽ��� - Chenglin Li������ - ֪��
https://zhuanlan.zhihu.com/p/138860110
������ϵͳ��ʮ�ģ���ģ������������� - Chenglin Li������ - ֪��
https://zhuanlan.zhihu.com/p/139066859
*/

//�����־λ
#define SIGN  1
#define SAT   2

typedef struct SMC SMC;
struct SMC
{
	float a;
	float K;
	float ref;
	float error_eps;//�������
	float p;
	float q;
	float u_max;
	float error;
	float error_last;
	float epsilon;
	float lamda;
	int flag;
	float lastout;
	float refl;
	float dref;
	float ddref;
	float eps;
	float J;
	float ang_vel;//���ٶȷ�����rad/s
	float s;
};

//��ʼ������
void SMC_Initial(SMC* smc);

//���º���
float SMC_Tick(SMC* smc, float y,float ang_vel);
//���ź���
int Signal(float s);

//���ͺ���
float Sat(SMC* smc, float s);

#endif
#endif //GIMBAL_2023_ABOARD_USER_FRAMEWORK_SMC_SLIDINGMODEC_H_
