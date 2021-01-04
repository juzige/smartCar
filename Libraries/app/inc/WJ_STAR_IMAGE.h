#ifndef WJ_STAR_IMAGE_H
#define WJ_STAR_IMAGE_H

#include "headfile.h"

#define MIDVALUE       48   //46
/*****************************�궨������******************************/
#define ROW_H        60
#define COL_W        94
/***************************ö�ٶ�������*****************************/

/************************ȫ�ֱ�����������****************************/

extern float  BlackThres;   //�ڰ���ֵ

//extern int16 Is_Right_Line[62],Is_Left_Line[62];
extern int16 Left_Add_Line[62], Right_Add_Line[62];		// ���ұ߽粹������
extern int16 Left_Line[62], Right_Line[62], Mid_Line[62];	// ԭʼ���ұ߽�����

extern int16  EdgeThres; //��ֵ

extern uint8 left_right_guai_flag;//�ж��Ǵ���ת�仹�Ǵ���ת��
/****************************    ����ʶ�����    ****************************/
extern uint8   Stop_Flag,count_num;


extern int16 Line_Count;	// ��¼�ɹ�ʶ�𵽵���������
extern int16 Foresight;
extern float Left_Ka,Left_Kb,Right_Ka,Right_Kb;
void Image_Get(void);
void yuzhiget(void);
void Image_Handle(uint8 (*data)[COL_W]);
uint8 Dynamic_Point_Weight(int8 Num);
uint8  Motor_Point_Weight(int8 Num);

extern int fand_ring_time;  //��Բ��֮��1s���ڼ��Բ��
extern int16 Left_Add_Start, Right_Add_Start;	// ���Ҳ�����ʼ������
extern int16 Left_Add_Stop, Right_Add_Stop;	// ���Ҳ��߽���������
extern int tuqi_point_hang,out_ring_point_hang;

extern int island_left_mid_add_flag,island_right_mid_add_flag;

extern int out_ring_num;
extern int island_flag;  //��־Ϊ0���ǻ���   ��־Ϊ1���󻷵�   ��־Ϊ2���һ���
extern int is_diu_flag;
extern uint8 island_test_time;

void fand_left_ring();
void fand_right_ring();
void image_processing(uint8 (*data)[COL_W]);
void go_island_set_value();
void out_island_set_value();
void Ramp_Check(void);
extern uint8 Ramp_Flag;
extern uint8 Ramp_Time_Count;

void out_ring_check(int a);

uint8 Starting_Check(uint8 i, uint8 (*data)[COL_W], uint8 Left_Min, uint8 Right_Max);
extern int16 Out_Side;

#endif //__IMAGE_PROCESS_H__

