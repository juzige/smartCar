/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2018,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��179029047
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		�����ļ�
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ3184284598)
 * @version    		�鿴LPC546XX_config.h�ļ��ڰ汾�궨��
 * @Software 		IAR 8.3 or MDK 5.26
 * @Target core		MK60DN512VLL10
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2019-10-10
 ********************************************************************************************************************/
#ifndef _SEEKFREE_FONT_h
#define _SEEKFREE_FONT_h

#include "common.h"


extern const uint8 tft_ascii[95][16];
extern const uint8  asc2_1608[1520];
extern const uint8 chinese_test[8][16];



//-------������ɫ----------
#define RED    0xF800//��ɫ
#define BLUE   0x001F//��ɫ
#define YELLOW 0xFFE0//��ɫ
#define GREEN  0x07E0//��ɫ
#define WHITE  0xFFFF//��ɫ
#define BLACK  0x0000//��ɫ 
#define GRAY   0X8430//��ɫ
#define BROWN  0XBC40//��ɫ
#define PURPLE 0XF81F//��ɫ
#define PINK   0XFE19//��ɫ


#endif
