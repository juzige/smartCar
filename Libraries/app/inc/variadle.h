 /*========================��λ������=============================*/
/*     ������������ͨѶ��51����������ͨѶ*/

#ifndef __VARIADLE_H
#define __VARIADLE_H


#include "headfile.h"

//ȡһ�����ݵĸ���λ
#define BYTE0(Temp)       (*(char *)(&Temp))     
#define BYTE1(Temp)       (*((char *)(&Temp) + 1))
#define BYTE2(Temp)       (*((char *)(&Temp) + 2))
#define BYTE3(Temp)       (*((char *)(&Temp) + 3))

extern float var_data[20];

/*����֪ͨ��λ���µ�һ�����ݿ�ʼ��Ҫ�������ݱ��뷢����*/

void Send_Begin();

void Send_Variable();

#endif //__VARIABLE_H
