# 云台调试记录

## 初始化
- 定义兵种（gimbal.h）现在是omni
- 检查pitch、yaw反馈、算法; 目前是陀螺仪反馈+matlab生成前馈PID
    
      getMotorAngelAll_TypeDef YawMotorAllAngel = { 0, 0, 0, 0, 0,
                                                0, 0, 0, 0, YAW_ANGLE,
                                                GYR_MODE, MATLAB};
      getMotorAngelAll_TypeDef PihMotorAllAngel = { 0, 0, 0, 0, 0,
                                                  0, 0, 0, 0, PIH_ANGLE,
                                                  GYR_MODE, NOMEL };
- 断电流，测限位（留出余量）：修改 `void Gimbal_CarInit(void)`（pitch轴、ecd反馈）；`Pih_GyrLimit`（pitch轴，gyr反馈）
- yaw轴与底盘：`void Gimbal_CarInit(void)`里修改 `ChassisYawTarget ;`
  - 11.15修改：由于云台反装，ChassisYawTarget从88改成 -94

## PID调试
- pid数值统一在：`Gimbal_CarChoose()`，需要根据兵种和算法修改对应pid，目前兵种omni，算法matlab

## 注意
- 关遥控器进保护模式
- 可以注释掉 `void Gimbal_SpeedC()`不输出电流，测角度

## to do list
- 研究摩擦轮电机
- 测试，调节弹道
- 测试拨弹轮卡弹保护
- 云台yaw轴pid细调
