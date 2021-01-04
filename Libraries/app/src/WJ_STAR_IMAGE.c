#include "WJ_STAR_IMAGE.h"

#define SSSSSSS_OFF_ON 1    //小S检测开关，1为开 0为关

int16  EdgeThres = 17;  //跳变沿阈值

float  BlackThres = 60;   //黑白阈值

uint8 image_h[ROW_H][COL_W];

uint8 Jump[62]; //起跑线
int16 Is_Right_Line[62] = {0},Is_Left_Line[62] = {0};
int16 Outring_Right_Line[62] = {0},Outring_Left_Line[62] = {0};
int16 Left_Line[62] = {0}, Right_Line[62] = {0}, Mid_Line[62] = {0};	// 原始左右边界数据
int16 Left_Add_Line[62], Right_Add_Line[62];		// 左右边界补线数据
int16 Left_Add_Flag[62], Right_Add_Flag[62];		// 左右边界补线标志位
int16 Width_Real[62];	// 实际赛道宽度
int16 Width_Add[62];	// 补线赛道宽度
int16 Width_Min;		// 最小赛道宽度

int16 S_temp;
int16 Foresight,Last_Foresight,Ramp_Flag_Last=0;	// 前瞻，用于速度控制

uint8 Weight[60] =                      //加权平均参数
{
    4,  4,  4,  4,  5,  5,  5,  5,  5,  6,
    6,  6,  6,  6,  6,  7,  8,  9,  10, 11,
    12, 13, 14, 15, 14, 13, 12, 11, 10, 9,
    8,  7,  6,  5,  4,  2,  2,  2,  2,  2,
    2,  2,  2,  2,  2,  2,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  1,};
uint8 Island_Width_Add[60] =            //环岛布线参数
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

//定义文件变量
uint8 Left_Max, Right_Min;
char Last_Point = MIDVALUE;

int16 Left_Hazard_Flag, Right_Hazard_Flag, Hazard_Flag;	// 左右障碍物标志

int16 Left_Add_Start, Right_Add_Start;	// 左右补线起始行坐标
int16 Left_Add_Stop, Right_Add_Stop;	// 左右补线结束行坐标
float Left_Ka = 0, Right_Ka = 0,ring_out_left_ka = 0,ring_out_left_kb = 0,ring_out_Right_kb = 0,ring_out_Right_ka = 0;
float Last_Right_Ka = 1,Last_Right_Kb = 70;
float Last_Left_Ka = 1,Last_Left_Kb = 35;
float Left_Kb = 1, Right_Kb = COL_W-1;	// 最小二乘法参数

int16 Out_Side = 0;	// 丢线控制
int16 Line_Count;	// 记录成功识别到的赛道行数

int8 ADD_line_flag = 0;//十字补线开始标志

uint8 left_right_guai_flag = 0;//判断是大左转弯还是打右转弯

/****************************    起跑识别变量    ****************************/
uint8 Stop_Flag = 0,count_num = 0;	// 起跑线标志位
/****************************        结束        ****************************/
int island_left_mid_add_flag = 0,island_right_mid_add_flag = 0;
int fand_ring_time = 0;  //出圆环之后，1s后在检测圆环
int right_ring_flag = 0,left_ring_flag = 0;
int tuqi_point_hang=0,tuqi_point_flag = 0;
int out_ring_point_hang = 0;
int out_ring_num = 0;
int go_ring_flag = 0,out_ring_flag = 0;
int island_flag = 0;  //标志为0不是环岛   标志为1是左环岛   标志为2是右环岛
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

#define ERROR      2      //误差
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
                            N0++;       //像素点数
			 }
			else
                         {
                            S1 += image[i][j];
                            N1++;       //像素点数
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
char Error_Transform(uint8 Data, uint8 Set_num)   //求绝对值
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
*	计算补线坐标
*
*	说明：使用两点法计算拟合出的补线坐标
*/
int16 Calculate_Add(uint8 i, float Ka, float Kb)	// 计算补线坐标
{
	float res;
	int16 Result;

	res = i * Ka + Kb;
	Result = range_protect((int32)res, 1, COL_W-1);

	return Result;
}
/*
*	两点法求直线
*
*	说明：拟合直线 y = Ka * x + Kb   Mode == 1代表左边界，Mode == 2代表右边界
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
*	两点法求直线
*
*	说明：拟合直线 y = Ka * x + Kb   Mode == 1代表左边界，Mode == 2代表右边界
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
*	两点法求直线
*
*	说明：拟合直线 y = Ka * x + Kb   Mode == 1代表左边界，Mode == 2代表右边界
*/
void Curve3_Fitting(float *Ka, float *Kb, uint8 Start,uint8 End, int16 *Line, int16 Mode)
//环岛检测专用，最正规的求线
{
    if (Mode==1)
    {
        *Ka = 1.0*((Line[Start]) - Line[End]) / (Start-End); //斜率
        *Kb = 1.0*Line[End] - (*Ka * End);   //截距
    }
    else
    {
        *Ka = 1.0*((Line[Start]) - Line[End]) / (Start-End);
        *Kb = 1.0*Line[End] - (*Ka * End);
    }

}
void Curve2_Fitting_Island(float *Ka, float *Kb, uint8 Start,uint8 End, int16 *Left,int16 *Right, int16 Mode,uint8 power)//环岛补线使用
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
****函数名：从中间向两边搜索边 ******************************************
****说明：本函数使用后将保存边界数据
*         Mid上一行中线的值        Left_Min左边最小值  Right_Max右边最大值
*         Left_Line实际左边界      Right_Line实际右边界
*         Left_Add_Line补线左边界  Right_Add_Line补线右边界
************************************************************************/
void Traversal_Mid_Line(int16 i,uint8 (*data)[COL_W],
                        int16 Mid, int16 Left_Min, int16 Right_Max,
                        int16 *Left_Line, int16 *Right_Line,
                        int16 *Left_Add_Line, int16 *Right_Add_Line)
{
    int16 j;
    Left_Add_Flag[i] = 1;	// 初始化补线标志位
    Right_Add_Flag[i] = 1;

    Right_Line[i] = Right_Max;
    Left_Line[i] = Left_Min;	// 给定边界初始值

    for (j = Mid; j >= Left_Min+1; j--)	// 以前一行中点为起点向左查找边界
    {
        if(((data[i][j+1] - data[i][j-1]) > EdgeThres)
           && ((data[i][j] - data[i][j-2]) > EdgeThres)
             && (data[i][j-1] < BlackThres+20))
            {
                Left_Add_Flag[i] = 0;	//左边界不需要补线，清除标志位
                Left_Line[i] = j;       //记录当前j值为本行实际左边界
                Left_Add_Line[i] = j;	//记录实际左边界为补线左边界
                break;
            }
    }
    for (j = Mid; j <= Right_Max-1; j++)	// 以前一行中点为起点向右查找右边界
    {
	if(((data[i][j-1] - data[i][j+1]) > EdgeThres)
           && ((data[i][j] - data[i][j+2]) > EdgeThres)
             && (data[i][j+1] < BlackThres+20))	//检测到黑点
            {
                Right_Add_Flag[i] = 0;		//右边界不需要补线，清除标志位
                Right_Line[i] = j;	//记录当前j值为本行右边界
                Right_Add_Line[i] = j;	// 记录实际右边界为补线左边界
                break;
            }
    }

    /************************上面两轮循环后 检测是否需要补线**************************/

    if(Left_Add_Flag[i])	// 左边界需要补线
	{
		if (i >= 55)	// 前6行
		{
			Left_Add_Line[i] = Left_Line[59];	// 使用底行数据 底行特别容易丢线
		}
		else                     ///
		{
			Left_Add_Line[i] = Left_Add_Line[i+2];	// 使用前2行左边界作为本行左边界
		}
	}
    if(Right_Add_Flag[i])	// 右边界需要补线
	{
		if (i >= 55)	// 前6行
		{
			Right_Add_Line[i] = Right_Line[59];	// 使用底行数据   底行特别容易丢线
		}
		else         //////
		{
			Right_Add_Line[i] = Right_Add_Line[i+2];	// 使用前2行右边界作为本行右边界
		}
	}
	Width_Real[i] = Right_Line[i] - Left_Line[i];		// 计算实际赛道宽度
	Width_Add[i] = Right_Add_Line[i] - Left_Add_Line[i];	// 计算补线赛道宽度
}
/*
*	从左侧开始搜索边界，返回赛道宽度
*
*	说明：本函数仅仅作为探测赛道使用，仅返回赛道宽度，不保存边界数据
*/
int16 Traversal_Left(int16 i,uint8 (*data)[COL_W], int16 *Mid, int16 Left_Min, int16 Right_Max)
{
	int16 j, White_Flag = 0;
	int16 Left_Line = Left_Min, Right_Line = Right_Max;

	for (j = Left_Min; j <= Right_Max; j++)	// 边界 1到93
	{
		if (!White_Flag)   // 先查找左边界 !White_Flag = 1
		{
                      if(data[i][j] > BlackThres)   //检测到白点
			{
				Left_Line = j;	// 记录当前j值为本行左边界
				White_Flag = 1;	// 左边界已找到，必有右边界

				continue;	// 结束本次循环，进入下次循环
			}
		}
		else    // White_Flag = 1 左边界已找到，开始寻找右边界
		{
                      if(data[i][j] < BlackThres)   //检测到黑点
			{
				Right_Line = j-1;//记录当前j值为本行右边界
				break;	// 左右边界都找到，结束循环
			}
		}
	}

	if (!White_Flag)	// White_Flag = 0 即没有找到左边界
	{                       //右边界自然没有 返回0
		return 0;
	}
	else                    // 找到左右边界 计算中线和宽度
	{
		*Mid = (Right_Line + Left_Line) / 2;
		return (Right_Line - Left_Line);
	}
}
/*
*	从右侧开始搜索边界，返回赛道宽度
*
*	说明：本函数仅仅作为探测赛道使用，仅返回赛道宽度，不保存边界数据
*/
int16 Traversal_Right(int16 i,uint8 (*data)[COL_W], int16 *Mid, int16 Left_Min, int16 Right_Max)
{
	int16 j, White_Flag = 0;
	int16 Left_Line = Left_Min, Right_Line = Right_Max;

	for (j = Right_Max; j >= Left_Min; j--)	// 边界 93到1
	{
		if (!White_Flag)	// 先查找右边界
		{
			if(data[i][j] > BlackThres)	// 检测到白点
			{
				Right_Line = j;	// 记录当前j值为本行右边界
				White_Flag = 1;	// 右边界已找到，必有左边界

				continue;	// 结束本次循环，进入下次循环
			}
		}
		else
		{
			if(data[i][j] < BlackThres)	//检测黑点
			{
				Left_Line = j+1;	//记录当前j值为本行左边界

				break;	// 左右边界都找到，结束循环
			}
		}
	}
	if (!White_Flag)	// 未找到左右边界
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
*	从左侧开始搜索边界，保存赛道边界，返回1成功 0失败
*
*	说明：本函数使用后将保存边界数据
*/
int16 Traversal_Left_Line(int16 i,uint8 (*data)[COL_W], int16 *Left_Line, int16 *Right_Line)
{
	int16 j, White_Flag = 0;

	Left_Line[i] = 1;
	Right_Line[i] = 93;

	for (j = 1; j < 94; j++)	// 边界坐标 1到79
	{
		if (!White_Flag)	// 先查找左边界
		{
			if(data[i][j] > BlackThres)	// 检测到白点
			{
				Left_Line[i] = j;	// 记录当前j值为本行左边界
				White_Flag = 1;		// 左边界已找到，必有右边界

				continue;	// 结束本次循环，进入下次循环
			}
		}
		else
		{
			if(data[i][j] < BlackThres)	//检测黑点
			{
				Right_Line[i] = j-1;//记录当前j值为本行右边界

				break;	// 左右边界都找到，结束循环
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

	return White_Flag;	// 返回搜索结果
}

/*
*	从右侧开始搜索边界，保存赛道边界，返回1成功 0失败
*
*	说明：本函数使用后将保存边界数据
*/
int16 Traversal_Right_Line(int16 i,uint8 (*data)[COL_W], int16 *Left_Line, int16 *Right_Line)
{
	int16 j, White_Flag = 0;

	Left_Line[i] = 1;
	Right_Line[i] = COL_W-1;

	for (j = COL_W-1; j > 0; j--)	// 边界坐标 1到79
	{
		if (!White_Flag)	// 先查找右边界
		{
			if(data[i][j] > BlackThres)	// 检测到白点
			{
				Right_Line[i] = j;	// 记录当前j值为本行右边界
				White_Flag = 1;		// 右边界已找到，必有左边界

				continue;	// 结束本次循环，进入下次循环
			}
		}
		else
		{
			if(data[i][j] < BlackThres)	//检测黑点
			{
				Left_Line[i] = j+1;//记录当前j值为本行左边界

				break;	// 左右边界都找到，结束循环
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
	return White_Flag;	// 返回搜索结果
}
/*
*	图像算法参数初始化
*
*	说明：仅影响第一行特殊处理     94列
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
*函数功能：第一行特殊处理
*函数说明：先使用第60行中点作为第59行(第一行)搜线起始位置，成功搜索到左右边界后
           将第59行中点赋值给第60行便于下一帧图像使用。如果第60行中点在本行为黑点，再
           分别使用左遍历和右遍历的方法搜索边界，以赛道宽度较大的结果作为第59行边界，
           若仍然搜索不到边界或数据异常认为出界，出界返回0
*******************************************************************************/
int16 First_Line_Handle(uint8 (*data)[COL_W])
{
	int16 i,res;	// 控制行
	int16 Weight_Left, Weight_Right;	// 左右赛道宽度
	int16 Mid_Left, Mid_Right;
	Image_Para_Init();           //图像算法参数初始化
	i = 59;  //第一行

        res = Starting_Check(i, data, 1, COL_W-1);	// 使用腐蚀滤波算法先对本行赛道进行预处理，返回跳变点数量
	Jump[59] = res;

        if((data[i][Mid_Line[61]] < BlackThres)
           || (data[i][Mid_Line[61]-2] < BlackThres)
             || (data[i][Mid_Line[61]+2] < BlackThres)
               || (data[i][Mid_Line[61]+4] < BlackThres)
                 || (data[i][Mid_Line[61]+6] < BlackThres))
	  {
              Weight_Left = Traversal_Left(i, data, &Mid_Left, 1, COL_W-1);	// 从左侧搜索边界
              Weight_Right = Traversal_Right(i, data, &Mid_Right, 1, COL_W-1);// 从右侧搜索边界
              if (Weight_Left >= Weight_Right && Weight_Left)	// 左赛道宽度大于右赛道宽度且不为0
              {
                Traversal_Left_Line(i, data, Left_Line, Right_Line);	// 使用左遍历获取赛道边界
              }
              else if (Weight_Left < Weight_Right && Weight_Right)
              {
                Traversal_Right_Line(i, data, Left_Line, Right_Line);	// 使用右遍历获取赛道边界
              }
              else	// 说明没查到
              {
                return 0;
              }

	  }
	else
	{
          Traversal_Mid_Line(i, data,
                             Mid_Line[i+2], 1, COL_W-1,
                             Left_Line, Right_Line,
                             Left_Add_Line, Right_Add_Line);	// 从前一行中点向两边扫描
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
		Mid_Line[61] = Mid_Line[59];	// 更新第60行虚拟中点，便于下一帧图像使用
	}
	Width_Real[61] = Width_Real[59];
	Width_Add[61] = Width_Add[59];
	Width_Min = Width_Add[59];

	return 1;
}
/*
*	补线修复
*
*	说明：有始有终才使用，直接使用两点斜率进行补线
*/
void Line_Repair(uint8 Start, uint8 Stop,uint8 (*data)[COL_W], int16 *Line, int16 *Line_Add, int16 *Add_Flag, int16 Mode)
{
	float res;
	int16 i;	// 控制行
	float Ka, Kb;

	if ((Mode == 1) && (Right_Add_Start <= Stop) && Start <= 53)	// 左边界补线
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
	if ((Mode == 2) && (Left_Add_Start <= Stop) && Start <= 53)	// 右边界补线
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
		if (Stop)	// 有始有终
		{
			if ((Right_Add_Stop >= MIDVALUE && Left_Add_Stop >= MIDVALUE) || (Right_Add_Stop >= MIDVALUE && Left_Add_Start <= Right_Add_Stop) || (Left_Add_Stop >= MIDVALUE && Right_Add_Start <= Left_Add_Stop))	// 只有较少行需要补线，不计算斜率，直接竖直向下补线
			{
				for (i = Stop-2; i <= 57; )
				{
					i += 2;
					Line_Add[i] = Line_Add[Stop];
				}
			}
			else	// 将起始行和结束行计算斜率补线
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
*	补线修复
*
*	说明：有始有终才使用，直接使用两点斜率进行补线
*/
void Line_Repair_island(uint8 Start, uint8 Stop,uint8 (*data)[COL_W], int16 *Line, int16 *Line_Add, int16 *Add_Flag, int16 Mode)
{
	float res;
	int16 i;	// 控制行
	float Ka, Kb;

	if ((Mode == 1) && (Right_Add_Start <= Stop) && Start <= 53)	// 左边界补线
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
	if ((Mode == 2) && (Left_Add_Start <= Stop) && Start <= 53)	// 右边界补线
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
		if (Stop)	// 有始有终
		{
			if ((Right_Add_Stop >= MIDVALUE && Left_Add_Stop >= MIDVALUE) || (Right_Add_Stop >= MIDVALUE && Left_Add_Start <= Right_Add_Stop) || (Left_Add_Stop >= MIDVALUE && Right_Add_Start <= Left_Add_Stop))	// 只有较少行需要补线，不计算斜率，直接竖直向下补线
			{
				for (i = Stop-2; i <= 57; )
				{
					i += 2;
					Line_Add[i] = Line_Add[Stop];
				}
			}

			else	// 将起始行和结束行计算斜率补线
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
void Mid_Line_Repair(uint8 count,uint8 (*data)[COL_W])//中线修复
{
	uint8 i;	// 控制行
        Left_Max = 0;
	Right_Min = COL_W-1;

	for (i = 59; i >= count;i-=2)
	{
		Mid_Line[i] = (Right_Add_Line[i] + Left_Add_Line[i]) / 2;	// 计算赛道中点
		Width_Add[i] = Right_Add_Line[i] - Left_Add_Line[i];		// 计算赛道宽度
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
//        //障碍处理
//        if(Left_Hazard_Flag)
//        {
//            Curve2_Fitting(&Left_Ka, &Left_Kb, 59,Left_Hazard_Flag, Left_Line, 1,21);
//            for (j=59;j>=Left_Hazard_Flag;j--)
//            {
//                Left_Add_Line[j] = Calculate_Add(j, Left_Ka,Left_Kb);	// 补线完成
//                Mid_Line[j] = (Right_Add_Line[j] + Left_Add_Line[j]) / 2;
//            }
//        }
//        if(Right_Hazard_Flag)
//        {
//            Curve2_Fitting(&Right_Ka, &Right_Kb, 59,Right_Hazard_Flag, Right_Line, 2,21);
//            for (j=59;j>=Right_Hazard_Flag;j--)
//            {
//                Right_Add_Line[j] = Calculate_Add(j, Right_Ka, Right_Kb);	// 补线完成
//                Mid_Line[j] = (Right_Add_Line[j] + Left_Add_Line[j]) / 2;
//            }
//
//        }
	}

	Mid_Line[61] = Mid_Line[59];
}
/*
*	图像处理算法
*
*	说明：处理普通图像，包括十字,障碍,断路，路障
*/
void Image_Handle(uint8 (*data)[COL_W])
{
    int16 i,j;	// 控制行
    int16 res;	// 用于结果状态判断

    Line_Count = 0;	// 赛道行数复位

    Left_Hazard_Flag = 0;	// 复位左右障碍物标志位
    Right_Hazard_Flag = 0;
    Hazard_Flag=0;

    Left_Add_Start = 0;		// 复位补线起始行坐标
    Right_Add_Start = 0;
    Left_Add_Stop = 0;
    Right_Add_Stop = 0;

    uint8 Limit_Left, Limit_Right;    //起跑线左右极限
    /**************************** 初始化 *************************************/
    for (i = 59; i >= 19;i-=2)  // 隔行处理，减小单片机负荷
    {
        Right_Line[i] = 93;     //右边界初始值
        Left_Line[i] = 1;	// 左边界初始值
        Left_Add_Flag[i] = 1;	// 初始化补线标志位
        Right_Add_Flag[i] = 1;  //同上
    }
    /***************************** 第一行特殊处理 *****************************/
	res = First_Line_Handle(data);
    /***************************** 第一行特殊处理 *****************************/
	if (res == 0)
	{
		Out_Side = 1;	// 丢线
		return;
	}
	Out_Side = 0;
	Line_Count = 59;

    /****************************** 图像处理 **********************************/
    for (i = 59; i >= 19;)	// 仅处理前40行图像，隔行后仅处理20行数据
    {
        i -= 2;	                // 隔行处理，减小单片机负荷

        /***********************起跑线检测*****************************/
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
        Jump[i] = Starting_Check(i, data, Limit_Left, Limit_Right); // 使用腐蚀滤波算法先对本行赛道进行预处理，返回跳变点数量
        if (Jump[i] >= 5 && i>= 21)
        {
                  Stop_Flag = 1;
        }

        if(data[i][Mid_Line[i+2]] < (BlackThres+5)
           &&data[i][Mid_Line[i+2]-2] < (BlackThres+5)
             &&data[i][Mid_Line[i+2]+2] < (BlackThres+5))
             //前2行中点在本行为黑点，可能是赛道结束，也可能是环路
        {
           i+=2;
           break;
        }
        else	// 使用前2行中点向两边扫描边界
        {
           Traversal_Mid_Line(i, data, Mid_Line[i+2], 1,COL_W-1,
                              Left_Line, Right_Line,
                              Left_Add_Line, Right_Add_Line);
        }
        /**************************** 补线检测开始 ****************************/
        if (Width_Real[i] > Width_Min+1)	// 赛道宽度变宽，可能是十字或环路，
        {                                       //Width_Min其实是第一行赛道宽度也就是
                                                //没有进行梯形矫正图像的赛道最长宽度
            if (Left_Add_Line[i] <=  Left_Add_Line[i+2]) //正常的是赛道远处宽度窄即Left_Add_Line[i]
                                                         //比较近处的大
			{
				if (!Left_Add_Flag[i])    //上面误判为0
				{
					Left_Add_Flag[i] = 1;	// 强制认定为需要补线
				}
			}
	    if (Right_Add_Line[i] >= Right_Add_Line[i+2])  //与上面相反
			{
				if (!Right_Add_Flag[i])   //上面误判为0
				{
					Right_Add_Flag[i] = 1;	// 强制认定为需要补线
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
        /*************************** 第一轮补线开始 ***************************/
		if (Left_Add_Flag[i])	// 左侧需要补线
		{
			if (i >= 55)	// 前三行补线不算
			{
				if (!Left_Add_Start)
				{
					Left_Add_Start = i;	// 记录补线开始行
					Left_Ka = 0;
					Left_Kb = Left_Add_Line[i];
				}
				Left_Add_Line[i] = Calculate_Add(i, Left_Ka, Left_Kb);	// 使用前一帧图像左边界斜率补线
			}
			else
			{
				if (!Left_Add_Start)	// 之前没有补线
				{
					Left_Add_Start = i;	// 记录左侧补线开始行
					Curve_Fitting(&Left_Ka, &Left_Kb, &Left_Add_Start, Left_Add_Line, Left_Add_Flag, 1);	// 使用两点法拟合直线
				}
				Left_Add_Line[i] = Calculate_Add(i, Left_Ka, Left_Kb);	// 补线完成
			}
		}
		else
		{
			if (Left_Add_Start)	// 已经开始补线
			{
				if (!Left_Add_Stop
                                    && !Left_Add_Flag[i+2]
                                      && !Left_Add_Flag[i+4])
				{
					if (Left_Add_Line[i] >= Left_Add_Line[i+2]
                                            && Left_Add_Line[i+2] >= Left_Add_Line[i+4])
					{
						Left_Add_Stop = i+4;	// 记录左侧补线结束行
                                                if(Ramp_Flag == 0)
                                                  Line_Repair(Left_Add_Start, Left_Add_Stop, data, Left_Line, Left_Add_Line, Left_Add_Flag, 1);
					}
				}
			}
		}

		if (Right_Add_Flag[i])	// 右侧需要补线
		{
			if (i >= 55)	// 前三行补线不算
			{
				if (!Right_Add_Start)
				{
					Right_Add_Start = i;	// 记录补线开始行
					Right_Ka = 0;
					Right_Kb = Right_Add_Line[i];
				}
				Right_Add_Line[i] = Calculate_Add(i, Right_Ka, Right_Kb);	// 使用前一帧图像右边界斜率补线
			}
			else
			{
				if (!Right_Add_Start)	// 之前没有补线
				{
					Right_Add_Start = i;	// 记录右侧补线开始行
					Curve_Fitting(&Right_Ka, &Right_Kb, &Right_Add_Start, Right_Add_Line, Right_Add_Flag, 2);	// 使用两点法拟合直线
				}
				Right_Add_Line[i] = Calculate_Add(i, Right_Ka, Right_Kb);	// 补线完成
			}
		}
		else
		{
			if (Right_Add_Start)	// 已经开始补线
			{
				if (!Right_Add_Stop && !Right_Add_Flag[i+2] && !Right_Add_Flag[i+4])
				{
					if (Right_Line[i] <= Right_Line[i+2] && Right_Line[i+2] <= Right_Line[i+4])
					{
						Right_Add_Stop = i+4;	// 记录右侧补线结束行
                                                if(Ramp_Flag == 0) Line_Repair(Right_Add_Start, Right_Add_Stop, data, Right_Line, Right_Add_Line, Right_Add_Flag, 2);
					}
				}
			}
		}
		/*************************** 第一轮补线结束 ***************************/
                Width_Add[i] = Right_Add_Line[i] - Left_Add_Line[i];	// 重新计算赛道宽度
		if ((Left_Add_Flag[i] && Right_Add_Flag[i]) || (!Left_Add_Flag[i] && !Right_Add_Flag[i]))
		{
			Width_Add[i] = Right_Add_Line[i] - Left_Add_Line[i];	// 重新计算赛道宽度
			Mid_Line[i] = (Right_Add_Line[i] + Left_Add_Line[i]) / 2;	// 计算中线
		}
		else
		{
			Width_Add[i] = Right_Add_Line[i] - Left_Add_Line[i];	// 重新计算赛道宽度
			Mid_Line[i] = Mid_Line[i+2];
		}
		if (Width_Add[i] < Width_Min)
		{
			Width_Min = Width_Add[i];	// 更新最小赛道宽度
		}

                Line_Count = i;                          //Line_Count更新

                if(Left_Add_Stop&&Right_Add_Stop)    //补线结束后又要补线这不补
                {
                    if(Left_Add_Flag[i]||Right_Add_Flag[i])
                    {
                        break;
                    }
                }
    }
    /*****************************环岛检测***********************************/
    if(fand_ring_time == 0) //进环岛
    {
        fand_left_ring();//左环岛检测
        fand_right_ring();//右环岛检测
    }
    if((flag1 == 1 || flag2 == 1)&& is_diu_flag == 5) //环岛中出环岛
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
     /*****************************环岛检测结束*****************************/

    /******************************坡道检测*********************************/
   // if(Ramp_Time_Count == 0)
    //{
    //     Ramp_Check();//开启坡道检测
   // }
    /****************************** 中线修复开始 ****************************/
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
*	加权平均
*
*	说明：权重是乱凑的，效果不好
*/
uint8 Line_Num=0;
uint8 Dynamic_Point_Weight(int8 Num)
{
    uint8 i,Point, Point_Mid;
    uint8 Count = 0;
    int8   Point_id;
    int32 Sum = 0, Weight_Count = 0;
    uint8 tep=0,L_Min=MIDVALUE,R_Max=MIDVALUE;

    if (Out_Side || Line_Count >= 57)	//出界或者摄像头图像异常
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

         for (i = 59; i >= Line_Count-Num;i-=2)		//使用加权平均
	   {
                     Point_id = i-Num;
                     if (Point_id>59)Point_id=59;
		       Sum += Mid_Line[Point_id] * Weight[59-i];
		     Weight_Count += Weight[59-i];
            }
	Point = range_protect(Sum / Weight_Count, 2, 92);//可以不限服保护

#if   SSSSSSS_OFF_ON
    S_temp=0;
    for (i = 57; i >= Line_Count+2; )		//检测小S弯道
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
        for (i = 61; i >= Line_Count+2; )		//计算小S弯道偏差
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

    Foresight =  0.8 * Error_Transform(Point_Mid, MIDVALUE)	//使用最远行偏差和加权偏差确定前瞻
               + 0.2 * Error_Transform(Point,  MIDVALUE);//+0.1*Ek_compar;

    Last_Foresight=Foresight;

    return Point;
}
/****************** 新算法 ******************/
/*
*	加权平均(差速)
*
*	说明：权重是乱凑的，效果不好
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
    for (i = 59; i >= Line_Count-Num;i-=2)		//使用加权平均
    {
        Point_id = i-Num;
        if (Point_id>59)Point_id=59;
        Sum += Mid_Line[Point_id] * Weight[59-i];
        Weight_Count += Weight[59-i];
    }
    Point = range_protect(Sum / Weight_Count, 2, 92);

#if   SSSSSSS_OFF_ON
    S_temp=0;
    for (i = 57; i >= Line_Count+2; )		//检测小S弯道
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
        for (i = 61; i >= Line_Count+2; )		//计算小S弯道偏差
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
                       &&Right_Line[i+6]!=COL_W-1)//左边有五行丢线
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
                       &&Left_Line[i+6]!=1)//右边有五行丢线
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
    int16 i;	// 控制行
	int16 res;	// 用于结果状态判断
	Line_Count = 0;	// 赛道行数复位

    Left_Add_Start = 0;		// 复位补线起始行坐标
	Right_Add_Start = 0;
	Left_Add_Stop = 0;
	Right_Add_Stop = 0;

    for (i = 59; i >= 19;)
    {
        i -= 2;
        Left_Add_Flag[i] = 1;	// 初始化补线标志位
        Right_Add_Flag[i] = 1;
        Right_Line[i] = 93;
        Left_Line[i] = 1;	// 给定边界初始值
    }
    /***************************** 第一行特殊处理 *****************************/
	res = First_Line_Handle(data);
    //	if (res == 0)
    //	{
    //		Out_Side = 1;	// 丢线
    //		return;
    //	}
	Out_Side = 0;
	Line_Count = 59;
    /*************************** 第一行特殊处理结束 ***************************/
    for (i = 59; i >= 19;)	// 仅处理前40行图像，隔行后仅处理20行数据
    {
        i -= 2;	// 隔行处理，减小单片机负荷
        if(data[i][Mid_Line[i+2]] < BlackThres)//前2行中点在本行为黑点，可能是赛道结束，也可能是环路
        {
            i += 2;
			break;
        }
        else	// 使用前2行中点向两边扫描边界
		{
			Traversal_Mid_Line(i, data, Mid_Line[i+2], 1,COL_W-1, Left_Line, Right_Line, Left_Add_Line, Right_Add_Line);
		}

        /**************************** 补线检测开始 ****************************/
        if (Width_Real[i] > Width_Min+1)//&&(Left_Add_Flag[i]||Right_Add_Flag[i]))	// 赛道宽度变宽，可能是十字或环路
        {
            if (Left_Add_Line[i] <=  Left_Add_Line[i+2])
			{
				if (!Left_Add_Flag[i])
				{
					Left_Add_Flag[i] = 1;	// 强制认定为需要补线
				}
			}
			if (Right_Add_Line[i] >= Right_Add_Line[i+2])
			{
				if (!Right_Add_Flag[i])
				{
					Right_Add_Flag[i] = 1;	// 强制认定为需要补线
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
        /*************************** 第一轮补线开始 ***************************/
		if (Left_Add_Flag[i])	// 左侧需要补线
		{
			if (i >= 55)	// 前三行补线不算
			{
				if (!Left_Add_Start)
				{
					Left_Add_Start = i;	// 记录补线开始行
					Left_Ka = 0;
					Left_Kb = Left_Add_Line[i];
				}
				Left_Add_Line[i] = Calculate_Add(i, Left_Ka, Left_Kb);	// 使用前一帧图像左边界斜率补线
			}
			else
			{
				if (!Left_Add_Start)	// 之前没有补线
				{
					Left_Add_Start = i;	// 记录左侧补线开始行
					Curve_Fitting(&Left_Ka, &Left_Kb, &Left_Add_Start, Left_Add_Line, Left_Add_Flag, 1);	// 使用两点法拟合直线
				}
				Left_Add_Line[i] = Calculate_Add(i, Left_Ka, Left_Kb);	// 补线完成
			}
		}
		else
		{
			if (Left_Add_Start)	// 已经开始补线
			{
				if (!Left_Add_Stop && !Left_Add_Flag[i+2])
				{
					if (Left_Add_Line[i] >= Left_Add_Line[i+2])
					{
						Left_Add_Stop = i;	// 记录左侧补线结束行
					}
				}
			}
		}

		if (Right_Add_Flag[i])	// 右侧需要补线
		{
			if (i >= 55)	// 前三行补线不算
			{
				if (!Right_Add_Start)
				{
					Right_Add_Start = i;	// 记录补线开始行
					Right_Ka = 0;
					Right_Kb = Right_Add_Line[i];
				}
				Right_Add_Line[i] = Calculate_Add(i, Right_Ka, Right_Kb);	// 使用前一帧图像右边界斜率补线
			}
			else
			{
				if (!Right_Add_Start)	// 之前没有补线
				{
					Right_Add_Start = i;	// 记录右侧补线开始行
					Curve_Fitting(&Right_Ka, &Right_Kb, &Right_Add_Start, Right_Add_Line, Right_Add_Flag, 2);	// 使用两点法拟合直线
				}
				Right_Add_Line[i] = Calculate_Add(i, Right_Ka, Right_Kb);	// 补线完成
			}
		}
		else
		{
			if (Right_Add_Start)	// 已经开始补线
			{
				if (!Right_Add_Stop && !Right_Add_Flag[i+2])
				{
					if (Right_Line[i] <= Right_Line[i+2])
					{
						Right_Add_Stop = i;	// 记录右侧补线结束行
					}
				}
			}
		}
		/*************************** 第一轮补线结束 ***************************/
                Width_Add[i] = Right_Add_Line[i] - Left_Add_Line[i];	// 重新计算赛道宽度
		if ((Left_Add_Flag[i] && Right_Add_Flag[i]) || (!Left_Add_Flag[i] && !Right_Add_Flag[i]))
		{
			Width_Add[i] = Right_Add_Line[i] - Left_Add_Line[i];	// 重新计算赛道宽度
			Mid_Line[i] = (Right_Add_Line[i] + Left_Add_Line[i]) / 2;	// 计算中线
		}
		else
		{
			Width_Add[i] = Right_Add_Line[i] - Left_Add_Line[i];	// 重新计算赛道宽度
			Mid_Line[i] = Mid_Line[i+2];
		}
		if (Width_Add[i] < Width_Min)
		{
			Width_Min = Width_Add[i];	// 更新最小赛道宽度
		}
		Line_Count = i;	// 记录成功识别到的赛道行数

                if(Left_Add_Stop&&Right_Add_Stop)    //补线结束后又要补线这不补
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
            for(int j=59;j>=21;j-=2)//超出的find_left_point_x       纠正中线，方便下次搜线，搜线用中线
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
            for(i = Line_Count; i >= 21; i -= 2)//纠正中线，方便下次搜线，搜线用中线
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
                       && (g_fSlope4 >= 0))//已经进入环岛 并找到清标志的条件 下一步准备找出环岛拐点
            {
                is_diu_flag = 4;
                tuqi_point_hang = 0;
            }

        }
        if(is_diu_flag == 4)
        {
            island_flag = 0;    //进环岛后立马请标志位停止布线。
            is_diu_flag = 5;     //出环补线标志位
        }

    }
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*************************** 第二轮补线修复结束 ***************************/

	/****************************** 中线修复开始 ******************************/
    for (i = Line_Count; i >= 21;i-=2)
	{
		Mid_Line[i] = (Right_Add_Line[i] + Left_Add_Line[i]) / 2;	// 计算赛道中点
        }
    Mid_Line_Repair(Line_Count, data);
}
void Right_island_Handle(uint8 (*data)[COL_W])
{
    int16 i;	// 控制行
    int16 res;	// 用于结果状态判断
    Line_Count = 0;	// 赛道行数复位

    Left_Add_Start = 0;		// 复位补线起始行坐标
    Right_Add_Start = 0;
    Left_Add_Stop = 0;
    Right_Add_Stop = 0;

    for (i = 59; i >= 19;)
    {
        i -= 2;
        Left_Add_Flag[i] = 1;	// 初始化补线标志位
        Right_Add_Flag[i] = 1;
        Right_Line[i] = 93;
        Left_Line[i] = 1;	// 给定边界初始值
    }

    /***************************** 第一行特殊处理 *****************************/
    res = First_Line_Handle(data);
    if (res == 0)
    {
            Out_Side = 1;	// 丢线
            return;
    }
    Out_Side = 0;
    Line_Count = 59;
    /*************************** 第一行特殊处理结束 ***************************/
    for (i = 59; i >= 19;)	// 仅处理前40行图像，隔行后仅处理20行数据
    {
        i -= 2;	// 隔行处理，减小单片机负荷
        if(data[i][Mid_Line[i+2]] < BlackThres)//前2行中点在本行为黑点，可能是赛道结束，也可能是环路
        {
            i += 2;
            break;
        }
        else	// 使用前2行中点向两边扫描边界
        {
                Traversal_Mid_Line(i, data, Mid_Line[i+2], 1,COL_W-1, Left_Line, Right_Line, Left_Add_Line, Right_Add_Line);
        }

        /**************************** 补线检测开始 ****************************/
        if (Width_Real[i] > Width_Min+1)//&&(Left_Add_Flag[i]||Right_Add_Flag[i]))	// 赛道宽度变宽，可能是十字或环路
        {
            if (Left_Add_Line[i] <=  Left_Add_Line[i+2])
			{
				if (!Left_Add_Flag[i])
				{
					Left_Add_Flag[i] = 1;	// 强制认定为需要补线
				}
			}
			if (Right_Add_Line[i] >= Right_Add_Line[i+2])
			{
				if (!Right_Add_Flag[i])
				{
					Right_Add_Flag[i] = 1;	// 强制认定为需要补线
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
        /*************************** 第一轮补线开始 ***************************/
		if (Left_Add_Flag[i])	// 左侧需要补线
		{
			if (i >= 55)	// 前三行补线不算
			{
				if (!Left_Add_Start)
				{
					Left_Add_Start = i;	// 记录补线开始行
					Left_Ka = 0;
					Left_Kb = Left_Add_Line[i];
				}
				Left_Add_Line[i] = Calculate_Add(i, Left_Ka, Left_Kb);	// 使用前一帧图像左边界斜率补线
			}
			else
			{
				if (!Left_Add_Start)	// 之前没有补线
				{
					Left_Add_Start = i;	// 记录左侧补线开始行
					Curve_Fitting(&Left_Ka, &Left_Kb, &Left_Add_Start, Left_Add_Line, Left_Add_Flag, 1);	// 使用两点法拟合直线
				}
				Left_Add_Line[i] = Calculate_Add(i, Left_Ka, Left_Kb);	// 补线完成
			}
		}
		else
		{
			if (Left_Add_Start)	// 已经开始补线
			{
				if (!Left_Add_Stop && !Left_Add_Flag[i+2])
				{
					if (Left_Add_Line[i] >= Left_Add_Line[i+2])
					{
						Left_Add_Stop = i;	// 记录左侧补线结束行
					}
				}
			}
		}

		if (Right_Add_Flag[i])	// 右侧需要补线
		{
			if (i >= 55)	// 前三行补线不算
			{
				if (!Right_Add_Start)
				{
					Right_Add_Start = i;	// 记录补线开始行
					Right_Ka = 0;
					Right_Kb = Right_Add_Line[i];
				}
				Right_Add_Line[i] = Calculate_Add(i, Right_Ka, Right_Kb);	// 使用前一帧图像右边界斜率补线
			}
			else
			{
				if (!Right_Add_Start)	// 之前没有补线
				{
					Right_Add_Start = i;	// 记录右侧补线开始行
					Curve_Fitting(&Right_Ka, &Right_Kb, &Right_Add_Start, Right_Add_Line, Right_Add_Flag, 2);	// 使用两点法拟合直线
				}
				Right_Add_Line[i] = Calculate_Add(i, Right_Ka, Right_Kb);	// 补线完成
			}
		}
		else
		{
			if (Right_Add_Start)	// 已经开始补线
			{
				if (!Right_Add_Stop && !Right_Add_Flag[i+2])
				{
					if (Right_Line[i] <= Right_Line[i+2])
					{
						Right_Add_Stop = i-2;	// 记录右侧补线结束行
					}
				}
			}
		}
		/*************************** 第一轮补线结束 ***************************/
                Width_Add[i] = Right_Add_Line[i] - Left_Add_Line[i];	// 重新计算赛道宽度
		if ((Left_Add_Flag[i] && Right_Add_Flag[i]) || (!Left_Add_Flag[i] && !Right_Add_Flag[i]))
		{
			Width_Add[i] = Right_Add_Line[i] - Left_Add_Line[i];	// 重新计算赛道宽度
			Mid_Line[i] = (Right_Add_Line[i] + Left_Add_Line[i]) / 2;	// 计算中线
		}
		else
		{
			Width_Add[i] = Right_Add_Line[i] - Left_Add_Line[i];	// 重新计算赛道宽度
			Mid_Line[i] = Mid_Line[i+2];
		}
		if (Width_Add[i] < Width_Min)
		{
			Width_Min = Width_Add[i];	// 更新最小赛道宽度
		}

		Line_Count = i;	// 记录成功识别到的赛道行数

                if(Left_Add_Stop&&Right_Add_Stop)    //补线结束后又要补线这不补
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
                           && image[i][j] > (BlackThres)) //找进环岛的切点
                        {
                            image_tiaobian_flag = 1;
                            find_point_1y = j;     //记录该列
                        }
                    }
                    else  //
                    {
                        if(image[i][j-1] - image[i][j] > 20
                           && image[i][j-1] > (BlackThres)
                             && image[i-2][j] < (BlackThres+10)) //找进环岛的上方拐点
                        {
                            if(find_point_1y - j < 15)
                            {
                                if(i%2==0)i=i-1;
                                find_right_point_x = i;  //记录上方拐点的行
                                find_right_point_y = j;  //记录上方拐点的列
                                image_tiaobian_flag = 2;
                                break;
                            }
                            else
                                break;
                        }
                    }
                }
            }
            Line_Count = find_right_point_x-2;   //识别的行数缩小，避免误差
            Left_Line[find_right_point_x] = find_right_point_y;  //将进环岛上方拐点左边界强制偏右
            Curve2_Fitting(&Left_Ka, &Left_Kb, 59,find_right_point_x, Left_Line, 1,30); //计算进环岛部分缺口处的左边界斜率
                                                                                        //为后面补线做准备
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
            for(int j=59;j>=21;j-=2)      //先使用找到的拐点来补线
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
                    zhaoguidian_diu_flag_num = j;//找到已经进入环岛的左线点
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
                Left_Add_Line[i] = Calculate_Add(i, Left_Ka, Left_Kb);//用已经进入环岛的点进行补线
            for(i = Line_Count; i >= 21; i -= 2)//纠正中线，方便下次搜线，搜线用中线
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
                       && (g_fSlope4 >= 0))//已经进入环岛 并找到清标志的条件 下一步准备找出环岛拐点
            {
                is_diu_flag = 4;
                tuqi_point_hang = 0;
            }

        }
        if(is_diu_flag == 4)//开始找出环岛拐点
        {
            island_flag = 0;    //进环岛后立马请标志位停止布线。
            is_diu_flag = 5;     //出环补线标志位
        }
    }
    for (i = Line_Count; i >= 21;i-=2)
    {
		Mid_Line[i] = (Right_Add_Line[i] + Left_Add_Line[i]) / 2;	// 计算赛道中点
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
//void go_island_set_value()   //入环岛
//{
//    Set = 0;    //环岛专属PID
//    CS_P_Right = 0.5;//差速系数
//    CS_P_Left = 0.5;
//}
//void out_island_set_value()  //出环岛
//{
//    Set = 0;    //环岛专属PID
//    CS_P_Right = 0.5;//差速系数
//    CS_P_Left = 0.5;
//}
/**************************** 坡道检测开始 ****************************/
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
        if (Left_Line[j] >= Left_Line[j+2]&&Right_Line[j] <= Right_Line[j+2]&&Left_Line[j]>Left_Line[59]&&Right_Line[j]<Right_Line[59])	//与前一行的边界实线比较
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
/**************************** 坡道检测结束 ****************************/


/**************************** 起跑线检测开始 ****************************/
uint8 Starting_Check(uint8 i, uint8 (*data)[COL_W], uint8 Left_Min, uint8 Right_Max)
{
    uint8 j;
    int White_Flag = 0;
    uint8 Jump_Count = 0;	// 跳变点计数

    Right_Max = range_protect(Right_Max, 1, 93);	// 保留右侧部分区域，防止溢出

    for (j = Left_Min; j < Right_Max; j++)	// 边界坐标 1到94
    {
          if (!White_Flag)	// 先查找左边界
          {
                  if(data[i][j] > BlackThres)	// 检测到白点
                  {
                          White_Flag = 1;		// 开始找黑点

                          continue;	// 结束本次循环，进入下次循环
                  }
          }
          else
          {
                  if(data[i][j] < BlackThres)	//检测黑点
                  {
                          Jump_Count ++;        // 视为一次跳变

                          if((data[i][j+1]< BlackThres) && j+1 <= Right_Max)	// 连续两个黑点
                          {
                                  if ((data[i][j+2]< BlackThres) && j+2 <= Right_Max)	// 连续三个黑点
                                  {
                                          if ((data[i][j+3]< BlackThres) && j+3 <= Right_Max)	// 连续四个黑点
                                          {
                                                  if ((data[i][j+4]< BlackThres) && j+4 <= Right_Max)	// 连续五个黑点
                                                  {
                                                          if ((data[i][j+5]< BlackThres) && j+5 <= Right_Max)	// 连续六个黑点
                                                          {
                                                                  if ((data[i][j+6]< BlackThres) && j+6 <= Right_Max)	// 连续七个黑点
                                                                  {
                                                                          if ((data[i][j+7]< BlackThres) && j+7 <= Right_Max)	// 连续八个黑点
                                                                          {
                                                                                  if ((data[i][j+8]< BlackThres) && j+8 <= Right_Max)	// 连续九个黑点
                                                                                  {
                                                                                          if ((data[i][j+9]< BlackThres) && j+9 <= Right_Max)	// 连续十个黑点
                                                                                          {
                                                                                                  if ((data[i][j+10]< BlackThres) && j+10 <= Right_Max)	// 连续11个黑点
                                                                                                  {
                                                                                                          White_Flag = 0;	// 认为不是干扰，不做任何处理，下次搜索白点
                                                                                                          j += 10;
                                                                                                  }
                                                                                                  else if (j+10 <= Right_Max)
                                                                                                  {
                                                                                                          data[i][j] < BlackThres;	// 仅有连续10个黑点，滤除掉
                                                                                                          data[i][j+1] < BlackThres;	// 仅有连续10个黑点，滤除掉
                                                                                                          data[i][j+2] < BlackThres;	// 仅有连续10个黑点，滤除掉
                                                                                                          data[i][j+3] < BlackThres;	// 仅有连续10个黑点，滤除掉
                                                                                                          data[i][j+4] < BlackThres;	// 仅有连续10个黑点，滤除掉
                                                                                                          data[i][j+5] < BlackThres;	// 仅有连续10个黑点，滤除掉
                                                                                                          data[i][j+6] < BlackThres;	// 仅有连续10个黑点，滤除掉
                                                                                                          data[i][j+7] < BlackThres;	// 仅有连续10个黑点，滤除掉
                                                                                                          data[i][j+8] < BlackThres;	// 仅有连续10个黑点，滤除掉
                                                                                                          data[i][j+9] < BlackThres;	// 仅有连续10个黑点，滤除掉

                                                                                                          j += 10;
                                                                                                  }
                                                                                                  else
                                                                                                  {
                                                                                                          j += 10;
                                                                                                  }
                                                                                          }
                                                                                          else if (j+9 <= Right_Max)
                                                                                          {
                                                                                                  data[i][j] < BlackThres;	// 仅有连续九个黑点，滤除掉
                                                                                                  data[i][j+1] < BlackThres;	// 仅有连续九个黑点，滤除掉
                                                                                                  data[i][j+2] < BlackThres;	// 仅有连续九个黑点，滤除掉
                                                                                                  data[i][j+3] < BlackThres;	// 仅有连续九个黑点，滤除掉
                                                                                                  data[i][j+4] < BlackThres;	// 仅有连续九个黑点，滤除掉
                                                                                                  data[i][j+5] < BlackThres;	// 仅有连续九个黑点，滤除掉
                                                                                                  data[i][j+6] < BlackThres;	// 仅有连续九个黑点，滤除掉
                                                                                                  data[i][j+7] < BlackThres;	// 仅有连续九个黑点，滤除掉
                                                                                                  data[i][j+8] < BlackThres;	// 仅有连续九个黑点，滤除掉

                                                                                                  j += 9;
                                                                                          }
                                                                                          else
                                                                                          {
                                                                                                  j += 9;
                                                                                          }
                                                                                  }
                                                                                  else if (j+8 <= Right_Max)
                                                                                  {
                                                                                          data[i][j] < BlackThres;	// 仅有连续八个黑点，滤除掉
                                                                                          data[i][j+1] < BlackThres;	// 仅有连续八个黑点，滤除掉
                                                                                          data[i][j+2] < BlackThres;	// 仅有连续八个黑点，滤除掉
                                                                                          data[i][j+3] < BlackThres;	// 仅有连续八个黑点，滤除掉
                                                                                          data[i][j+4] < BlackThres;	// 仅有连续八个黑点，滤除掉
                                                                                          data[i][j+5] < BlackThres;	// 仅有连续八个黑点，滤除掉
                                                                                          data[i][j+6] < BlackThres;	// 仅有连续八个黑点，滤除掉
                                                                                          data[i][j+7] < BlackThres;	// 仅有连续八个黑点，滤除掉

                                                                                          j += 8;
                                                                                  }
                                                                                  else
                                                                                  {
                                                                                          j += 8;
                                                                                  }
                                                                          }
                                                                          else if (j+7 <= Right_Max)
                                                                          {
                                                                                  data[i][j] < BlackThres;	// 仅有连续七个黑点，滤除掉
                                                                                  data[i][j+1] < BlackThres;	// 仅有连续七个黑点，滤除掉
                                                                                  data[i][j+2] < BlackThres;	// 仅有连续七个黑点，滤除掉
                                                                                  data[i][j+3] < BlackThres;	// 仅有连续七个黑点，滤除掉
                                                                                  data[i][j+4] < BlackThres;	// 仅有连续七个黑点，滤除掉
                                                                                  data[i][j+5] < BlackThres;	// 仅有连续七个黑点，滤除掉
                                                                                  data[i][j+6] < BlackThres;	// 仅有连续七个黑点，滤除掉

                                                                                  j += 7;
                                                                          }
                                                                          else
                                                                          {
                                                                                  j += 7;
                                                                          }
                                                                  }
                                                                  else if (j+6 <= Right_Max)
                                                                  {
                                                                          data[i][j] < BlackThres;	// 仅有连续六个黑点，滤除掉
                                                                          data[i][j+1] < BlackThres;	// 仅有连续六个黑点，滤除掉
                                                                          data[i][j+2] < BlackThres;	// 仅有连续六个黑点，滤除掉
                                                                          data[i][j+3] < BlackThres;	// 仅有连续六个黑点，滤除掉
                                                                          data[i][j+4] < BlackThres;	// 仅有连续六个黑点，滤除掉
                                                                          data[i][j+5] < BlackThres;	// 仅有连续六个黑点，滤除掉

                                                                          j += 6;
                                                                  }
                                                                  else
                                                                  {
                                                                          j += 6;
                                                                  }
                                                          }
                                                          else if (j+5 <= Right_Max)
                                                          {
                                                                  data[i][j] < BlackThres;	// 仅有连续五个黑点，滤除掉
                                                                  data[i][j+1] < BlackThres;	// 仅有连续五个黑点，滤除掉
                                                                  data[i][j+2] < BlackThres;	// 仅有连续五个黑点，滤除掉
                                                                  data[i][j+3] < BlackThres;	// 仅有连续五个黑点，滤除掉
                                                                  data[i][j+4] < BlackThres;	// 仅有连续五个黑点，滤除掉

                                                                  j += 5;
                                                          }
                                                          else
                                                          {
                                                                  j += 5;
                                                          }
                                                  }
                                                  else if (j+4 <= Right_Max)
                                                  {
                                                          data[i][j] < BlackThres;	// 仅有连续四个黑点，滤除掉
                                                          data[i][j+1] < BlackThres;	// 仅有连续四个黑点，滤除掉
                                                          data[i][j+2] < BlackThres;	// 仅有连续四个黑点，滤除掉
                                                          data[i][j+3] < BlackThres;	// 仅有连续四个黑点，滤除掉

                                                          j += 4;
                                                  }
                                                  else
                                                  {
                                                          j += 4;
                                                  }
                                          }
                                          else if (j+3 <= Right_Max)
                                          {
                                                  data[i][j] < BlackThres;	// 仅有连续三个黑点，滤除掉
                                                  data[i][j+1] < BlackThres;	// 仅有连续三个黑点，滤除掉
                                                  data[i][j+2] < BlackThres;	// 仅有连续三个黑点，滤除掉

                                                  j += 3;
                                          }
                                          else
                                          {
                                                  j += 3;
                                          }
                                  }
                                  else if (j+2 <= Right_Max)
                                  {
                                          data[i][j] < BlackThres;	// 仅有连续两个黑点，滤除掉
                                          data[i][j+1] < BlackThres;	// 仅有连续两个黑点，滤除掉

                                          j += 2;
                                  }
                                  else
                                  {
                                          j += 2;
                                  }
                          }
                          else if (j+1 <= Right_Max)
                          {
                                  data[i][j] < BlackThres;	// 有一个黑点，滤除掉

                                  j += 1;
                          }
                          else
                          {
                                  j += 1;
                          }
                  }
          }
  }

   return Jump_Count;	// 返回跳变点计数
}
/*进环岛以后检测出环岛拐点并补线出去*/
void out_ring_check(int a)
{
 uint8 count_flag = 0;
 uint8 image_tiaobian_flag = 0;
 uint8 out_find_point_1x = 0;
 out_ring_num = 0;
 int16 i = 55;
 if(a == 1)   //右环岛
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
                     && image[k-1][j] < BlackThres+5) //找拐点
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
   if(out_ring_flag)//找到出环岛拐点 开始补线
   {
//        led(LED1,LED_ON);   //出环岛指示灯开启
//        led(LED2,LED_OFF);  //进环岛指示灯关闭
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
                for(int aaaaa = 29; aaaaa <= 57; aaaaa += 2)//找 出环岛的标志
                {
                    Outring_Left_Line[aaaaa] = Calculate_Add(aaaaa, Left_Ka, Left_Kb);
                    if(Left_Line[aaaaa] <= (Outring_Left_Line[aaaaa]+2) && Left_Line[aaaaa] >= (Outring_Left_Line[aaaaa]-2))
                    {
                       if(++count_flag >= 13)  ////出环岛
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
 else  //左环岛
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
//            led(LED1,LED_ON);   //出环岛指示灯开启
//            led(LED2,LED_OFF);  //进环岛指示灯关闭
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

                        if(++count_flag >= 13)  //出环岛
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