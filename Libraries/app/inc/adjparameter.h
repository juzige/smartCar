#ifndef _ADJPARAMETER_H
#define _ADJPARAMETER_H


#include "headfile.h"
#define PARANUM 29
#define ADDR    0x00
#define DEBUG_TIME_MS  12.0*debug_time/FOSC*1000

//����flash���һ������������������ͻ
//�洢������ô����һ��������ʼʹ��
#define EXAMPLE_FLASH_SECTOR        (FLASH_SECTOR_NUM-1)

//��������������ҳ���
#define EXAMPLE_FLASH_SECTOR_PAGE   (0)

extern uint8 car_qidong;
extern uint8 buzzer_flag;
extern uint8 mode_flag;
extern uint16 debug_time;
extern int16 l_speed,r_speed;
extern uint8  mode_flag;             //��־λ����
extern uint8 var[32];
extern uint8 debug_times;

//ͨ����������
void parameter_adj_bykey();

//���뿪�س�ʼ��
void Switch_Init(void);

//ͨ�����뿪�ؾ�����ȡ����
void Flash_Read(uint8 k);

//��ȡģʽλ
void Mode_Read(void);

void buzzer(uint8 x);





#endif //_ADJPARAMETER_H
