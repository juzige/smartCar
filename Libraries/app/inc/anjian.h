#ifndef _anjian_h_
#define _anjian_h_

#include "headfile.h"




/* 按键管脚定义 */ 

#define FLASH     E3
#define UP        E2
#define DOWN      E5
#define TRUE      E4
#define CANCEL    E6


void KEY_Init(void);
uint8 KEY_Read(uint8 mode);


#endif 