//
// Created by 24316 on 2023/10/25.
//Edited by naranja on 2024/11/30.
//
//滑膜控制 憋一个

// S = Ce+de;e为位置误差，de即速度误差
//void SMC(float target,float now)//slidingmode controller
//{
//
//}
#include"slidingmodec.h"
#include<cmath>
#include"debugc.h"

void SMC_Initial(SMC* smc)
{
	smc->a=20;//1阶
	smc->K=120;
	smc->q = 21;
	smc->p = 27;
	smc->error_eps=0.001;//误差下限
	smc->u_max=25000; //指定控制量最大幅值等于K
	smc->epsilon=5000;
	smc->epsilon=1;
	smc->flag=SAT; //切换或饱和函数标志位
	smc->error_last = 0;
	// smc->lamda = 0.5*smc->a;//2阶
	smc->lamda = 1;
	smc->lastout=0;
	smc->refl = 0;
	smc->dref = 0;
	smc->ddref = 0;
	smc->eps = 0.5;
	smc->J = 0.8;
	smc-> ang_vel = 0;

}

float SMC_Tick(SMC* smc, float y, float ang_vel) //y为当前位置,ang_vel为角速度（rad/s）
{
	float s,u,ds,e_qp;
	float qp = smc->q/smc->p;
	//读取参数
	smc->error =  y-smc->ref ; //smc->ref为目标角度
	smc->ddref = (smc->ref - smc->refl) - smc->dref;
	smc->dref = (smc->ref - smc->refl);
	smc->ang_vel = ang_vel;

	//误差下限处理
	if (fabs(smc->error) < smc->error_eps)
	{
		return 0;
	}

	// 普通滑膜
	// s = smc->a * smc->error + (smc->ang_vel-smc->dref)* smc->lamda;//smc surface
	// u = smc->J*(smc->ddref-smc->a* (smc->ang_vel-smc->dref) - smc->eps * Sat(smc, s) - smc->K * s);
	//u = J((θd)''-ce'-ε*sat(s)-K*s) ;J转动惯量，θd为目标角度，c为阻尼系数，ε为饱和函数消除抖动参数，K为控制增益
	if(smc->error<0)
		e_qp = -pow(abs(smc->error),qp);
	else
		e_qp = pow(abs(smc->error),qp);
	// //快速终端滑膜
	s = smc->ang_vel-smc->dref + smc->a *e_qp;
	ds = - smc->eps * Sat(smc, s) - smc->K * s;
	u = -smc->J*(smc->ddref +ds - smc->a * qp * (smc->ang_vel-smc->dref) *e_qp/abs(smc->error));

	if(abs(smc->error) < 1 )
	{
		s = smc->a * smc->error + (smc->ang_vel-smc->dref);//smc surface
		u = -smc->J*(smc->ddref-smc->a* (smc->ang_vel-smc->dref) - smc->eps * Sat(smc, s) - smc->K * s);
	}

	//控制量限幅
	if (u > smc->u_max)
		u = smc->u_max;
	if (u < -smc->u_max)
		u = -smc->u_max;


	//参数更新
	smc->lastout = u;
	smc->refl = smc -> ref;
	smc->s = s;
	return u;
}

// 符号函数 ,若有抖动可以换个陡峭的饱和函数，sat饱和函数
int Signal(float s)
{
	if (s > 0)
		return 1;
	else if (s == 0)
		return 0;
	else
		return -1;
}

//饱和函数
float Sat(SMC* smc, float s)
{
	float y;
	y = s / smc->epsilon;
	if (fabs(y) <= 1)
		return y;
	else
		return Signal(y);
		// return tanh(y);
}
