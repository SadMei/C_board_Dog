//
// Created by 24316 on 2023/10/25.
//Edited by naranja on 2024/11/30.
//
//��Ĥ���� ��һ��

// S = Ce+de;eΪλ����de���ٶ����
//void SMC(float target,float now)//slidingmode controller
//{
//
//}
#include"slidingmodec.h"
#include<cmath>
#include"debugc.h"

void SMC_Initial(SMC* smc)
{
	smc->a=20;//1��
	smc->K=120;
	smc->q = 21;
	smc->p = 27;
	smc->error_eps=0.001;//�������
	smc->u_max=25000; //ָ������������ֵ����K
	smc->epsilon=5000;
	smc->epsilon=1;
	smc->flag=SAT; //�л��򱥺ͺ�����־λ
	smc->error_last = 0;
	// smc->lamda = 0.5*smc->a;//2��
	smc->lamda = 1;
	smc->lastout=0;
	smc->refl = 0;
	smc->dref = 0;
	smc->ddref = 0;
	smc->eps = 0.5;
	smc->J = 0.8;
	smc-> ang_vel = 0;

}

float SMC_Tick(SMC* smc, float y, float ang_vel) //yΪ��ǰλ��,ang_velΪ���ٶȣ�rad/s��
{
	float s,u,ds,e_qp;
	float qp = smc->q/smc->p;
	//��ȡ����
	smc->error =  y-smc->ref ; //smc->refΪĿ��Ƕ�
	smc->ddref = (smc->ref - smc->refl) - smc->dref;
	smc->dref = (smc->ref - smc->refl);
	smc->ang_vel = ang_vel;

	//������޴���
	if (fabs(smc->error) < smc->error_eps)
	{
		return 0;
	}

	// ��ͨ��Ĥ
	// s = smc->a * smc->error + (smc->ang_vel-smc->dref)* smc->lamda;//smc surface
	// u = smc->J*(smc->ddref-smc->a* (smc->ang_vel-smc->dref) - smc->eps * Sat(smc, s) - smc->K * s);
	//u = J((��d)''-ce'-��*sat(s)-K*s) ;Jת����������dΪĿ��Ƕȣ�cΪ����ϵ������Ϊ���ͺ�����������������KΪ��������
	if(smc->error<0)
		e_qp = -pow(abs(smc->error),qp);
	else
		e_qp = pow(abs(smc->error),qp);
	// //�����ն˻�Ĥ
	s = smc->ang_vel-smc->dref + smc->a *e_qp;
	ds = - smc->eps * Sat(smc, s) - smc->K * s;
	u = -smc->J*(smc->ddref +ds - smc->a * qp * (smc->ang_vel-smc->dref) *e_qp/abs(smc->error));

	if(abs(smc->error) < 1 )
	{
		s = smc->a * smc->error + (smc->ang_vel-smc->dref);//smc surface
		u = -smc->J*(smc->ddref-smc->a* (smc->ang_vel-smc->dref) - smc->eps * Sat(smc, s) - smc->K * s);
	}

	//�������޷�
	if (u > smc->u_max)
		u = smc->u_max;
	if (u < -smc->u_max)
		u = -smc->u_max;


	//��������
	smc->lastout = u;
	smc->refl = smc -> ref;
	smc->s = s;
	return u;
}

// ���ź��� ,���ж������Ի������͵ı��ͺ�����sat���ͺ���
int Signal(float s)
{
	if (s > 0)
		return 1;
	else if (s == 0)
		return 0;
	else
		return -1;
}

//���ͺ���
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
