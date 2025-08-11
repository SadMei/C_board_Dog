//
// Created by mei on 8/11/25.
//
#include "PD.h"
void PD::Calc()
{
	V_T = P_T - L_P_T;
	V_T = V_T / timestep;
	u = Kp * ( P_T - P_f ) + Kd * ( V_T - V_f );
	if ( u > Torque_max )
	{
		u = Torque_max;
	}
	else if ( u < -Torque_max )
	{
		u = -Torque_max;
	}
	L_P_T = P_T;
}