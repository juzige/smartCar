 /*========================上位机设置=============================*/
/*     可用蓝牙无线通讯和51下载器有线通讯*/

#ifndef __VARIADLE_H
#define __VARIADLE_H


#include "headfile.h"

//取一个数据的各个位
#define BYTE0(Temp)       (*(char *)(&Temp))     
#define BYTE1(Temp)       (*((char *)(&Temp) + 1))
#define BYTE2(Temp)       (*((char *)(&Temp) + 2))
#define BYTE3(Temp)       (*((char *)(&Temp) + 3))

extern float var_data[20];

/*用来通知上位机新的一组数据开始，要保存数据必须发送它*/

void Send_Begin();

void Send_Variable();

#endif //__VARIABLE_H
