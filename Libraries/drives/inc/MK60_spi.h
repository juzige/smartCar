/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2017,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：179029047
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		MK60_spi
 * @company	   		成都逐飞科技有限公司
 * @author     		Go For It(1325536866)
 * @version    		v2.0
 * @Software 		IAR 7.7 or MDK 5.17
 * @Target core		MK60DN and MK60FX
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2017-09-19
 ********************************************************************************************************************/
 
 
 
#ifndef _MK60_spi_h
#define _MK60_spi_h


#include "misc.h"
#include "common.h"
#include "MK60_port.h"
#include "MK60_gpio.h"
#include "MK60_port_cfg.h"
#include "stdlib.h"




//主从机模式

typedef enum
{
    MASTER,    //主机模式
    SLAVE      //从机模式
} SPI_CFG;


//模块号
typedef enum
{
    spi0,
    spi1,
    spi2
} SPIn_e;


//SPI模块片选号
typedef enum
{
    SPI_PCS0 = 1 << 0,
    SPI_PCS1 = 1 << 1,
    SPI_PCS2 = 1 << 2,
    SPI_PCS3 = 1 << 3,
    SPI_PCS4 = 1 << 4,
    SPI_PCS5 = 1 << 5,
} SPI_PCSn_e;

uint32 spi_init(SPIn_e spin, SPI_PCSn_e pcs, SPI_CFG master,uint32 baud);
void spi_mosi(SPIn_e spin, SPI_PCSn_e pcs, uint8 *modata, uint8 *midata, uint32 len);
void spi_mosi_cmd(SPIn_e spin, SPI_PCSn_e pcs, uint8 *mocmd , uint8 *micmd , uint8 *modata , uint8 *midata, uint32 cmdlen , uint32 len);





#endif

