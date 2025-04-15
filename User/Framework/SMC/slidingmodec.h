//
// Created by 24316 on 2023/10/25.
//

#ifndef GIMBAL_2023_ABOARD_USER_FRAMEWORK_SMC_SLIDINGMODEC_H_
#define GIMBAL_2023_ABOARD_USER_FRAMEWORK_SMC_SLIDINGMODEC_H_
#include "stm32f4xx_hal.h"

#ifndef _SMC_H_
#define _SMC_H_
/*
针对2阶系统
参考链接：
非线性系统（十三）滑模控制解析 - Chenglin Li的文章 - 知乎
https://zhuanlan.zhihu.com/p/138860110
非线性系统（十四）滑模控制器设计流程 - Chenglin Li的文章 - 知乎
https://zhuanlan.zhihu.com/p/139066859
*/

//定义标志位
#define SIGN  1
#define SAT   2

typedef struct SMC SMC;
struct SMC
{
	float a;
	float K;
	float ref;
	float error_eps;//误差下限
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
	float ang_vel;//角速度反馈，rad/s
	float s;
};

//初始化函数
void SMC_Initial(SMC* smc);

//更新函数
float SMC_Tick(SMC* smc, float y,float ang_vel);
//符号函数
int Signal(float s);

//饱和函数
float Sat(SMC* smc, float s);

#endif
#endif //GIMBAL_2023_ABOARD_USER_FRAMEWORK_SMC_SLIDINGMODEC_H_
