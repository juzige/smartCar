#ifndef __CONTROL_H_
#define __CONTROL_H_

#include "headfile.h"

typedef struct
{
  int8 Kp;                       //比例系数Proportional
  int8 Ki;                       //积分系数Integral
  int8 Kd;                       //微分系数Derivative

  int32 Ek;                       //当前误差
  int32 Ek1;                      //前一次误差 e(k-1)
  int32 LocSum;                   //累计积分位置
}PID_LocTypeDef;

extern int16 L_Duty,R_Duty;
extern int16 speed;
extern PID_LocTypeDef  lpid, rpid, tpid;
extern int8 Centerline_diff;
extern int16 turn;
extern uint8 curve_flag;
extern int16 temp1;

void MOTOR_Control(int32 left_motor_u,int32 right_motor_u);

void speed_control();
void motor_output();

void CarControl(void);


#endif