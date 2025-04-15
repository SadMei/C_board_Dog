#ifndef BSP_IMU_PWM_H
#define BSP_IMU_PWM_H

#include <stdint.h>
#include "struct_typedef.h"
#include "tim.h"

void TIM_Set_PWM(TIM_HandleTypeDef *tim_pwmHandle, uint8_t Channel, uint16_t value);
extern void imu_pwm_set(uint16_t pwm);
#endif
