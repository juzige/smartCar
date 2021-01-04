/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2017,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：179029047
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		MK60_pit
 * @company	   		成都逐飞科技有限公司
 * @author     		Go For It(1325536866)
 * @version    		v2.0
 * @Software 		IAR 7.7 or MDK 5.17
 * @Target core		MK60DN and MK60FX
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2017-09-19
 ********************************************************************************************************************/
 
 
 
#ifndef _MK60_pit_h
#define _MK60_pit_h

#include "misc.h"
#include "common.h"




//定义PIT模块号
typedef enum
{
    pit0,
    pit1,
    pit2,
    pit3,
} PITn;

#define     PIT_FlAG_CLR(PITn)          PIT->CHANNEL[PITn].TFLG |= PIT_TFLG_TIF_MASK        //清中断标志（写1 清空标志位）


void pit_init(PITn pitn, uint32 cnt);
void pit_delay(PITn pitn, uint32 cnt);
void pit_time_start(PITn pitn);
uint32 pit_time_get(PITn pitn);
void pit_close(PITn pitn);

//------------------------------------以下代码用于PIT中断------------------------------------
#define     pit_init_ms(PITn_e,ms)          pit_init(PITn_e,ms * bus_clk_mhz*1000);          //初始化PITn，并设置定时时间(单位为 ms)
#define     pit_init_us(PITn_e,us)          pit_init(PITn_e,us * bus_clk_mhz);               //初始化PITn，并设置定时时间(单位为 us)
#define     pit_init_ns(PITn_e,ns)          pit_init(PITn_e,ns * bus_clk_mhz/1000);          //初始化PITn，并设置定时时间(单位为 ns)


//------------------------------------以下代码用于PIT延时------------------------------------
#define     pit_delay_ms(PITn_e,ms)          pit_delay(PITn_e,ms * bus_clk_mhz*1000);        //PIT延时 ms
#define     pit_delay_us(PITn_e,us)          pit_delay(PITn_e,us * bus_clk_mhz);             //PIT延时 us
#define     pit_delay_ns(PITn_e,ns)          pit_delay(PITn_e,ns * bus_clk_mhz/1000);        //PIT延时 ns(100M总线频率时最小单位为10ns)


#endif
