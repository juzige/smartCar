/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2016,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：179029047
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		SEEKFREE_IPS114_SPI.h
 * @brief      		IPS 液晶驱动
 * @company	   		成都逐飞科技有限公司
 * @author     		Go For It(1325536866)
 * @version    		v1.0
 * @Software 		IAR 8.3 or MDK 5.24
 * @Target core		MK60DN512VLL10
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2019-10-10
 * @note
					IPS液晶接线定义：
					------------------------------------
						模块管脚            单片机管脚
						SCL                 查看SEEKFREE_IPS114_SPI.h文件内的IPS114_SCL		宏定义     硬件SPI引脚不可随意切换
						SDA                 查看SEEKFREE_IPS114_SPI.h文件内的IPS114_SDA		宏定义     硬件SPI引脚不可随意切换
						RES                 查看SEEKFREE_IPS114_SPI.h文件内的IPS114_REST_PIN宏定义
						DC                  查看SEEKFREE_IPS114_SPI.h文件内的IPS114_DC_PIN	宏定义
						CS                  查看SEEKFREE_IPS114_SPI.h文件内的IPS114_CS		宏定义     硬件SPI引脚不可随意切换

						电源引脚
						BL  3.3V电源（背光控制引脚，也可以接PWM来控制亮度）
						VCC 3.3V电源
						GND 电源地
						最大分辨率135*240
					------------------------------------

 ********************************************************************************************************************/


#ifndef _SEEKFREE_IPS114_H
#define _SEEKFREE_IPS114_H


#include "headfile.h"


//-----------------引脚定义------------------------------
//#define IPS114_BL_PIN      B7	            //主板已接vcc
#define IPS114_REST_PIN    D15             	//液晶复位引脚定义
#define IPS114_DC_PIN 	  D14	            //液晶命令位引脚定义


#define IPS114_DC(x)       gpio_set(IPS114_DC_PIN,x);
#define IPS114_REST(x)     gpio_set(IPS114_REST_PIN,x);





#define IPS114_W   135
#define IPS114_H   240



//-------常用颜色在SEEKFREE_FONT.h文件中定义----------
//#define RED          	0xF800	//红色
//#define BLUE         	0x001F  //蓝色
//#define YELLOW       	0xFFE0	//黄色
//#define GREEN        	0x07E0	//绿色
//#define WHITE        	0xFFFF	//白色
//#define BLACK        	0x0000	//黑色
//#define GRAY  		0X8430 	//灰色
//#define BROWN 		0XBC40 	//棕色
//#define PURPLE    	0XF81F	//紫色
//#define PINK    		0XFE19	//粉色



//定义写字笔的颜色
#define IPS114_PENCOLOR    RED

//定义背景颜色
#define IPS114_BGCOLOR     WHITE



//定义显示方向
//0 竖屏模式
//1 竖屏模式  旋转180
//2 横屏模式
//3 横屏模式  旋转180
#define IPS114_DISPLAY_DIR 3

#if (0==IPS114_DISPLAY_DIR || 1==IPS114_DISPLAY_DIR)
#define	IPS114_X_MAX	IPS114_W	//液晶X方宽度
#define IPS114_Y_MAX	IPS114_H   	//液晶Y方宽度

#elif (2==IPS114_DISPLAY_DIR || 3==IPS114_DISPLAY_DIR)
#define	IPS114_X_MAX	IPS114_H	//液晶X方宽度
#define IPS114_Y_MAX	IPS114_W   	//液晶Y方宽度

#else
#error "IPS114_DISPLAY_DIR 定义错误"

#endif


void ips114_init(void);
void ips114_clear(uint16 color);
void ips114_drawpoint(uint16 x,uint16 y,uint16 color);
void ips114_showchar(uint16 x,uint16 y,uint8 dat);
void ips114_showstr(uint16 x,uint16 y,uint8 dat[]);
void ips114_showint8(uint16 x,uint16 y,int8 dat);
void ips114_showuint8(uint16 x,uint16 y,uint8 dat);
void ips114_showint16(uint16 x,uint16 y,int16 dat);
void ips114_showuint16(uint16 x,uint16 y,uint16 dat);
void ips114_displayimage032(uint8 *p, uint16 width, uint16 height);
void ips114_displayimage032_zoom(uint8 *p, uint16 width, uint16 height, uint16 dis_width, uint16 dis_height);
void ips114_displayimage7725(uint8 *p, uint16 width, uint16 height);
void ips114_display_chinese(uint16 x, uint16 y, uint8 size, const uint8 *p, uint8 number, uint16 color);

void ips114_writeIndex(uint8 dat)	;
void ips114_writeData(uint8 dat);			//写数据
void ips114_writedata_16bit(uint16 dat);	//向液晶屏写一个16位数据

#endif
