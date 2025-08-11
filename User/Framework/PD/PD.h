//
// Created by mei on 8/11/25.
//

#ifndef STM32F407IGH6TR_BOARD_USER_FRAMEWORK_PD_PD_H_
#define STM32F407IGH6TR_BOARD_USER_FRAMEWORK_PD_PD_H_
class PD{
 public:
	float P_f,P_T;
	float V_f,V_T;
	float Kp,Kd;
	float u;
	PD(float kp,float kd):Kp(kp),Kd(kd){}
	void Calc();
 private:
	float L_P_T, timestep = 0.01, Torque_max = 10;
};
#endif //STM32F407IGH6TR_BOARD_USER_FRAMEWORK_PD_PD_H_
