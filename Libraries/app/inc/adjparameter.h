#ifndef _ADJPARAMETER_H
#define _ADJPARAMETER_H


#include "headfile.h"
#define PARANUM 29
#define ADDR    0x00
#define DEBUG_TIME_MS  12.0*debug_time/FOSC*1000

//定义flash最后一个扇区，避免与程序冲突
//存储参数最好从最后一个扇区开始使用
#define EXAMPLE_FLASH_SECTOR        (FLASH_SECTOR_NUM-1)

//定义所在扇区的页编号
#define EXAMPLE_FLASH_SECTOR_PAGE   (0)

extern uint8 car_qidong;
extern uint8 buzzer_flag;
extern uint8 mode_flag;
extern uint16 debug_time;
extern int16 l_speed,r_speed;
extern uint8  mode_flag;             //标志位清零
extern uint8 var[32];
extern uint8 debug_times;

//通过按键调参
void parameter_adj_bykey();

//拨码开关初始化
void Switch_Init(void);

//通过拨码开关决定读取数据
void Flash_Read(uint8 k);

//读取模式位
void Mode_Read(void);

void buzzer(uint8 x);





#endif //_ADJPARAMETER_H
