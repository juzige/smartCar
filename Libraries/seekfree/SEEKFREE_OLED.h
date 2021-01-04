/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2016,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��179029047
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		SEEKFREE_OLED.h
 * @brief      		0.96OLED������
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		Go For It(1325536866)
 * @version    		v1.0
 * @Software 		IAR 7.7 or MDK 5.17
 * @Target core		MK60DN512VLL10
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2016-02-25
 * @note	
					NRF24L01���߶��壺
					------------------------------------ 
					    OLEDҺ��    ��Ƭ��                        
    					D0          A15
    					D1          A16            
    					RES         A14    
    					DC          B16
					------------------------------------ 
 ********************************************************************************************************************/



#ifndef _SEEKFREE_OELD_H
#define _SEEKFREE_OELD_H

#include "headfile.h"



//----�궨��OLED����----	 
#define  OLED_SCL_PIN	A15
#define  OLED_SDA_PIN	A16
#define  OLED_RST_PIN	A14
#define  OLED_DC_PIN	B16
#define  OLED_CS_PIN	A14


void Dly_ms(uint16 ms);
void OLED_Init(void);
void OLED_Fill(uint8 dat);
void OLED_WrDat(uint8 data);
void OLED_Wr6Dat(uint8 data);
void OLED_PutPixel(uint8 x,uint8 y);
void OLED_ClrPixel(uint8 x,uint8 y);
void OLED_Set_Pos(uint8 x, uint8 y);
void OLED_P6x8Str(uint8 x,uint8 y,uint8 ch[]);
void OLED_P8x16Str(uint8 x,uint8 y,uint8 ch[]);
void OLED_HEXACSII(uint16 hex,uint8* Print);
void OLED_Print_Num(uint8 x, uint8 y, uint16 num);
void OLED_Print_Num1(uint8 x, uint8 y, int16 num);
void dis_bmp(uint16 high, uint16 width, uint8 *p,uint8 value);



/********************************************************************/

#endif
