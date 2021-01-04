#include "WJ_STAR_IMAGE.h"

#define SSSSSSS_OFF_ON 1    //СS��⿪�أ�1Ϊ�� 0Ϊ��

int16  EdgeThres = 17;  //��������ֵ

float  BlackThres = 60;   //�ڰ���ֵ

uint8 image_h[ROW_H][COL_W];

uint8 Jump[62]; //������
int16 Is_Right_Line[62] = {0},Is_Left_Line[62] = {0};
int16 Outring_Right_Line[62] = {0},Outring_Left_Line[62] = {0};
int16 Left_Line[62] = {0}, Right_Line[62] = {0}, Mid_Line[62] = {0};	// ԭʼ���ұ߽�����
int16 Left_Add_Line[62], Right_Add_Line[62];		// ���ұ߽粹������
int16 Left_Add_Flag[62], Right_Add_Flag[62];		// ���ұ߽粹�߱�־λ
int16 Width_Real[62];	// ʵ���������
int16 Width_Add[62];	// �����������
int16 Width_Min;		// ��С�������

int16 S_temp;
int16 Foresight,Last_Foresight,Ramp_Flag_Last=0;	// ǰհ�������ٶȿ���

uint8 Weight[60] =                      //��Ȩƽ������
{
    4,  4,  4,  4,  5,  5,  5,  5,  5,  6,
    6,  6,  6,  6,  6,  7,  8,  9,  10, 11,
    12, 13, 14, 15, 14, 13, 12, 11, 10, 9,
    8,  7,  6,  5,  4,  2,  2,  2,  2,  2,
    2,  2,  2,  2,  2,  2,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  1,};
uint8 Island_Width_Add[60] =            //�������߲���
{
    0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,
    0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,
    9 ,9 ,12,12,16,16,19,19,23,23,
    27,27,31,31,35,35,38,38,42,42,
    46,46,50,50,52,52,57,57,60,60,
    63,63,67,67,69,69,73,73,75,75
};
uint8 Width_Max[60] =
{
    0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,
    0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,
    9 ,9 ,12,12,16,16,19,19,23,23,
    27,27,31,31,35,35,38,38,42,42,
    46,46,50,50,52,52,57,57,60,60,
    63,63,67,67,69,69,73,73,75,75
};
uint8 Ramp_Error[11];
uint8 Ramp_Flag = 0;
uint8 Ramp_Time_Count = 0;

//�����ļ�����
uint8 Left_Max, Right_Min;
char Last_Point = MIDVALUE;

int16 Left_Hazard_Flag, Right_Hazard_Flag, Hazard_Flag;	// �����ϰ����־

int16 Left_Add_Start, Right_Add_Start;	// ���Ҳ�����ʼ������
int16 Left_Add_Stop, Right_Add_Stop;	// ���Ҳ��߽���������
float Left_Ka = 0, Right_Ka = 0,ring_out_left_ka = 0,ring_out_left_kb = 0,ring_out_Right_kb = 0,ring_out_Right_ka = 0;
float Last_Right_Ka = 1,Last_Right_Kb = 70;
float Last_Left_Ka = 1,Last_Left_Kb = 35;
float Left_Kb = 1, Right_Kb = COL_W-1;	// ��С���˷�����

int16 Out_Side = 0;	// ���߿���
int16 Line_Count;	// ��¼�ɹ�ʶ�𵽵���������

int8 ADD_line_flag = 0;//ʮ�ֲ��߿�ʼ��־

uint8 left_right_guai_flag = 0;//�ж��Ǵ���ת�仹�Ǵ���ת��

/****************************    ����ʶ�����    ****************************/
uint8 Stop_Flag = 0,count_num = 0;	// �����߱�־λ
/****************************        ����        ****************************/
int island_left_mid_add_flag = 0,island_right_mid_add_flag = 0;
int fand_ring_time = 0;  //��Բ��֮��1s���ڼ��Բ��
int right_ring_flag = 0,left_ring_flag = 0;
int tuqi_point_hang=0,tuqi_point_flag = 0;
int out_ring_point_hang = 0;
int out_ring_num = 0;
int go_ring_flag = 0,out_ring_flag = 0;
int island_flag = 0;  //��־Ϊ0���ǻ���   ��־Ϊ1���󻷵�   ��־Ϊ2���һ���
int is_diu_flag = 0;
uint8 find_point = 0;
int zhaoguidian_diu_flag_num = 0;
int flag1 = 0,flag2 = 0;
int ring_big_less_choose = 0;

uint8 find_right_point_x = 29,find_right_point_y = 55;
uint8 find_left_point_x = 29,find_left_point_y = 35;

uint8 left_ring_off_flag = 0;
uint8 right_ring_off_flag = 0;
uint8 island_test_time = 0;

void Image_Get(void)
{
    for(int i = 0; i < 60; i++)
    {
        for(int j = 0; j < 94; j++)
        {
            image[i][j] = image_h[i][j*2];
        }
    }
}

#define ERROR      2      //���
void yuzhiget(void)
{
	uint16_t i = 0,j = 0,N0 = 0,N1 = 0,flag = 0;
	float T0,T1,T2,T_center;
	uint32_t S0 = 0,S1 = 0;
	T2 = BlackThres;
	do{
		for(i=0;i<ROW_H;i++)
                {
		  for(j=0;j<COL_W;j++)
                   {
			if(image[i][j] < T2)
                         {
                            S0 += image[i][j];
                            N0++;       //���ص���
			 }
			else
                         {
                            S1 += image[i][j];
                            N1++;       //���ص���
			 }
	           }
		}

		T0 = S0/N0;
		T1 = S1/N1;
		T_center = (T0+T1)/2;

		if(T2 < T_center)
                {
			if((T_center - T2)> ERROR)
                         {
			   flag = 1;
			 }
			else
                         {
			   flag = 0;
			 }
		}
		else
                {
			if((T2 - T_center) > ERROR)
                         {
			   flag = 1;
			 }
			else
                         {
			   flag = 0;
			 }
		}
		T2 =T_center;
		BlackThres = T2;
	}
	while(flag);

}
char Error_Transform(uint8 Data, uint8 Set_num)   //�����ֵ
{
	char Error;

	Error = Set_num - Data;
	if (Error < 0)
	{
		Error = -Error;
	}

	return Error;
}
/*
*	���㲹������
*
*	˵����ʹ�����㷨������ϳ��Ĳ�������
*/
int16 Calculate_Add(uint8 i, float Ka, float Kb)	// ���㲹������
{
	float res;
	int16 Result;

	res = i * Ka + Kb;
	Result = range_protect((int32)res, 1, COL_W-1);

	return Result;
}
/*
*	���㷨��ֱ��
*
*	˵�������ֱ�� y = Ka * x + Kb   Mode == 1������߽磬Mode == 2�����ұ߽�
*/
void Curve_Fitting(float *Ka, float *Kb, int16 *Start, int16 *Line, int16 *Add_Flag, int16 Mode)
{
	*Start += 2;
	if (Add_Flag[*Start] == 1)
	{
		if (*Start <= 51)
		{
			*Start += 2;
		}
		if (Mode == 2)
		{
                        *Ka = 1.0*(Line[*Start+2] - Line[*Start]) / 2;
			if (*Ka < 0)
			{
				*Ka = 0;
			}
		}
		if (Mode == 1)
		{
                        *Ka = 1.0*(Line[*Start+2] - Line[*Start]) / 2;
			if (*Ka > 0)
			{
				*Ka = 0;
			}
		}
	}
	else
	{
            if(Mode == 2)
                *Ka = 1.0*(Line[*Start+2] - Line[*Start]) / 2;
            if(Mode == 1)
                *Ka = 1.0*(Line[*Start+2] - Line[*Start]) / 2;
	}
	*Kb = 1.0*Line[*Start] - (*Ka * (*Start));
}
/*
*	���㷨��ֱ��
*
*	˵�������ֱ�� y = Ka * x + Kb   Mode == 1������߽磬Mode == 2�����ұ߽�
*/
void Curve2_Fitting(float *Ka, float *Kb, uint8 Start,uint8 End, int16 *Line, int16 Mode,int16 num)
{
    if (Mode==1)
    {
        *Ka = 1.0*((Line[Start]+num) - Line[End]) / (Start-End);
        *Kb = 1.0*Line[End] - (*Ka * End);
    }
    else
    {
        *Ka = 1.0*((Line[Start]-num) - Line[End]) / (Start-End);
        *Kb = 1.0*Line[End] - (*Ka * End);
    }

}
/*
*	���㷨��ֱ��
*
*	˵�������ֱ�� y = Ka * x + Kb   Mode == 1������߽磬Mode == 2�����ұ߽�
*/
void Curve3_Fitting(float *Ka, float *Kb, uint8 Start,uint8 End, int16 *Line, int16 Mode)
//�������ר�ã������������
{
    if (Mode==1)
    {
        *Ka = 1.0*((Line[Start]) - Line[End]) / (Start-End); //б��
        *Kb = 1.0*Line[End] - (*Ka * End);   //�ؾ�
    }
    else
    {
        *Ka = 1.0*((Line[Start]) - Line[End]) / (Start-End);
        *Kb = 1.0*Line[End] - (*Ka * End);
    }

}
void Curve2_Fitting_Island(float *Ka, float *Kb, uint8 Start,uint8 End, int16 *Left,int16 *Right, int16 Mode,uint8 power)//��������ʹ��
{
        if (Mode==1)
        {
             *Ka = 1.0*((Left[Start]) - Right[End] +power) / (Start-End);
             *Kb = 1.0*Right[End] - (*Ka * End);
        }
        else
        {
              *Ka = 1.0*((Right[Start]) - Left[End] -power)/ (Start-End);
              *Kb = 1.0*Left[End] - (*Ka * End);
        }
}

/***********************************************************************
****�����������м������������� ******************************************
****˵����������ʹ�ú󽫱���߽�����
*         Mid��һ�����ߵ�ֵ        Left_Min�����Сֵ  Right_Max�ұ����ֵ
*         Left_Lineʵ����߽�      Right_Lineʵ���ұ߽�
*         Left_Add_Line������߽�  Right_Add_Line�����ұ߽�
************************************************************************/
void Traversal_Mid_Line(int16 i,uint8 (*data)[COL_W],
                        int16 Mid, int16 Left_Min, int16 Right_Max,
                        int16 *Left_Line, int16 *Right_Line,
                        int16 *Left_Add_Line, int16 *Right_Add_Line)
{
    int16 j;
    Left_Add_Flag[i] = 1;	// ��ʼ�����߱�־λ
    Right_Add_Flag[i] = 1;

    Right_Line[i] = Right_Max;
    Left_Line[i] = Left_Min;	// �����߽��ʼֵ

    for (j = Mid; j >= Left_Min+1; j--)	// ��ǰһ���е�Ϊ���������ұ߽�
    {
        if(((data[i][j+1] - data[i][j-1]) > EdgeThres)
           && ((data[i][j] - data[i][j-2]) > EdgeThres)
             && (data[i][j-1] < BlackThres+20))
            {
                Left_Add_Flag[i] = 0;	//��߽粻��Ҫ���ߣ������־λ
                Left_Line[i] = j;       //��¼��ǰjֵΪ����ʵ����߽�
                Left_Add_Line[i] = j;	//��¼ʵ����߽�Ϊ������߽�
                break;
            }
    }
    for (j = Mid; j <= Right_Max-1; j++)	// ��ǰһ���е�Ϊ������Ҳ����ұ߽�
    {
	if(((data[i][j-1] - data[i][j+1]) > EdgeThres)
           && ((data[i][j] - data[i][j+2]) > EdgeThres)
             && (data[i][j+1] < BlackThres+20))	//��⵽�ڵ�
            {
                Right_Add_Flag[i] = 0;		//�ұ߽粻��Ҫ���ߣ������־λ
                Right_Line[i] = j;	//��¼��ǰjֵΪ�����ұ߽�
                Right_Add_Line[i] = j;	// ��¼ʵ���ұ߽�Ϊ������߽�
                break;
            }
    }

    /************************��������ѭ���� ����Ƿ���Ҫ����**************************/

    if(Left_Add_Flag[i])	// ��߽���Ҫ����
	{
		if (i >= 55)	// ǰ6��
		{
			Left_Add_Line[i] = Left_Line[59];	// ʹ�õ������� �����ر����׶���
		}
		else                     ///
		{
			Left_Add_Line[i] = Left_Add_Line[i+2];	// ʹ��ǰ2����߽���Ϊ������߽�
		}
	}
    if(Right_Add_Flag[i])	// �ұ߽���Ҫ����
	{
		if (i >= 55)	// ǰ6��
		{
			Right_Add_Line[i] = Right_Line[59];	// ʹ�õ�������   �����ر����׶���
		}
		else         //////
		{
			Right_Add_Line[i] = Right_Add_Line[i+2];	// ʹ��ǰ2���ұ߽���Ϊ�����ұ߽�
		}
	}
	Width_Real[i] = Right_Line[i] - Left_Line[i];		// ����ʵ���������
	Width_Add[i] = Right_Add_Line[i] - Left_Add_Line[i];	// ���㲹���������
}
/*
*	����࿪ʼ�����߽磬�����������
*
*	˵����������������Ϊ̽������ʹ�ã�������������ȣ�������߽�����
*/
int16 Traversal_Left(int16 i,uint8 (*data)[COL_W], int16 *Mid, int16 Left_Min, int16 Right_Max)
{
	int16 j, White_Flag = 0;
	int16 Left_Line = Left_Min, Right_Line = Right_Max;

	for (j = Left_Min; j <= Right_Max; j++)	// �߽� 1��93
	{
		if (!White_Flag)   // �Ȳ�����߽� !White_Flag = 1
		{
                      if(data[i][j] > BlackThres)   //��⵽�׵�
			{
				Left_Line = j;	// ��¼��ǰjֵΪ������߽�
				White_Flag = 1;	// ��߽����ҵ��������ұ߽�

				continue;	// ��������ѭ���������´�ѭ��
			}
		}
		else    // White_Flag = 1 ��߽����ҵ�����ʼѰ���ұ߽�
		{
                      if(data[i][j] < BlackThres)   //��⵽�ڵ�
			{
				Right_Line = j-1;//��¼��ǰjֵΪ�����ұ߽�
				break;	// ���ұ߽綼�ҵ�������ѭ��
			}
		}
	}

	if (!White_Flag)	// White_Flag = 0 ��û���ҵ���߽�
	{                       //�ұ߽���Ȼû�� ����0
		return 0;
	}
	else                    // �ҵ����ұ߽� �������ߺͿ��
	{
		*Mid = (Right_Line + Left_Line) / 2;
		return (Right_Line - Left_Line);
	}
}
/*
*	���Ҳ࿪ʼ�����߽磬�����������
*
*	˵����������������Ϊ̽������ʹ�ã�������������ȣ�������߽�����
*/
int16 Traversal_Right(int16 i,uint8 (*data)[COL_W], int16 *Mid, int16 Left_Min, int16 Right_Max)
{
	int16 j, White_Flag = 0;
	int16 Left_Line = Left_Min, Right_Line = Right_Max;

	for (j = Right_Max; j >= Left_Min; j--)	// �߽� 93��1
	{
		if (!White_Flag)	// �Ȳ����ұ߽�
		{
			if(data[i][j] > BlackThres)	// ��⵽�׵�
			{
				Right_Line = j;	// ��¼��ǰjֵΪ�����ұ߽�
				White_Flag = 1;	// �ұ߽����ҵ���������߽�

				continue;	// ��������ѭ���������´�ѭ��
			}
		}
		else
		{
			if(data[i][j] < BlackThres)	//���ڵ�
			{
				Left_Line = j+1;	//��¼��ǰjֵΪ������߽�

				break;	// ���ұ߽綼�ҵ�������ѭ��
			}
		}
	}
	if (!White_Flag)	// δ�ҵ����ұ߽�
	{
		return 0;
	}
	else
	{
		*Mid = (Right_Line + Left_Line) / 2;

		return (Right_Line - Left_Line);
	}
}
/*
*	����࿪ʼ�����߽磬���������߽磬����1�ɹ� 0ʧ��
*
*	˵����������ʹ�ú󽫱���߽�����
*/
int16 Traversal_Left_Line(int16 i,uint8 (*data)[COL_W], int16 *Left_Line, int16 *Right_Line)
{
	int16 j, White_Flag = 0;

	Left_Line[i] = 1;
	Right_Line[i] = 93;

	for (j = 1; j < 94; j++)	// �߽����� 1��79
	{
		if (!White_Flag)	// �Ȳ�����߽�
		{
			if(data[i][j] > BlackThres)	// ��⵽�׵�
			{
				Left_Line[i] = j;	// ��¼��ǰjֵΪ������߽�
				White_Flag = 1;		// ��߽����ҵ��������ұ߽�

				continue;	// ��������ѭ���������´�ѭ��
			}
		}
		else
		{
			if(data[i][j] < BlackThres)	//���ڵ�
			{
				Right_Line[i] = j-1;//��¼��ǰjֵΪ�����ұ߽�

				break;	// ���ұ߽綼�ҵ�������ѭ��
			}
		}
	}
	if (White_Flag)
	{
		Left_Add_Line[i] = Left_Line[i];
		Right_Add_Line[i] = Right_Line[i];
		Width_Real[i] = Right_Line[i] - Left_Line[i];
		Width_Add[i] = Width_Real[i];
	}

	return White_Flag;	// �����������
}

/*
*	���Ҳ࿪ʼ�����߽磬���������߽磬����1�ɹ� 0ʧ��
*
*	˵����������ʹ�ú󽫱���߽�����
*/
int16 Traversal_Right_Line(int16 i,uint8 (*data)[COL_W], int16 *Left_Line, int16 *Right_Line)
{
	int16 j, White_Flag = 0;

	Left_Line[i] = 1;
	Right_Line[i] = COL_W-1;

	for (j = COL_W-1; j > 0; j--)	// �߽����� 1��79
	{
		if (!White_Flag)	// �Ȳ����ұ߽�
		{
			if(data[i][j] > BlackThres)	// ��⵽�׵�
			{
				Right_Line[i] = j;	// ��¼��ǰjֵΪ�����ұ߽�
				White_Flag = 1;		// �ұ߽����ҵ���������߽�

				continue;	// ��������ѭ���������´�ѭ��
			}
		}
		else
		{
			if(data[i][j] < BlackThres)	//���ڵ�
			{
				Left_Line[i] = j+1;//��¼��ǰjֵΪ������߽�

				break;	// ���ұ߽綼�ҵ�������ѭ��
			}
		}
	}
	if (White_Flag)
	{
		Left_Add_Line[i] = Left_Line[i];
		Right_Add_Line[i] = Right_Line[i];
		Width_Real[i] = Right_Line[i] - Left_Line[i];
		Width_Add[i] = Width_Real[i];
	}
	return White_Flag;	// �����������
}
/*
*	ͼ���㷨������ʼ��
*
*	˵������Ӱ���һ�����⴦��     94��
*/
void Image_Para_Init(void)
{
	Mid_Line[61] = MIDVALUE;
	Left_Line[61] = 1;
	Right_Line[61] = 93;
	Left_Add_Line[61] = 1;
	Right_Add_Line[61] = 93;
	Width_Real[61] = 92;  //93-1
	Width_Add[61] = 92;
}
/*******************************************************************************
*�������ܣ���һ�����⴦��
*����˵������ʹ�õ�60���е���Ϊ��59��(��һ��)������ʼλ�ã��ɹ����������ұ߽��
           ����59���е㸳ֵ����60�б�����һ֡ͼ��ʹ�á������60���е��ڱ���Ϊ�ڵ㣬��
           �ֱ�ʹ����������ұ����ķ��������߽磬��������Ƚϴ�Ľ����Ϊ��59�б߽磬
           ����Ȼ���������߽�������쳣��Ϊ���磬���緵��0
*******************************************************************************/
int16 First_Line_Handle(uint8 (*data)[COL_W])
{
	int16 i,res;	// ������
	int16 Weight_Left, Weight_Right;	// �����������
	int16 Mid_Left, Mid_Right;
	Image_Para_Init();           //ͼ���㷨������ʼ��
	i = 59;  //��һ��

        res = Starting_Check(i, data, 1, COL_W-1);	// ʹ�ø�ʴ�˲��㷨�ȶԱ�����������Ԥ�����������������
	Jump[59] = res;

        if((data[i][Mid_Line[61]] < BlackThres)
           || (data[i][Mid_Line[61]-2] < BlackThres)
             || (data[i][Mid_Line[61]+2] < BlackThres)
               || (data[i][Mid_Line[61]+4] < BlackThres)
                 || (data[i][Mid_Line[61]+6] < BlackThres))
	  {
              Weight_Left = Traversal_Left(i, data, &Mid_Left, 1, COL_W-1);	// ����������߽�
              Weight_Right = Traversal_Right(i, data, &Mid_Right, 1, COL_W-1);// ���Ҳ������߽�
              if (Weight_Left >= Weight_Right && Weight_Left)	// ��������ȴ�������������Ҳ�Ϊ0
              {
                Traversal_Left_Line(i, data, Left_Line, Right_Line);	// ʹ���������ȡ�����߽�
              }
              else if (Weight_Left < Weight_Right && Weight_Right)
              {
                Traversal_Right_Line(i, data, Left_Line, Right_Line);	// ʹ���ұ�����ȡ�����߽�
              }
              else	// ˵��û�鵽
              {
                return 0;
              }

	  }
	else
	{
          Traversal_Mid_Line(i, data,
                             Mid_Line[i+2], 1, COL_W-1,
                             Left_Line, Right_Line,
                             Left_Add_Line, Right_Add_Line);	// ��ǰһ���е�������ɨ��
	}

	Left_Line[61] = Left_Line[59];
	Right_Line[61] = Right_Line[59];
	Left_Add_Line[61] = Left_Add_Line[59];
	Right_Add_Line[61] = Right_Add_Line[59];

	if (Left_Add_Flag[59] && Right_Add_Flag[59])
	{
		Mid_Line[59] = Mid_Line[61];
	}
	else
	{
		Mid_Line[59] = (Right_Line[59] + Left_Line[59]) / 2;
		Mid_Line[61] = Mid_Line[59];	// ���µ�60�������е㣬������һ֡ͼ��ʹ��
	}
	Width_Real[61] = Width_Real[59];
	Width_Add[61] = Width_Add[59];
	Width_Min = Width_Add[59];

	return 1;
}
/*
*	�����޸�
*
*	˵������ʼ���ղ�ʹ�ã�ֱ��ʹ������б�ʽ��в���
*/
void Line_Repair(uint8 Start, uint8 Stop,uint8 (*data)[COL_W], int16 *Line, int16 *Line_Add, int16 *Add_Flag, int16 Mode)
{
	float res;
	int16 i;	// ������
	float Ka, Kb;

	if ((Mode == 1) && (Right_Add_Start <= Stop) && Start <= 53)	// ��߽粹��
	{
		if (Start <= 57)
		{
			Start +=2;
		}
		for (i = Start+2; i >= Stop+2;)
		{
			i -= 2;
			Line_Add[i] = range_protect(Right_Add_Line[i] - Width_Add[i+2], 1, COL_W-1);
			Width_Add[i] = Width_Add[i+2];
		}
	}
	if ((Mode == 2) && (Left_Add_Start <= Stop) && Start <= 53)	// �ұ߽粹��
	{
		if (Start <= 57)
		{
			Start +=2;
		}
		for (i = Start+2; i >= Stop+2;)
		{
			i -= 2;
			Line_Add[i] = range_protect(Left_Add_Line[i] + Width_Add[i+2], 1, COL_W-1);
			Width_Add[i] = Width_Add[i+2];
		}
	}
	else
	{
		if (Stop)	// ��ʼ����
		{
			if ((Right_Add_Stop >= MIDVALUE && Left_Add_Stop >= MIDVALUE) || (Right_Add_Stop >= MIDVALUE && Left_Add_Start <= Right_Add_Stop) || (Left_Add_Stop >= MIDVALUE && Right_Add_Start <= Left_Add_Stop))	// ֻ�н�������Ҫ���ߣ�������б�ʣ�ֱ����ֱ���²���
			{
				for (i = Stop-2; i <= 57; )
				{
					i += 2;
					Line_Add[i] = Line_Add[Stop];
				}
			}
			else	// ����ʼ�кͽ����м���б�ʲ���
			{
				if (Start <= 57)
				{
					Start +=2;
				}
				Ka = 1.0*(Line_Add[Start] - Line_Add[Stop]) / (Start - Stop);
				Kb = 1.0*Line_Add[Start] - (Ka * Start);

				for (i = Stop; i <= Start; )
				{
					i += 2;
					res = i * Ka + Kb;
					Line_Add[i] = range_protect((int32)res, 1, COL_W-1);
				}
			}
		}
	}
}
/*
*	�����޸�
*
*	˵������ʼ���ղ�ʹ�ã�ֱ��ʹ������б�ʽ��в���
*/
void Line_Repair_island(uint8 Start, uint8 Stop,uint8 (*data)[COL_W], int16 *Line, int16 *Line_Add, int16 *Add_Flag, int16 Mode)
{
	float res;
	int16 i;	// ������
	float Ka, Kb;

	if ((Mode == 1) && (Right_Add_Start <= Stop) && Start <= 53)	// ��߽粹��
	{
		if (Start <= 57)
		{
			Start +=2;
		}
		for (i = Start+2; i >= Stop+2;)
		{
			i -= 2;
			Line_Add[i] = range_protect(Right_Add_Line[i] - Width_Add[i+2], 1, COL_W-1);
			//Width_Add[i] = Width_Add[i+2]-3;
		}
	}
	if ((Mode == 2) && (Left_Add_Start <= Stop) && Start <= 53)	// �ұ߽粹��
	{
		if (Start <= 57)
		{
			Start +=2;
		}
		for (i = Start+2; i >= Stop+2;)
		{
			i -= 2;
			Line_Add[i] = range_protect(Left_Add_Line[i] + Width_Add[i+2], 1, COL_W-1);
			//Width_Add[i] = Width_Add[i+2]-3;
		}
	}
	else
	{
		if (Stop)	// ��ʼ����
		{
			if ((Right_Add_Stop >= MIDVALUE && Left_Add_Stop >= MIDVALUE) || (Right_Add_Stop >= MIDVALUE && Left_Add_Start <= Right_Add_Stop) || (Left_Add_Stop >= MIDVALUE && Right_Add_Start <= Left_Add_Stop))	// ֻ�н�������Ҫ���ߣ�������б�ʣ�ֱ����ֱ���²���
			{
				for (i = Stop-2; i <= 57; )
				{
					i += 2;
					Line_Add[i] = Line_Add[Stop];
				}
			}

			else	// ����ʼ�кͽ����м���б�ʲ���
			{
				if (Start <= 57)
				{
					Start +=2;
				}
				Ka = 1.0*(Line_Add[Start] - Line_Add[Stop]) / (Start - Stop);
				Kb = 1.0*Line_Add[Start] - (Ka * Start);

				for (i = Stop; i <= Start; )
				{
					i += 2;
					res = i * Ka + Kb;
					Line_Add[i] = range_protect((int32)res, 1, COL_W-1);
				}
			}
		}
	}
}
void Mid_Line_Repair(uint8 count,uint8 (*data)[COL_W])//�����޸�
{
	uint8 i;	// ������
        Left_Max = 0;
	Right_Min = COL_W-1;

	for (i = 59; i >= count;i-=2)
	{
		Mid_Line[i] = (Right_Add_Line[i] + Left_Add_Line[i]) / 2;	// ���������е�
		Width_Add[i] = Right_Add_Line[i] - Left_Add_Line[i];		// �����������
        if (i>21)
        {
            if (Left_Add_Line[i] > Left_Max)
            {
                Left_Max = Left_Add_Line[i];
            }
            if (Right_Add_Line[i] < Right_Min)
            {
                Right_Min = Right_Add_Line[i];
            }
        }
//        //�ϰ�����
//        if(Left_Hazard_Flag)
//        {
//            Curve2_Fitting(&Left_Ka, &Left_Kb, 59,Left_Hazard_Flag, Left_Line, 1,21);
//            for (j=59;j>=Left_Hazard_Flag;j--)
//            {
//                Left_Add_Line[j] = Calculate_Add(j, Left_Ka,Left_Kb);	// �������
//                Mid_Line[j] = (Right_Add_Line[j] + Left_Add_Line[j]) / 2;
//            }
//        }
//        if(Right_Hazard_Flag)
//        {
//            Curve2_Fitting(&Right_Ka, &Right_Kb, 59,Right_Hazard_Flag, Right_Line, 2,21);
//            for (j=59;j>=Right_Hazard_Flag;j--)
//            {
//                Right_Add_Line[j] = Calculate_Add(j, Right_Ka, Right_Kb);	// �������
//                Mid_Line[j] = (Right_Add_Line[j] + Left_Add_Line[j]) / 2;
//            }
//
//        }
	}

	Mid_Line[61] = Mid_Line[59];
}
/*
*	ͼ�����㷨
*
*	˵����������ͨͼ�񣬰���ʮ��,�ϰ�,��·��·��
*/
void Image_Handle(uint8 (*data)[COL_W])
{
    int16 i,j;	// ������
    int16 res;	// ���ڽ��״̬�ж�

    Line_Count = 0;	// ����������λ

    Left_Hazard_Flag = 0;	// ��λ�����ϰ����־λ
    Right_Hazard_Flag = 0;
    Hazard_Flag=0;

    Left_Add_Start = 0;		// ��λ������ʼ������
    Right_Add_Start = 0;
    Left_Add_Stop = 0;
    Right_Add_Stop = 0;

    uint8 Limit_Left, Limit_Right;    //���������Ҽ���
    /**************************** ��ʼ�� *************************************/
    for (i = 59; i >= 19;i-=2)  // ���д�����С��Ƭ������
    {
        Right_Line[i] = 93;     //�ұ߽��ʼֵ
        Left_Line[i] = 1;	// ��߽��ʼֵ
        Left_Add_Flag[i] = 1;	// ��ʼ�����߱�־λ
        Right_Add_Flag[i] = 1;  //ͬ��
    }
    /***************************** ��һ�����⴦�� *****************************/
	res = First_Line_Handle(data);
    /***************************** ��һ�����⴦�� *****************************/
	if (res == 0)
	{
		Out_Side = 1;	// ����
		return;
	}
	Out_Side = 0;
	Line_Count = 59;

    /****************************** ͼ���� **********************************/
    for (i = 59; i >= 19;)	// ������ǰ40��ͼ�񣬸��к������20������
    {
        i -= 2;	                // ���д�����С��Ƭ������

        /***********************�����߼��*****************************/
        if (Left_Add_Flag[i+2])
        {
                Limit_Left = Left_Line[i+2];
        }
        else
        {
                if (Jump[i] >= 2)
                {
                        Limit_Left = Left_Add_Line[i+2];
                }
                else
                {
                        Limit_Left = Left_Add_Line[i+2]+1;
                }

        }

        if (Right_Add_Flag[i+2])
        {
                Limit_Right = Right_Line[i+2];
        }
        else
        {
                if (Jump[i] >= 2)
                {
                        Limit_Right = Right_Add_Line[i+2];
                }
                else
                {
                        Limit_Right = Right_Add_Line[i+2]-1;
                }
        }
        Jump[i] = Starting_Check(i, data, Limit_Left, Limit_Right); // ʹ�ø�ʴ�˲��㷨�ȶԱ�����������Ԥ�����������������
        if (Jump[i] >= 5 && i>= 21)
        {
                  Stop_Flag = 1;
        }

        if(data[i][Mid_Line[i+2]] < (BlackThres+5)
           &&data[i][Mid_Line[i+2]-2] < (BlackThres+5)
             &&data[i][Mid_Line[i+2]+2] < (BlackThres+5))
             //ǰ2���е��ڱ���Ϊ�ڵ㣬����������������Ҳ�����ǻ�·
        {
           i+=2;
           break;
        }
        else	// ʹ��ǰ2���е�������ɨ��߽�
        {
           Traversal_Mid_Line(i, data, Mid_Line[i+2], 1,COL_W-1,
                              Left_Line, Right_Line,
                              Left_Add_Line, Right_Add_Line);
        }
        /**************************** ���߼�⿪ʼ ****************************/
        if (Width_Real[i] > Width_Min+1)	// ������ȱ��������ʮ�ֻ�·��
        {                                       //Width_Min��ʵ�ǵ�һ���������Ҳ����
                                                //û�н������ν���ͼ�����������
            if (Left_Add_Line[i] <=  Left_Add_Line[i+2]) //������������Զ�����խ��Left_Add_Line[i]
                                                         //�ȽϽ����Ĵ�
			{
				if (!Left_Add_Flag[i])    //��������Ϊ0
				{
					Left_Add_Flag[i] = 1;	// ǿ���϶�Ϊ��Ҫ����
				}
			}
	    if (Right_Add_Line[i] >= Right_Add_Line[i+2])  //�������෴
			{
				if (!Right_Add_Flag[i])   //��������Ϊ0
				{
					Right_Add_Flag[i] = 1;	// ǿ���϶�Ϊ��Ҫ����
				}
			}

	    if (Left_Add_Flag[i] || Right_Add_Flag[i])
			{
				if (Left_Add_Stop || Right_Add_Stop)
				{
					break;
				}
			}
        }
        /*************************** ��һ�ֲ��߿�ʼ ***************************/
		if (Left_Add_Flag[i])	// �����Ҫ����
		{
			if (i >= 55)	// ǰ���в��߲���
			{
				if (!Left_Add_Start)
				{
					Left_Add_Start = i;	// ��¼���߿�ʼ��
					Left_Ka = 0;
					Left_Kb = Left_Add_Line[i];
				}
				Left_Add_Line[i] = Calculate_Add(i, Left_Ka, Left_Kb);	// ʹ��ǰһ֡ͼ����߽�б�ʲ���
			}
			else
			{
				if (!Left_Add_Start)	// ֮ǰû�в���
				{
					Left_Add_Start = i;	// ��¼��ಹ�߿�ʼ��
					Curve_Fitting(&Left_Ka, &Left_Kb, &Left_Add_Start, Left_Add_Line, Left_Add_Flag, 1);	// ʹ�����㷨���ֱ��
				}
				Left_Add_Line[i] = Calculate_Add(i, Left_Ka, Left_Kb);	// �������
			}
		}
		else
		{
			if (Left_Add_Start)	// �Ѿ���ʼ����
			{
				if (!Left_Add_Stop
                                    && !Left_Add_Flag[i+2]
                                      && !Left_Add_Flag[i+4])
				{
					if (Left_Add_Line[i] >= Left_Add_Line[i+2]
                                            && Left_Add_Line[i+2] >= Left_Add_Line[i+4])
					{
						Left_Add_Stop = i+4;	// ��¼��ಹ�߽�����
                                                if(Ramp_Flag == 0)
                                                  Line_Repair(Left_Add_Start, Left_Add_Stop, data, Left_Line, Left_Add_Line, Left_Add_Flag, 1);
					}
				}
			}
		}

		if (Right_Add_Flag[i])	// �Ҳ���Ҫ����
		{
			if (i >= 55)	// ǰ���в��߲���
			{
				if (!Right_Add_Start)
				{
					Right_Add_Start = i;	// ��¼���߿�ʼ��
					Right_Ka = 0;
					Right_Kb = Right_Add_Line[i];
				}
				Right_Add_Line[i] = Calculate_Add(i, Right_Ka, Right_Kb);	// ʹ��ǰһ֡ͼ���ұ߽�б�ʲ���
			}
			else
			{
				if (!Right_Add_Start)	// ֮ǰû�в���
				{
					Right_Add_Start = i;	// ��¼�Ҳಹ�߿�ʼ��
					Curve_Fitting(&Right_Ka, &Right_Kb, &Right_Add_Start, Right_Add_Line, Right_Add_Flag, 2);	// ʹ�����㷨���ֱ��
				}
				Right_Add_Line[i] = Calculate_Add(i, Right_Ka, Right_Kb);	// �������
			}
		}
		else
		{
			if (Right_Add_Start)	// �Ѿ���ʼ����
			{
				if (!Right_Add_Stop && !Right_Add_Flag[i+2] && !Right_Add_Flag[i+4])
				{
					if (Right_Line[i] <= Right_Line[i+2] && Right_Line[i+2] <= Right_Line[i+4])
					{
						Right_Add_Stop = i+4;	// ��¼�Ҳಹ�߽�����
                                                if(Ramp_Flag == 0) Line_Repair(Right_Add_Start, Right_Add_Stop, data, Right_Line, Right_Add_Line, Right_Add_Flag, 2);
					}
				}
			}
		}
		/*************************** ��һ�ֲ��߽��� ***************************/
                Width_Add[i] = Right_Add_Line[i] - Left_Add_Line[i];	// ���¼����������
		if ((Left_Add_Flag[i] && Right_Add_Flag[i]) || (!Left_Add_Flag[i] && !Right_Add_Flag[i]))
		{
			Width_Add[i] = Right_Add_Line[i] - Left_Add_Line[i];	// ���¼����������
			Mid_Line[i] = (Right_Add_Line[i] + Left_Add_Line[i]) / 2;	// ��������
		}
		else
		{
			Width_Add[i] = Right_Add_Line[i] - Left_Add_Line[i];	// ���¼����������
			Mid_Line[i] = Mid_Line[i+2];
		}
		if (Width_Add[i] < Width_Min)
		{
			Width_Min = Width_Add[i];	// ������С�������
		}

                Line_Count = i;                          //Line_Count����

                if(Left_Add_Stop&&Right_Add_Stop)    //���߽�������Ҫ�����ⲻ��
                {
                    if(Left_Add_Flag[i]||Right_Add_Flag[i])
                    {
                        break;
                    }
                }
    }
    /*****************************�������***********************************/
    if(fand_ring_time == 0) //������
    {
        fand_left_ring();//�󻷵����
        fand_right_ring();//�һ������
    }
    if((flag1 == 1 || flag2 == 1)&& is_diu_flag == 5) //�����г�����
    {
        if(flag1 == 1)
        {
           out_ring_check(1);
        }
        else
        {
           out_ring_check(2);
        }
    }
     /*****************************����������*****************************/

    /******************************�µ����*********************************/
   // if(Ramp_Time_Count == 0)
    //{
    //     Ramp_Check();//�����µ����
   // }
    /****************************** �����޸���ʼ ****************************/
     Mid_Line_Repair(Line_Count, data);

     if(island_left_mid_add_flag)
      {
         for(int mid_count = 21; mid_count <= 59; mid_count+=2)
         {
            Mid_Line[mid_count] = Right_Line[mid_count]-Island_Width_Add[mid_count]/2-1;
         }
      }
     if(island_right_mid_add_flag)
      {
         for(int mid_count = 21; mid_count <= 59; mid_count+=2)
         {
            Mid_Line[mid_count] = Left_Line[mid_count]+Island_Width_Add[mid_count]/2+1;
         }
      }

}
/*
*	��Ȩƽ��
*
*	˵����Ȩ�����Ҵյģ�Ч������
*/
uint8 Line_Num=0;
uint8 Dynamic_Point_Weight(int8 Num)
{
    uint8 i,Point, Point_Mid;
    uint8 Count = 0;
    int8   Point_id;
    int32 Sum = 0, Weight_Count = 0;
    uint8 tep=0,L_Min=MIDVALUE,R_Max=MIDVALUE;

    if (Out_Side || Line_Count >= 57)	//�����������ͷͼ���쳣
	{
	  Point  = Last_Point;
	}
     if(1)
     {
        if (Line_Count <= 20)
         {
            Line_Count = 21;
         }
         Count = 60-Line_Count;

         for (i = 59; i >= Line_Count-Num;i-=2)		//ʹ�ü�Ȩƽ��
	   {
                     Point_id = i-Num;
                     if (Point_id>59)Point_id=59;
		       Sum += Mid_Line[Point_id] * Weight[59-i];
		     Weight_Count += Weight[59-i];
            }
	Point = range_protect(Sum / Weight_Count, 2, 92);//���Բ��޷�����

#if   SSSSSSS_OFF_ON
    S_temp=0;
    for (i = 57; i >= Line_Count+2; )		//���СS���
    {
        i -= 2;
        if(tep==0)
        {
            if (Mid_Line[i]>(Mid_Line[i+4]+1))
            {
                tep=1;
                S_temp++;
            }
            else if ((Mid_Line[i]+1)<Mid_Line[i+4])
            {
                tep=2;
                S_temp++;
            }
        }
        if (tep==1)
        {
            if ((Mid_Line[i]+1)<Mid_Line[i+2])
            {
                tep=2;
                S_temp++;
            }
        }
        if (tep==2)
        {
            if (Mid_Line[i]>(Mid_Line[i+2]+1))
            {
                tep=1;
                S_temp++;
            }
        }

    }
    if (S_temp>=3 && island_flag != 0)
    {
        for (i = 61; i >= Line_Count+2; )		//����СS���ƫ��
        {
            i -= 2;
            if(L_Min> Mid_Line[i])
            {
                L_Min = Mid_Line[i];
            }
            if (R_Max < Mid_Line[i])
            {
                R_Max = Mid_Line[i];
            }
        }
        Point = (R_Max + L_Min) / 2;
    }
#endif
	Point = range_protect(Point, 2, 92);
	Last_Point = Point;


        Point_Mid = Mid_Line[Line_Count+10];

     }

    Foresight =  0.8 * Error_Transform(Point_Mid, MIDVALUE)	//ʹ����Զ��ƫ��ͼ�Ȩƫ��ȷ��ǰհ
               + 0.2 * Error_Transform(Point,  MIDVALUE);//+0.1*Ek_compar;

    Last_Foresight=Foresight;

    return Point;
}
/****************** ���㷨 ******************/
/*
*	��Ȩƽ��(����)
*
*	˵����Ȩ�����Ҵյģ�Ч������
*/
uint8  Motor_Point_Weight(int8 Num)
{
    uint8 i ,Point,Point_Mid;
    uint8 Count=0;
    int8   Point_id;
    int32 Sum = 0, Weight_Count = 0;
    uint8 tep=0,L_Min=MIDVALUE,R_Max=MIDVALUE;

    if (Line_Count <= 15)
    {
        Line_Count = 15;
    }
    Count = 60-Line_Count;
    if(Count <= Num)
    {

    }
    for (i = 59; i >= Line_Count-Num;i-=2)		//ʹ�ü�Ȩƽ��
    {
        Point_id = i-Num;
        if (Point_id>59)Point_id=59;
        Sum += Mid_Line[Point_id] * Weight[59-i];
        Weight_Count += Weight[59-i];
    }
    Point = range_protect(Sum / Weight_Count, 2, 92);

#if   SSSSSSS_OFF_ON
    S_temp=0;
    for (i = 57; i >= Line_Count+2; )		//���СS���
    {
        i -= 2;
        if(tep==0)
        {
            if (Mid_Line[i]>(Mid_Line[i+4]+1))
            {
                tep=1;
                S_temp++;
            }
            else if ((Mid_Line[i]+1)<Mid_Line[i+4])
            {
                tep=2;
                S_temp++;
            }
        }
        if (tep==1)
        {
            if ((Mid_Line[i]+1)<Mid_Line[i+2])
            {
                tep=2;
                S_temp++;
            }
        }
        if (tep==2)
        {
            if (Mid_Line[i]>(Mid_Line[i+2]+1))
            {
                tep=1;
                S_temp++;
            }
        }

    }
    if (S_temp>=3 && island_flag != 0)
    {
        for (i = 61; i >= Line_Count+2; )		//����СS���ƫ��
        {
            i -= 2;
            if(L_Min> Mid_Line[i])
            {
                L_Min = Mid_Line[i];
            }
            if (R_Max < Mid_Line[i])
            {
                R_Max = Mid_Line[i];
            }
        }
        Point = (R_Max + L_Min) / 2;
    }
#endif
    ////////////////////////
    Point = range_protect(Point, 2, 92);
    Last_Point = Point;


    Point_Mid = Mid_Line[Line_Count+2];


	return Point;
}
//************************************************************************************************************//
//************************************************************************************************************//
//************************************************************************************************************//
//************************************************************************************************************//
//************************************************************************************************************//
//************************************************************************************************************//

void fand_left_ring()
{
    int i=0;
    left_ring_flag = 0;
    for(i=45;i>=25;i-=2)
    {
        if(Left_Line[i]==1
           &&Left_Line[i+2]==1
             &&Left_Line[i+4]==1
               &&Left_Line[i+6]==1
                 &&Right_Line[i] != COL_W-1
                   &&Right_Line[i+2] != COL_W-1
                     &&Right_Line[i+4] != COL_W-1
                       &&Right_Line[i+6]!=COL_W-1)//��������ж���
        {
            left_ring_flag=1;
            break;
        }
    }
//    if(left_ring_flag==1 && g_AD_OUT[2]==100)
//    {
//        flag2 = 1;
//
//        if(flag2 == 1 && g_AD_OUT[1]==100)
//        {
//              island_flag = 1;
//              //led(LED2,LED_ON);
//              //island_right_mid_add_flag = 1;
//              go_island_set_value();
//        }
//    }
}
void fand_right_ring()
{
    int i=0;
    right_ring_flag = 0;
    for(i=45;i>=25;i-=2)
    {
        if(Right_Line[i]==COL_W-1
           &&Right_Line[i+2]==COL_W-1
             &&Right_Line[i+4]==COL_W-1
               &&Right_Line[i+6]==COL_W-1
                 &&Left_Line[i] != 1
                   &&Left_Line[i+2] != 1
                     &&Left_Line[i+4] != 1
                       &&Left_Line[i+6]!=1)//�ұ������ж���
        {
            right_ring_flag=1;
            break;
        }
    }
//    if(right_ring_flag==1 && g_AD_OUT[2]==100)
//    {
//        flag1 = 1;
//
//        if(flag1 == 1 && g_AD_OUT[1]==100)
//        {
//              island_flag = 2;
//              led(LED2,LED_ON);
//              //island_right_mid_add_flag = 1;
//              go_island_set_value();
//        }
//    }
}

void Left_island_Handle(uint8 (*data)[COL_W])
{
    int16 i;	// ������
	int16 res;	// ���ڽ��״̬�ж�
	Line_Count = 0;	// ����������λ

    Left_Add_Start = 0;		// ��λ������ʼ������
	Right_Add_Start = 0;
	Left_Add_Stop = 0;
	Right_Add_Stop = 0;

    for (i = 59; i >= 19;)
    {
        i -= 2;
        Left_Add_Flag[i] = 1;	// ��ʼ�����߱�־λ
        Right_Add_Flag[i] = 1;
        Right_Line[i] = 93;
        Left_Line[i] = 1;	// �����߽��ʼֵ
    }
    /***************************** ��һ�����⴦�� *****************************/
	res = First_Line_Handle(data);
    //	if (res == 0)
    //	{
    //		Out_Side = 1;	// ����
    //		return;
    //	}
	Out_Side = 0;
	Line_Count = 59;
    /*************************** ��һ�����⴦����� ***************************/
    for (i = 59; i >= 19;)	// ������ǰ40��ͼ�񣬸��к������20������
    {
        i -= 2;	// ���д�����С��Ƭ������
        if(data[i][Mid_Line[i+2]] < BlackThres)//ǰ2���е��ڱ���Ϊ�ڵ㣬����������������Ҳ�����ǻ�·
        {
            i += 2;
			break;
        }
        else	// ʹ��ǰ2���е�������ɨ��߽�
		{
			Traversal_Mid_Line(i, data, Mid_Line[i+2], 1,COL_W-1, Left_Line, Right_Line, Left_Add_Line, Right_Add_Line);
		}

        /**************************** ���߼�⿪ʼ ****************************/
        if (Width_Real[i] > Width_Min+1)//&&(Left_Add_Flag[i]||Right_Add_Flag[i]))	// ������ȱ��������ʮ�ֻ�·
        {
            if (Left_Add_Line[i] <=  Left_Add_Line[i+2])
			{
				if (!Left_Add_Flag[i])
				{
					Left_Add_Flag[i] = 1;	// ǿ���϶�Ϊ��Ҫ����
				}
			}
			if (Right_Add_Line[i] >= Right_Add_Line[i+2])
			{
				if (!Right_Add_Flag[i])
				{
					Right_Add_Flag[i] = 1;	// ǿ���϶�Ϊ��Ҫ����
				}
			}

			if (Left_Add_Flag[i] || Right_Add_Flag[i])
			{
				if (Left_Add_Stop || Right_Add_Stop)
				{
					break;
				}
			}
        }
        /*************************** ��һ�ֲ��߿�ʼ ***************************/
		if (Left_Add_Flag[i])	// �����Ҫ����
		{
			if (i >= 55)	// ǰ���в��߲���
			{
				if (!Left_Add_Start)
				{
					Left_Add_Start = i;	// ��¼���߿�ʼ��
					Left_Ka = 0;
					Left_Kb = Left_Add_Line[i];
				}
				Left_Add_Line[i] = Calculate_Add(i, Left_Ka, Left_Kb);	// ʹ��ǰһ֡ͼ����߽�б�ʲ���
			}
			else
			{
				if (!Left_Add_Start)	// ֮ǰû�в���
				{
					Left_Add_Start = i;	// ��¼��ಹ�߿�ʼ��
					Curve_Fitting(&Left_Ka, &Left_Kb, &Left_Add_Start, Left_Add_Line, Left_Add_Flag, 1);	// ʹ�����㷨���ֱ��
				}
				Left_Add_Line[i] = Calculate_Add(i, Left_Ka, Left_Kb);	// �������
			}
		}
		else
		{
			if (Left_Add_Start)	// �Ѿ���ʼ����
			{
				if (!Left_Add_Stop && !Left_Add_Flag[i+2])
				{
					if (Left_Add_Line[i] >= Left_Add_Line[i+2])
					{
						Left_Add_Stop = i;	// ��¼��ಹ�߽�����
					}
				}
			}
		}

		if (Right_Add_Flag[i])	// �Ҳ���Ҫ����
		{
			if (i >= 55)	// ǰ���в��߲���
			{
				if (!Right_Add_Start)
				{
					Right_Add_Start = i;	// ��¼���߿�ʼ��
					Right_Ka = 0;
					Right_Kb = Right_Add_Line[i];
				}
				Right_Add_Line[i] = Calculate_Add(i, Right_Ka, Right_Kb);	// ʹ��ǰһ֡ͼ���ұ߽�б�ʲ���
			}
			else
			{
				if (!Right_Add_Start)	// ֮ǰû�в���
				{
					Right_Add_Start = i;	// ��¼�Ҳಹ�߿�ʼ��
					Curve_Fitting(&Right_Ka, &Right_Kb, &Right_Add_Start, Right_Add_Line, Right_Add_Flag, 2);	// ʹ�����㷨���ֱ��
				}
				Right_Add_Line[i] = Calculate_Add(i, Right_Ka, Right_Kb);	// �������
			}
		}
		else
		{
			if (Right_Add_Start)	// �Ѿ���ʼ����
			{
				if (!Right_Add_Stop && !Right_Add_Flag[i+2])
				{
					if (Right_Line[i] <= Right_Line[i+2])
					{
						Right_Add_Stop = i;	// ��¼�Ҳಹ�߽�����
					}
				}
			}
		}
		/*************************** ��һ�ֲ��߽��� ***************************/
                Width_Add[i] = Right_Add_Line[i] - Left_Add_Line[i];	// ���¼����������
		if ((Left_Add_Flag[i] && Right_Add_Flag[i]) || (!Left_Add_Flag[i] && !Right_Add_Flag[i]))
		{
			Width_Add[i] = Right_Add_Line[i] - Left_Add_Line[i];	// ���¼����������
			Mid_Line[i] = (Right_Add_Line[i] + Left_Add_Line[i]) / 2;	// ��������
		}
		else
		{
			Width_Add[i] = Right_Add_Line[i] - Left_Add_Line[i];	// ���¼����������
			Mid_Line[i] = Mid_Line[i+2];
		}
		if (Width_Add[i] < Width_Min)
		{
			Width_Min = Width_Add[i];	// ������С�������
		}
		Line_Count = i;	// ��¼�ɹ�ʶ�𵽵���������

                if(Left_Add_Stop&&Right_Add_Stop)    //���߽�������Ҫ�����ⲻ��
                {
                    if(Left_Add_Flag[i]||Right_Add_Flag[i])
                    {
                        break;
                    }
                }
    }
    ///////////////////////////////////////////////////////////////
    if(island_flag == 1)
    {
            //***********************************************************************************//find_left_point
            uint8 image_tiaobian_flag = 0;
            uint8 find_point_1y = 0;
            for(i=59;i>=21;i-=2)
            {
                if(image_tiaobian_flag == 2)
                    break;
                image_tiaobian_flag = 0;
                for(int j = 15;j <= Right_Line[i];j++)
                {
                    if(!image_tiaobian_flag)
                    {
                        if(image[i][j-1] - image[i][j] > 20 && image[i][j-1] > (BlackThres))
                        {
                            image_tiaobian_flag = 1;
                            find_point_1y = j;
                        }
                    }
                    else
                    {
                        if(image[i][j] - image[i][j-1] > 20 && image[i][j] > (BlackThres)&& image[i-2][j] < (BlackThres+10))
                        {
                            if(j - find_point_1y < 15)
                            {
                                if(i%2==0)i=i-1;
                                find_left_point_x = i;
                                find_left_point_y = j;
                                image_tiaobian_flag = 2;
                                break;
                            }
                            else
                                break;
                        }
                    }
                }
            }
            Line_Count = find_left_point_x-2;
            Right_Line[find_left_point_x] = find_left_point_y;
            Curve2_Fitting(&Right_Ka, &Right_Kb, 59,find_left_point_x, Right_Line, 2,30);
            if(Right_Ka >= 0)
            {
                Last_Right_Ka = Right_Ka;
                Last_Right_Kb = Right_Kb;
            }
            else
            {
                Right_Ka = Last_Right_Ka;
                Right_Kb = Last_Right_Kb-10;
            }
            for(int j=59;j>=21;j-=2)//������find_left_point_x       �������ߣ������´����ߣ�����������
            {
                if(Line_Count >= 27)
                    Right_Add_Line[j] = Calculate_Add(j, Right_Ka, Right_Kb) - (Line_Count - 27);
                else
                    Right_Add_Line[j] = Calculate_Add(j, Right_Ka, Right_Kb);
                if(Right_Add_Line[j] < Left_Add_Line[j])
                {
                    Left_Add_Line[j] = 1;
                }
            }
            for(int j=find_left_point_x;j>=31;j-=2)
            {
                if((Right_Line[j] < find_left_point_y)&&(Right_Line[j-2] < Right_Line[j])&&(Right_Line[j-4] < Right_Line[j-2])&&(Right_Line[j-6] < Right_Line[j-4]))
                {
                    is_diu_flag = 3;
                    zhaoguidian_diu_flag_num = j;
                    break;
                }
            }
        if(is_diu_flag == 3)
        {
            for(int j=59;j>=Line_Count-2;j-=2)
            {
                if(Right_Line[j] < 80&&image[j][Right_Line[j]+7]>(BlackThres-5)&&image[j][Right_Line[j]+8] > (BlackThres-5))
                {
                    zhaoguidian_diu_flag_num = j;
                    break;
                }
            }
            for(int j=zhaoguidian_diu_flag_num;j>=21;j-=2)
            {
                if(Right_Line[j] > Right_Line[zhaoguidian_diu_flag_num])
                {
                    Line_Count = j+2;
                    break;
                }
            }
            Curve2_Fitting(&Right_Ka, &Right_Kb, 59,zhaoguidian_diu_flag_num, Right_Line, 2,27);
            if(Right_Ka >= 0)
            {
                Last_Right_Ka = Right_Ka;
                Last_Right_Kb = Right_Kb;
            }
            else
            {
                Right_Ka = Last_Right_Ka;
                Right_Kb = Last_Right_Kb-10;
            }
            for(i = 59; i >= zhaoguidian_diu_flag_num; i -= 2)
                Right_Add_Line[i] = Calculate_Add(i, Right_Ka, Right_Kb);
            for(i = Line_Count; i >= 21; i -= 2)//�������ߣ������´����ߣ�����������
                Right_Add_Line[i] = Right_Line[zhaoguidian_diu_flag_num];

            float g_fSlope2 = 0;
            float g_fSlope3 = 0;
            float g_fSlope4 = 0;

            g_fSlope2 = 1.0*(Right_Line[55] - Right_Line[51])/4;
            g_fSlope3 = 1.0*(Right_Line[53] - Right_Line[49])/4;
            g_fSlope4 = 1.0*(Right_Line[51] - Right_Line[47])/4;

            if((g_fSlope2 <= 2)
               && (g_fSlope3 <= 2)
                 && (g_fSlope4 <= 2)
                   && (g_fSlope2 >= 0)
                     && (g_fSlope3 >= 0)
                       && (g_fSlope4 >= 0))//�Ѿ����뻷�� ���ҵ����־������ ��һ��׼���ҳ������յ�
            {
                is_diu_flag = 4;
                tuqi_point_hang = 0;
            }

        }
        if(is_diu_flag == 4)
        {
            island_flag = 0;    //���������������־λֹͣ���ߡ�
            is_diu_flag = 5;     //�������߱�־λ
        }

    }
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*************************** �ڶ��ֲ����޸����� ***************************/

	/****************************** �����޸���ʼ ******************************/
    for (i = Line_Count; i >= 21;i-=2)
	{
		Mid_Line[i] = (Right_Add_Line[i] + Left_Add_Line[i]) / 2;	// ���������е�
        }
    Mid_Line_Repair(Line_Count, data);
}
void Right_island_Handle(uint8 (*data)[COL_W])
{
    int16 i;	// ������
    int16 res;	// ���ڽ��״̬�ж�
    Line_Count = 0;	// ����������λ

    Left_Add_Start = 0;		// ��λ������ʼ������
    Right_Add_Start = 0;
    Left_Add_Stop = 0;
    Right_Add_Stop = 0;

    for (i = 59; i >= 19;)
    {
        i -= 2;
        Left_Add_Flag[i] = 1;	// ��ʼ�����߱�־λ
        Right_Add_Flag[i] = 1;
        Right_Line[i] = 93;
        Left_Line[i] = 1;	// �����߽��ʼֵ
    }

    /***************************** ��һ�����⴦�� *****************************/
    res = First_Line_Handle(data);
    if (res == 0)
    {
            Out_Side = 1;	// ����
            return;
    }
    Out_Side = 0;
    Line_Count = 59;
    /*************************** ��һ�����⴦����� ***************************/
    for (i = 59; i >= 19;)	// ������ǰ40��ͼ�񣬸��к������20������
    {
        i -= 2;	// ���д�����С��Ƭ������
        if(data[i][Mid_Line[i+2]] < BlackThres)//ǰ2���е��ڱ���Ϊ�ڵ㣬����������������Ҳ�����ǻ�·
        {
            i += 2;
            break;
        }
        else	// ʹ��ǰ2���е�������ɨ��߽�
        {
                Traversal_Mid_Line(i, data, Mid_Line[i+2], 1,COL_W-1, Left_Line, Right_Line, Left_Add_Line, Right_Add_Line);
        }

        /**************************** ���߼�⿪ʼ ****************************/
        if (Width_Real[i] > Width_Min+1)//&&(Left_Add_Flag[i]||Right_Add_Flag[i]))	// ������ȱ��������ʮ�ֻ�·
        {
            if (Left_Add_Line[i] <=  Left_Add_Line[i+2])
			{
				if (!Left_Add_Flag[i])
				{
					Left_Add_Flag[i] = 1;	// ǿ���϶�Ϊ��Ҫ����
				}
			}
			if (Right_Add_Line[i] >= Right_Add_Line[i+2])
			{
				if (!Right_Add_Flag[i])
				{
					Right_Add_Flag[i] = 1;	// ǿ���϶�Ϊ��Ҫ����
				}
			}

			if (Left_Add_Flag[i] || Right_Add_Flag[i])
			{
				if (Left_Add_Stop || Right_Add_Stop)
				{
                                        if(i < 30)break;
				}
			}
        }
        /*************************** ��һ�ֲ��߿�ʼ ***************************/
		if (Left_Add_Flag[i])	// �����Ҫ����
		{
			if (i >= 55)	// ǰ���в��߲���
			{
				if (!Left_Add_Start)
				{
					Left_Add_Start = i;	// ��¼���߿�ʼ��
					Left_Ka = 0;
					Left_Kb = Left_Add_Line[i];
				}
				Left_Add_Line[i] = Calculate_Add(i, Left_Ka, Left_Kb);	// ʹ��ǰһ֡ͼ����߽�б�ʲ���
			}
			else
			{
				if (!Left_Add_Start)	// ֮ǰû�в���
				{
					Left_Add_Start = i;	// ��¼��ಹ�߿�ʼ��
					Curve_Fitting(&Left_Ka, &Left_Kb, &Left_Add_Start, Left_Add_Line, Left_Add_Flag, 1);	// ʹ�����㷨���ֱ��
				}
				Left_Add_Line[i] = Calculate_Add(i, Left_Ka, Left_Kb);	// �������
			}
		}
		else
		{
			if (Left_Add_Start)	// �Ѿ���ʼ����
			{
				if (!Left_Add_Stop && !Left_Add_Flag[i+2])
				{
					if (Left_Add_Line[i] >= Left_Add_Line[i+2])
					{
						Left_Add_Stop = i;	// ��¼��ಹ�߽�����
					}
				}
			}
		}

		if (Right_Add_Flag[i])	// �Ҳ���Ҫ����
		{
			if (i >= 55)	// ǰ���в��߲���
			{
				if (!Right_Add_Start)
				{
					Right_Add_Start = i;	// ��¼���߿�ʼ��
					Right_Ka = 0;
					Right_Kb = Right_Add_Line[i];
				}
				Right_Add_Line[i] = Calculate_Add(i, Right_Ka, Right_Kb);	// ʹ��ǰһ֡ͼ���ұ߽�б�ʲ���
			}
			else
			{
				if (!Right_Add_Start)	// ֮ǰû�в���
				{
					Right_Add_Start = i;	// ��¼�Ҳಹ�߿�ʼ��
					Curve_Fitting(&Right_Ka, &Right_Kb, &Right_Add_Start, Right_Add_Line, Right_Add_Flag, 2);	// ʹ�����㷨���ֱ��
				}
				Right_Add_Line[i] = Calculate_Add(i, Right_Ka, Right_Kb);	// �������
			}
		}
		else
		{
			if (Right_Add_Start)	// �Ѿ���ʼ����
			{
				if (!Right_Add_Stop && !Right_Add_Flag[i+2])
				{
					if (Right_Line[i] <= Right_Line[i+2])
					{
						Right_Add_Stop = i-2;	// ��¼�Ҳಹ�߽�����
					}
				}
			}
		}
		/*************************** ��һ�ֲ��߽��� ***************************/
                Width_Add[i] = Right_Add_Line[i] - Left_Add_Line[i];	// ���¼����������
		if ((Left_Add_Flag[i] && Right_Add_Flag[i]) || (!Left_Add_Flag[i] && !Right_Add_Flag[i]))
		{
			Width_Add[i] = Right_Add_Line[i] - Left_Add_Line[i];	// ���¼����������
			Mid_Line[i] = (Right_Add_Line[i] + Left_Add_Line[i]) / 2;	// ��������
		}
		else
		{
			Width_Add[i] = Right_Add_Line[i] - Left_Add_Line[i];	// ���¼����������
			Mid_Line[i] = Mid_Line[i+2];
		}
		if (Width_Add[i] < Width_Min)
		{
			Width_Min = Width_Add[i];	// ������С�������
		}

		Line_Count = i;	// ��¼�ɹ�ʶ�𵽵���������

                if(Left_Add_Stop&&Right_Add_Stop)    //���߽�������Ҫ�����ⲻ��
                {
                    if(Left_Add_Flag[i]||Right_Add_Flag[i])
                    {
                        if(i < 30)break;
                    }
                }
    }
    if(island_flag == 2)
    {
            uint8 image_tiaobian_flag = 0;
            uint8 find_point_1y = 0;
            for(i=59;i>=21;i-=2)
            {
                if(image_tiaobian_flag == 2)
                    break;
                image_tiaobian_flag = 0;
                for(int j = 78;j >= Left_Line[i];j--)
                {
                    if(!image_tiaobian_flag)
                    {
                        if(image[i][j] - image[i][j-1] > 20
                           && image[i][j] > (BlackThres)) //�ҽ��������е�
                        {
                            image_tiaobian_flag = 1;
                            find_point_1y = j;     //��¼����
                        }
                    }
                    else  //
                    {
                        if(image[i][j-1] - image[i][j] > 20
                           && image[i][j-1] > (BlackThres)
                             && image[i-2][j] < (BlackThres+10)) //�ҽ��������Ϸ��յ�
                        {
                            if(find_point_1y - j < 15)
                            {
                                if(i%2==0)i=i-1;
                                find_right_point_x = i;  //��¼�Ϸ��յ����
                                find_right_point_y = j;  //��¼�Ϸ��յ����
                                image_tiaobian_flag = 2;
                                break;
                            }
                            else
                                break;
                        }
                    }
                }
            }
            Line_Count = find_right_point_x-2;   //ʶ���������С���������
            Left_Line[find_right_point_x] = find_right_point_y;  //���������Ϸ��յ���߽�ǿ��ƫ��
            Curve2_Fitting(&Left_Ka, &Left_Kb, 59,find_right_point_x, Left_Line, 1,30); //�������������ȱ�ڴ�����߽�б��
                                                                                        //Ϊ���油����׼��
            if(Left_Ka <= 0)
            {
                Last_Left_Ka = Left_Ka;
                Last_Left_Kb = Left_Kb;
            }
            else
            {
                Left_Ka = Last_Left_Ka;
                Left_Kb = Last_Left_Kb+10;
            }
            for(int j=59;j>=21;j-=2)      //��ʹ���ҵ��Ĺյ�������
            {
                if(Line_Count >= 27)
                    Left_Add_Line[j] = Calculate_Add(j, Left_Ka, Left_Kb) + (Line_Count-27);
                else
                    Left_Add_Line[j] = Calculate_Add(j, Left_Ka, Left_Kb);
                if(Left_Add_Line[j] > Right_Add_Line[j])
                {
                    Right_Add_Line[j] = 93;
                }
            }

            for(int j=find_right_point_x;j>=31;j-=2)
            {

                if((Left_Line[j] > find_right_point_y)
                   &&(Left_Line[j-2] > Left_Line[j])
                     &&(Left_Line[j-4] > Left_Line[j-2])
                       &&(Left_Line[j-6] > Left_Line[j-4]))
                {
                    is_diu_flag = 3;
                    zhaoguidian_diu_flag_num = j;
                    break;
                }
            }

        if(is_diu_flag == 3)
        {
            for(int j=59;j>=Line_Count-2;j-=2)
            {
                if((Left_Line[j] > 13)&&image[j][Left_Line[j]-7]>(BlackThres-5)&&image[j][Left_Line[j]-8] > (BlackThres-5))
                {
                    zhaoguidian_diu_flag_num = j;//�ҵ��Ѿ����뻷�������ߵ�
                    break;
                }
            }
            for(int j=zhaoguidian_diu_flag_num;j>=21;j-=2)
            {
                if(Left_Line[j] < Left_Line[zhaoguidian_diu_flag_num])
                {
                    Line_Count = j+2;
                    break;
                }
            }
            Curve2_Fitting(&Left_Ka, &Left_Kb, 59,zhaoguidian_diu_flag_num, Left_Line, 1,27);
            if(Left_Ka <= 0)
            {
                Last_Left_Ka = Left_Ka;
                Last_Left_Kb = Left_Kb;
            }
            else
            {
                Left_Ka = Last_Left_Ka;
                Left_Kb = Last_Left_Kb+10;
            }
            for(i = 59; i >= zhaoguidian_diu_flag_num; i -= 2)
                Left_Add_Line[i] = Calculate_Add(i, Left_Ka, Left_Kb);//���Ѿ����뻷���ĵ���в���
            for(i = Line_Count; i >= 21; i -= 2)//�������ߣ������´����ߣ�����������
                Left_Add_Line[i] = Left_Line[zhaoguidian_diu_flag_num];
            /***************************************************************************************************/
            float g_fSlope2 = 0;
            float g_fSlope3 = 0;
            float g_fSlope4 = 0;

            g_fSlope2 = 1.0*(Left_Line[51] - Left_Line[55])/4;
            g_fSlope3 = 1.0*(Left_Line[49] - Left_Line[53])/4;
            g_fSlope4 = 1.0*(Left_Line[47] - Left_Line[51])/4;

            if((g_fSlope2 <= 2)
               && (g_fSlope3 <= 2)
                 && (g_fSlope4 <= 2)
                   && (g_fSlope2 >= 0)
                     && (g_fSlope3 >= 0)
                       && (g_fSlope4 >= 0))//�Ѿ����뻷�� ���ҵ����־������ ��һ��׼���ҳ������յ�
            {
                is_diu_flag = 4;
                tuqi_point_hang = 0;
            }

        }
        if(is_diu_flag == 4)//��ʼ�ҳ������յ�
        {
            island_flag = 0;    //���������������־λֹͣ���ߡ�
            is_diu_flag = 5;     //�������߱�־λ
        }
    }
    for (i = Line_Count; i >= 21;i-=2)
    {
		Mid_Line[i] = (Right_Add_Line[i] + Left_Add_Line[i]) / 2;	// ���������е�
    }

    Mid_Line_Repair(Line_Count, data);
}
void image_processing(uint8 (*data)[COL_W])
{
    switch(island_flag)
    {
      case 0:   Image_Handle(data);               break;
      case 1:   Left_island_Handle(data);         break;
      case 2:   Right_island_Handle(data);        break;
    }
}
//void go_island_set_value()   //�뻷��
//{
//    Set = 0;    //����ר��PID
//    CS_P_Right = 0.5;//����ϵ��
//    CS_P_Left = 0.5;
//}
//void out_island_set_value()  //������
//{
//    Set = 0;    //����ר��PID
//    CS_P_Right = 0.5;//����ϵ��
//    CS_P_Left = 0.5;
//}
/**************************** �µ���⿪ʼ ****************************/
void Ramp_Check(void)
{
    uint8 i,j,Ramp_Count,line_Count;
    uint16 Ramp_Error_Sum=0;
    //Ramp_Flag = 0;
    Ramp_Count = 0;
    line_Count = 0;
    i = 0;

    for(j=51;j>=31;)
    {
      j -= 2;
      if(!Left_Add_Flag[j]&&!Right_Add_Flag[j])
      {
        if (Left_Line[j] >= Left_Line[j+2]&&Right_Line[j] <= Right_Line[j+2]&&Left_Line[j]>Left_Line[59]&&Right_Line[j]<Right_Line[59])	//��ǰһ�еı߽�ʵ�߱Ƚ�
        {
            Ramp_Error[i]=Width_Real[j]-Width_Max[j];
            if(Ramp_Error[i]>=20)
              Ramp_Error[i]=0;
            else if(Ramp_Error[i]<=5)
              Ramp_Error[i]=0;
            if(Width_Real[j]>Width_Max[j])
              Ramp_Count++;
        }
      }
      Ramp_Error_Sum+=Ramp_Error[i];
      i++;
    }
    //Mid_Line_K = 1.0*(Mid_Line[59]-Mid_Line[21])/38;
    if(Ramp_Count>=9&&Ramp_Error_Sum>=103&&Ramp_Error[8]>=(Ramp_Error[2]+3))// 8 98  11 220  9     you  zuida  18  11dian  he 111
    {
        Ramp_Flag = 1;
        Ramp_Time_Count = 140;
    }
}
/**************************** �µ������� ****************************/


/**************************** �����߼�⿪ʼ ****************************/
uint8 Starting_Check(uint8 i, uint8 (*data)[COL_W], uint8 Left_Min, uint8 Right_Max)
{
    uint8 j;
    int White_Flag = 0;
    uint8 Jump_Count = 0;	// ��������

    Right_Max = range_protect(Right_Max, 1, 93);	// �����Ҳಿ�����򣬷�ֹ���

    for (j = Left_Min; j < Right_Max; j++)	// �߽����� 1��94
    {
          if (!White_Flag)	// �Ȳ�����߽�
          {
                  if(data[i][j] > BlackThres)	// ��⵽�׵�
                  {
                          White_Flag = 1;		// ��ʼ�Һڵ�

                          continue;	// ��������ѭ���������´�ѭ��
                  }
          }
          else
          {
                  if(data[i][j] < BlackThres)	//���ڵ�
                  {
                          Jump_Count ++;        // ��Ϊһ������

                          if((data[i][j+1]< BlackThres) && j+1 <= Right_Max)	// ���������ڵ�
                          {
                                  if ((data[i][j+2]< BlackThres) && j+2 <= Right_Max)	// ���������ڵ�
                                  {
                                          if ((data[i][j+3]< BlackThres) && j+3 <= Right_Max)	// �����ĸ��ڵ�
                                          {
                                                  if ((data[i][j+4]< BlackThres) && j+4 <= Right_Max)	// ��������ڵ�
                                                  {
                                                          if ((data[i][j+5]< BlackThres) && j+5 <= Right_Max)	// ���������ڵ�
                                                          {
                                                                  if ((data[i][j+6]< BlackThres) && j+6 <= Right_Max)	// �����߸��ڵ�
                                                                  {
                                                                          if ((data[i][j+7]< BlackThres) && j+7 <= Right_Max)	// �����˸��ڵ�
                                                                          {
                                                                                  if ((data[i][j+8]< BlackThres) && j+8 <= Right_Max)	// �����Ÿ��ڵ�
                                                                                  {
                                                                                          if ((data[i][j+9]< BlackThres) && j+9 <= Right_Max)	// ����ʮ���ڵ�
                                                                                          {
                                                                                                  if ((data[i][j+10]< BlackThres) && j+10 <= Right_Max)	// ����11���ڵ�
                                                                                                  {
                                                                                                          White_Flag = 0;	// ��Ϊ���Ǹ��ţ������κδ����´������׵�
                                                                                                          j += 10;
                                                                                                  }
                                                                                                  else if (j+10 <= Right_Max)
                                                                                                  {
                                                                                                          data[i][j] < BlackThres;	// ��������10���ڵ㣬�˳���
                                                                                                          data[i][j+1] < BlackThres;	// ��������10���ڵ㣬�˳���
                                                                                                          data[i][j+2] < BlackThres;	// ��������10���ڵ㣬�˳���
                                                                                                          data[i][j+3] < BlackThres;	// ��������10���ڵ㣬�˳���
                                                                                                          data[i][j+4] < BlackThres;	// ��������10���ڵ㣬�˳���
                                                                                                          data[i][j+5] < BlackThres;	// ��������10���ڵ㣬�˳���
                                                                                                          data[i][j+6] < BlackThres;	// ��������10���ڵ㣬�˳���
                                                                                                          data[i][j+7] < BlackThres;	// ��������10���ڵ㣬�˳���
                                                                                                          data[i][j+8] < BlackThres;	// ��������10���ڵ㣬�˳���
                                                                                                          data[i][j+9] < BlackThres;	// ��������10���ڵ㣬�˳���

                                                                                                          j += 10;
                                                                                                  }
                                                                                                  else
                                                                                                  {
                                                                                                          j += 10;
                                                                                                  }
                                                                                          }
                                                                                          else if (j+9 <= Right_Max)
                                                                                          {
                                                                                                  data[i][j] < BlackThres;	// ���������Ÿ��ڵ㣬�˳���
                                                                                                  data[i][j+1] < BlackThres;	// ���������Ÿ��ڵ㣬�˳���
                                                                                                  data[i][j+2] < BlackThres;	// ���������Ÿ��ڵ㣬�˳���
                                                                                                  data[i][j+3] < BlackThres;	// ���������Ÿ��ڵ㣬�˳���
                                                                                                  data[i][j+4] < BlackThres;	// ���������Ÿ��ڵ㣬�˳���
                                                                                                  data[i][j+5] < BlackThres;	// ���������Ÿ��ڵ㣬�˳���
                                                                                                  data[i][j+6] < BlackThres;	// ���������Ÿ��ڵ㣬�˳���
                                                                                                  data[i][j+7] < BlackThres;	// ���������Ÿ��ڵ㣬�˳���
                                                                                                  data[i][j+8] < BlackThres;	// ���������Ÿ��ڵ㣬�˳���

                                                                                                  j += 9;
                                                                                          }
                                                                                          else
                                                                                          {
                                                                                                  j += 9;
                                                                                          }
                                                                                  }
                                                                                  else if (j+8 <= Right_Max)
                                                                                  {
                                                                                          data[i][j] < BlackThres;	// ���������˸��ڵ㣬�˳���
                                                                                          data[i][j+1] < BlackThres;	// ���������˸��ڵ㣬�˳���
                                                                                          data[i][j+2] < BlackThres;	// ���������˸��ڵ㣬�˳���
                                                                                          data[i][j+3] < BlackThres;	// ���������˸��ڵ㣬�˳���
                                                                                          data[i][j+4] < BlackThres;	// ���������˸��ڵ㣬�˳���
                                                                                          data[i][j+5] < BlackThres;	// ���������˸��ڵ㣬�˳���
                                                                                          data[i][j+6] < BlackThres;	// ���������˸��ڵ㣬�˳���
                                                                                          data[i][j+7] < BlackThres;	// ���������˸��ڵ㣬�˳���

                                                                                          j += 8;
                                                                                  }
                                                                                  else
                                                                                  {
                                                                                          j += 8;
                                                                                  }
                                                                          }
                                                                          else if (j+7 <= Right_Max)
                                                                          {
                                                                                  data[i][j] < BlackThres;	// ���������߸��ڵ㣬�˳���
                                                                                  data[i][j+1] < BlackThres;	// ���������߸��ڵ㣬�˳���
                                                                                  data[i][j+2] < BlackThres;	// ���������߸��ڵ㣬�˳���
                                                                                  data[i][j+3] < BlackThres;	// ���������߸��ڵ㣬�˳���
                                                                                  data[i][j+4] < BlackThres;	// ���������߸��ڵ㣬�˳���
                                                                                  data[i][j+5] < BlackThres;	// ���������߸��ڵ㣬�˳���
                                                                                  data[i][j+6] < BlackThres;	// ���������߸��ڵ㣬�˳���

                                                                                  j += 7;
                                                                          }
                                                                          else
                                                                          {
                                                                                  j += 7;
                                                                          }
                                                                  }
                                                                  else if (j+6 <= Right_Max)
                                                                  {
                                                                          data[i][j] < BlackThres;	// �������������ڵ㣬�˳���
                                                                          data[i][j+1] < BlackThres;	// �������������ڵ㣬�˳���
                                                                          data[i][j+2] < BlackThres;	// �������������ڵ㣬�˳���
                                                                          data[i][j+3] < BlackThres;	// �������������ڵ㣬�˳���
                                                                          data[i][j+4] < BlackThres;	// �������������ڵ㣬�˳���
                                                                          data[i][j+5] < BlackThres;	// �������������ڵ㣬�˳���

                                                                          j += 6;
                                                                  }
                                                                  else
                                                                  {
                                                                          j += 6;
                                                                  }
                                                          }
                                                          else if (j+5 <= Right_Max)
                                                          {
                                                                  data[i][j] < BlackThres;	// ������������ڵ㣬�˳���
                                                                  data[i][j+1] < BlackThres;	// ������������ڵ㣬�˳���
                                                                  data[i][j+2] < BlackThres;	// ������������ڵ㣬�˳���
                                                                  data[i][j+3] < BlackThres;	// ������������ڵ㣬�˳���
                                                                  data[i][j+4] < BlackThres;	// ������������ڵ㣬�˳���

                                                                  j += 5;
                                                          }
                                                          else
                                                          {
                                                                  j += 5;
                                                          }
                                                  }
                                                  else if (j+4 <= Right_Max)
                                                  {
                                                          data[i][j] < BlackThres;	// ���������ĸ��ڵ㣬�˳���
                                                          data[i][j+1] < BlackThres;	// ���������ĸ��ڵ㣬�˳���
                                                          data[i][j+2] < BlackThres;	// ���������ĸ��ڵ㣬�˳���
                                                          data[i][j+3] < BlackThres;	// ���������ĸ��ڵ㣬�˳���

                                                          j += 4;
                                                  }
                                                  else
                                                  {
                                                          j += 4;
                                                  }
                                          }
                                          else if (j+3 <= Right_Max)
                                          {
                                                  data[i][j] < BlackThres;	// �������������ڵ㣬�˳���
                                                  data[i][j+1] < BlackThres;	// �������������ڵ㣬�˳���
                                                  data[i][j+2] < BlackThres;	// �������������ڵ㣬�˳���

                                                  j += 3;
                                          }
                                          else
                                          {
                                                  j += 3;
                                          }
                                  }
                                  else if (j+2 <= Right_Max)
                                  {
                                          data[i][j] < BlackThres;	// �������������ڵ㣬�˳���
                                          data[i][j+1] < BlackThres;	// �������������ڵ㣬�˳���

                                          j += 2;
                                  }
                                  else
                                  {
                                          j += 2;
                                  }
                          }
                          else if (j+1 <= Right_Max)
                          {
                                  data[i][j] < BlackThres;	// ��һ���ڵ㣬�˳���

                                  j += 1;
                          }
                          else
                          {
                                  j += 1;
                          }
                  }
          }
  }

   return Jump_Count;	// ������������
}
/*�������Ժ���������յ㲢���߳�ȥ*/
void out_ring_check(int a)
{
 uint8 count_flag = 0;
 uint8 image_tiaobian_flag = 0;
 uint8 out_find_point_1x = 0;
 out_ring_num = 0;
 int16 i = 55;
 if(a == 1)   //�һ���
 {
    for(int j=60;j>=Left_Line[i];j-=1)
    {
        i-=2;
        if(i<=37)i=37;
        if(image_tiaobian_flag == 2)
            break;
        image_tiaobian_flag = 0;
        for(int k = 55;k >= i;k-=2)
        {
            if(!image_tiaobian_flag)
            {
                if(image[k][j] - image[k-1][j] > 18
                   && image[k][j] > (BlackThres-5)
                     && image[k-1][j] < BlackThres+5) //�ҹյ�
                {
                    image_tiaobian_flag = 1;
                    out_find_point_1x = k;
                }
            }
            else
            {
                if(image[k-1][j] - image[k][j] > 18
                   && image[k-1][j-2] > (BlackThres-5)
                     && image[k][j] < BlackThres+5)
                {
                    if(out_find_point_1x - k <= 15 && out_find_point_1x - k >= 3)
                    {
                        out_ring_num = k;
                        out_ring_flag = 1;

                        if(out_ring_num >=53)
                        {
                            out_ring_num = 0;
                        }
                        image_tiaobian_flag = 2;
                        break;
                    }
                    else
                        break;
                }
            }
        }
    }
   if(out_ring_flag)//�ҵ��������յ� ��ʼ����
   {
//        led(LED1,LED_ON);   //������ָʾ�ƿ���
//        led(LED2,LED_OFF);  //������ָʾ�ƹر�
        if( out_ring_num != 0)
        {
                Curve2_Fitting(&Left_Ka, &Left_Kb, out_ring_num+6,out_ring_num+4, Left_Line, 1,-3);
//                if(Left_Ka >=-3)
//                {
//                    ring_out_left_ka = Left_Ka;
//                    ring_out_left_kb = Left_Kb;
//                }
//                else
//                {
//                    Left_Ka = -2.5;
//                    Left_Kb = ring_out_left_kb+10;
//                }
                ring_out_left_ka = Left_Ka;
                ring_out_left_kb = Left_Kb;
                for(i = out_ring_num; i >= 21; i -= 2)
                    Left_Add_Line[i] = Calculate_Add(i, Left_Ka, Left_Kb);
        }
        else
        {
                for(i = 59; i >= 21; i -= 2)
                {
                    Left_Add_Line[i] = Calculate_Add(i, ring_out_left_ka, ring_out_left_kb);
                }

        }
        if(Left_Line[55] != 1 && Left_Line[29] != 1)
        {       Curve3_Fitting(&Left_Ka, &Left_Kb, 55,29, Left_Line, 1);
                for(int aaaaa = 29; aaaaa <= 57; aaaaa += 2)//�� �������ı�־
                {
                    Outring_Left_Line[aaaaa] = Calculate_Add(aaaaa, Left_Ka, Left_Kb);
                    if(Left_Line[aaaaa] <= (Outring_Left_Line[aaaaa]+2) && Left_Line[aaaaa] >= (Outring_Left_Line[aaaaa]-2))
                    {
                       if(++count_flag >= 13)  ////������
                       {
                          island_right_mid_add_flag = 1;
                          //is_diu_flag = 6;
                          count_flag = 0;
                          is_diu_flag = 0;
                          flag1 = 0;
                          flag2 = 0;
                          out_ring_flag = 0;
                          right_ring_flag = 0;
                          left_ring_flag = 0;
                          //led(LED1,LED_OFF);
                          fand_ring_time = 50;
                          out_island_set_value();
                       }
                    }
                }
        }
     }
 }
 else  //�󻷵�
 {
        for(int j=33;j<=Right_Line[i];j+=1)
        {
            i-=2;
            if(i<=37)i=37;
            if(image_tiaobian_flag == 2)
                break;
            image_tiaobian_flag = 0;
            for(int k = 55;k >= i;k-=2)
            {
                if(!image_tiaobian_flag)
                {
                    if(image[k][j] - image[k-1][j] > 18 && image[k][j] > (BlackThres-5) && image[k-1][j] < (BlackThres+5))
                    {
                        image_tiaobian_flag = 1;
                        out_find_point_1x = k;
                    }
                }
                else
                {
                    if(image[k-1][j] - image[k][j] > 18 && image[k-1][j-2] > (BlackThres-5) && image[k-1][j] < (BlackThres+5))
                    {
                        if(out_find_point_1x - k <= 15 && out_find_point_1x - k >= 3)
                        {
                            out_ring_num = k;
                            out_ring_flag = 1;
                            if(out_ring_num >=53)
                            {
                                out_ring_num = 0;
                            }
                            image_tiaobian_flag = 2;
                            break;
                        }
                        else
                            break;
                    }
                }
            }
        }
        if(out_ring_flag)
        {
//            led(LED1,LED_ON);   //������ָʾ�ƿ���
//            led(LED2,LED_OFF);  //������ָʾ�ƹر�
            if(out_ring_num != 0)
            {
                Curve2_Fitting(&Right_Ka, &Right_Kb, out_ring_num+6,out_ring_num+4, Right_Line, 2,-3);
                ring_out_Right_ka = Right_Ka;
                ring_out_Right_kb = Right_Kb;
                for(i = out_ring_num; i >= 21; i -= 2)
                    Right_Add_Line[i] = Calculate_Add(i, Right_Ka, Right_Kb);
            }
            else
            {
                for(i = 59; i >= 21; i -= 2)
                {
                    Right_Add_Line[i] = Calculate_Add(i, ring_out_Right_ka, ring_out_Right_kb);
                }
            }
            if(Right_Line[55] != 93 && Right_Line[29] != 93)
            {
                Curve3_Fitting(&Right_Ka, &Right_Kb, 55,29, Right_Line, 2);
                for(int aaaaa = 29; aaaaa <= 57; aaaaa += 2)
                {
                    Outring_Right_Line[aaaaa] = Calculate_Add(aaaaa, Right_Ka, Right_Kb);
                    if(Right_Line[aaaaa] <= (Outring_Right_Line[aaaaa]+2) && Right_Line[aaaaa] >= (Outring_Right_Line[aaaaa]-2))
                    {

                        if(++count_flag >= 13)  //������
                        {
                           island_left_mid_add_flag = 1;
                          // is_diu_flag = 6;
                           count_flag = 0;
                           is_diu_flag = 0;
                           flag1 = 0;
                           flag2 = 0;
                           out_ring_flag = 0;
                           right_ring_flag = 0;
                           left_ring_flag = 0;
                           //led(LED1,LED_OFF);
                           fand_ring_time = 50;
                           out_island_set_value();
                           break;
                        }

                    }
                }
            }
        }
    }
}