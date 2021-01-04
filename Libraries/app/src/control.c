#include "control.h"

//���Ʒ����洫��������֪�Ƿ����
PID_LocTypeDef  lpid, rpid, tpid;
int16 speed = 0;

/************************************************
�������� �� limit_value
��    �� ���޷�
��    �� �� value -----����ֵ
            max   ----- ���ֵ
            min   ----- ��Сֵ
�� �� ֵ ��value ----- ������ֵ
*************************************************/
static int16 limit_value(int16 value, int16 max, int16 min)
{
    if(value > max)
        return max;
    else if (value < min)
        return min;
    else
        return value;
}

/************************************************
�������� �� PID_Loc
��    �� �� PIDλ��(Location)����
��    �� �� SetValue ------ ����ֵ(����ֵ)
            ActualValue --- ʵ��ֵ(����ֵ)
            PID ----------- PID���ݽṹ
�� �� ֵ �� PIDLoc -------- PIDλ��
*************************************************/
int16 PID_Locturn(int32 SetValue, int32 ActualValue, PID_LocTypeDef *PID)
{
    static int32 PIDLoc = 0, d_error = 0;                                  //λ��

    PID->Ek = ActualValue - SetValue;

    d_error = ((PID->Ek1 - PID->Ek) << 3 + d_error << 1) / 10;

    d_error = limit_value(d_error, 5, -5);

    PIDLoc = PID->Kp * PID->Ek + PID->Kd * (d_error);

    PIDLoc = limit_value(PIDLoc, 200, -200);

    PID->Ek1 = PID->Ek;

    return (int16)PIDLoc;
}

/************************************************
�������� �� PID_Loc
��    �� �� PIDλ��(Location)����
��    �� �� SetValue ------ ����ֵ(����ֵ)
            ActualValue --- ʵ��ֵ(����ֵ)
            PID ----------- PID���ݽṹ
�� �� ֵ �� PIDLoc -------- PIDλ��
*************************************************/
int16 PID_Loc(int32 SetValue, int32 ActualValue, PID_LocTypeDef *PID)
{
  static int32 PIDLoc = 0;                       //λ��

  PID->Ek = ActualValue - SetValue;

  PID->LocSum += PID->Ek;                         //�ۼ����

  PID->LocSum = limit_value(PID->LocSum, 220, -220);

  PIDLoc = PID->Kp * PID->Ek + (PID->Ki * PID->LocSum) + PID->Kd * (PID->Ek1 - PID->Ek);

  PIDLoc = limit_value(PIDLoc, 500, -500);

  PID->Ek1 = PID->Ek;

  return (int16)PIDLoc;
}

int16 L_Duty,R_Duty;
#define LEFT_MOTOR_DEAD_VAL 0
#define RIGHT_MOTOR_DEAD_VAL 0

void MOTOR_Control(int32 left_motor_u,int32 right_motor_u)
{
     L_Duty =  left_motor_u;
     R_Duty =  right_motor_u;

    if(1 == car_qidong)
    {
        if(left_motor_u > 0)
        {
            ftm_pwm_duty(ftm0, ftm_ch2,0);
            ftm_pwm_duty(ftm0, ftm_ch3,L_Duty);
        }
        else
        {
            ftm_pwm_duty(ftm0, ftm_ch3,0);
            ftm_pwm_duty(ftm0, ftm_ch2,-L_Duty);
        }

        if(right_motor_u > 0)
        {
            ftm_pwm_duty(ftm0, ftm_ch1,0);
            ftm_pwm_duty(ftm0, ftm_ch0,R_Duty);
        }
        else
        {
            ftm_pwm_duty(ftm0, ftm_ch0,0);
            ftm_pwm_duty(ftm0, ftm_ch1,-R_Duty);
        }
    }
    else
    {
        ftm_pwm_duty(ftm0, ftm_ch0,0);
        ftm_pwm_duty(ftm0, ftm_ch1,0);
        ftm_pwm_duty(ftm0, ftm_ch2,0);
        ftm_pwm_duty(ftm0, ftm_ch3,0);
    }
}

#if 0
void PID_LOC(Double_PID_LOC_Typedef *pid,int8 type)
{
    double index=0;
    double temp,c1,c2,c3,derivative;
    pid->error0 =pid->target-pid->current;

    /**********************************�����ֱ�������ַ���**************************/
    if(pid->result > pid->up_limit)
    {
        if(fabs(pid->error0)>pid->error_max)
            index=0;//���ϴ�ȡ����������
        else if(fabs(pid->error0)>pid->error_min)
        {
            index=(pid->error_max-fabs(pid->error0))/(pid->error_max-pid->error_min);//����ϵ��0-1
            //���С�����������˳����ֱ��������ۼ�ƫ��
            if(pid->error0<0)
                pid->iout+=(pid->error0+pid->error1)*0.5;                     //���λ��� ��������������̬���
        }
        else
        {
            index=1;
            if(pid->error0<0)
                pid->iout+=(pid->error0+pid->error1)*0.5;
        }

    }
    else if(pid->result<pid->low_limit)
    {
        if(fabs(pid->error0)>pid->error_max)
            index=0;
        else if(fabs(pid->error0)>pid->error_min)
        {
            index=(pid->error_max-fabs(pid->error0))/(pid->error_max-pid->error_min);
            if(pid->error0>0)
                pid->iout+=(pid->error0+pid->error1)*0.5;
        }

        else
        {
            index=1;
            if(pid->error0>0)
                pid->iout+=(pid->error0+pid->error1)*0.5;
        }
    }
    //pid������ض���Χ��
    else
    {
        if(fabs(pid->error0)>pid->error_max)
            index=0;
        else if(fabs(pid->error0)>pid->error_min)
        {
            index=(pid->error_max-fabs(pid->error0))/(pid->error_max-pid->error_min);
            pid->iout+=(pid->error0+pid->error1)*0.5;
        }
        else
        {
            index=1;
            pid->iout+=(pid->error0+pid->error1)*0.5;
        }
    }
    /******************�����ֱ�������ַ������************************************/
    /************************�����޷�*********************************************/
	if(pid->iout>pid->imax)
		pid->iout=pid->imax;
	else if(pid->iout<-pid->imax)
		pid->iout=-pid->imax;
    /**************************************************************************/
    /**********************΢�����кͲ���ȫ΢��*********************************/
	if(type==3)
	{
		temp=pid->para*pid->kd+pid->kp;                                   //΢�����������趨ֵƵ�������ĳ���
		c3=pid->kd/temp;
		c2=(pid->kd+pid->kp)/temp;
		c1=pid->para*c3;
		derivative=c1*pid->last_derivative+c2*pid->current+c3*pid->last_measure;
		pid->last_derivative=derivative;
        pid->result=pid->kp*pid->error0 +index*pid->ki*pid->iout +derivative;
	}
	else if(type==1)
	{
		derivative=(1-pid->para)*pid->kd*(pid->error0-pid->error1)+pid->para*pid->last_derivative;          //����ȫ΢��  �൱����΢�ֲ��ּ���һ�ڵ�ͨ�˲�
        pid->last_derivative=derivative;
        pid->result=pid->kp*pid->error0+index*pid->ki*pid->iout +derivative;
	}
    /***********************************************************************/
	else if(type==2)
	{
        pid->result=pid->kp*pid->error0+index*pid->ki*pid->iout+pid->kd*(pid->error0-pid->error1);
	}
    pid->error1=pid->error0;     // ������
    pid->last_measure=pid->current;// �ϴβ���ֵ ����΢������
    /*****************************�޷�***********************************/
    if(pid->result>pid->up_limit)
        pid->result=pid->up_limit;
    if(pid->result<pid->low_limit)
        pid->result=pid->low_limit;
    /**********************������pid*************************************/
    if(fabs(pid->result) < pid->min_result)
        pid->result=0;
}

#endif

uint8 whitr_line[5] = {0};
uint8 Startingpoint= 93;
uint8 isStraight = 1;

int16 Straightline_fitting(uint8 x1, uint8 y1, uint8 x2, uint8 y2, uint8 c1, uint8 c2)    //0~39����     y:0~39  x:0~187
{
      float k = 0.0, b = 0.0;
      int16 err = 0;
      k = (y2-y1)*1.0 / (x2 - x1)*1.0;
      b = y1 - k*x1;

      err += (k * c1 + b - 10);
      err += (k * c2 + b - 25);
      err /= 2;
      return err;
}
int8 curve_deal(uint8 start)
{
    int16 error = 0;
    uint8 i = 0, j = 0;
    Startingpoint= 93;
    memset(whitr_line, 0, sizeof(whitr_line));
    if(image[79][187] < img_threshold)
    {
         for(i = 0; i< 5; i++,start-=2)
         {
            for(j = Startingpoint; j > 1; j--)
            {
                if(image[start][j] > img_threshold &&
                  image[start][j - 1] > img_threshold &&
                  image[start][j - 2] > img_threshold)
                {
                   whitr_line[i] = j;
                   error += whitr_line[i];
                }
            }

            Startingpoint =  whitr_line[i] + 4;
         }

         return error * 5 / 50;

    }
    else if(image[79][187] > img_threshold)
    {
         for(i = 0; i< 5; i++,start-=2)
         {
            for(j = 93; j < 184; j++)
            {
                if(image[start][j] > img_threshold &&
                  image[start][j + 1] > img_threshold &&
                  image[start][j + 2] > img_threshold)
                {
                   whitr_line[i] = j;
                   error += whitr_line[i];
                }
            }

            Startingpoint =  whitr_line[i] - 4;
         }

         return error * 5 / 50;
    }
}

int8 Centerline_diff= 0;
uint8 vaildline = 0;
int16 err_centerline = 0;
int16 turn = 0;
int16 last_turn = 0;
uint8 straight_flag = 0;
uint8 curve_flag = 0;
uint8 outline_flag = 0;
int16 temp1 = 0;
extern uint32 time_count;
void Track_info_deal(void)
{
    int16  err_sum = 0;
    if(lost_b_count > 25)
    {
        if(image[79][187] > img_threshold)
         temp1 = Straightline_fitting(left_line[0], 0, left_line[39], 39, left_line[10], left_line[25]);    //����ͷ������ߣ��԰ױ�Ϊ�����仯ֵ
        else if(image[79][0] > img_threshold)
         temp1 = Straightline_fitting(Right_line[0], 0, Right_line[39], 39, Right_line[10], Right_line[25]);
         return;
    }


    if(1)
    {
         curve_flag = 1;

    }



    if(curve_flag == 0 && abs(Model_line[39] - Model_line[38] > 9))
    {
        curve_flag = 1;
        vaildline = 39 - 7 - Mutationline;
        buzzer(20);

    }
    else
    {
        curve_flag = 0;
        vaildline = 10;
    }



    for(uint8 i = 0; i < 5; i++)
    {
        err_sum += (Model_line[vaildline++] - 93);
    }
    Centerline_diff = err_sum * 5 / 50;
}


void CarControl(void)
{
    if(!search_finished) return;
    search_finished = 0;
    static int32 left_motor = 0,right_motor = 0;

    Track_info_deal();
    left_motor = PID_Loc(speed, (l_speed + r_speed) >> 1, &lpid);
    right_motor = PID_Loc(speed, (l_speed + r_speed) >> 1, &rpid);

    turn = PID_Locturn(err_centerline, Centerline_diff, &tpid);

    left_motor -= turn;
    right_motor += turn;


    MOTOR_Control(left_motor, right_motor);

}