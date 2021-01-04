/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2016,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��179029047
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		SEEKFREE_TSL1401.h
 * @brief      		�캢��(����CCD)������
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		Go For It(1325536866)
 * @version    		v1.0
 * @Software 		IAR 7.7 or MDK 5.17
 * @Target core		MK60DN512VLL10
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2016-02-25
 * @note	
					TSL1401���߶��壺
					------------------------------------ 
					    ģ��ܽ�       	��Ƭ���ܽ�
					    CCD_AO	   		B0
					    CCD_CLK	   		E4
					    CCD_SI	   		E5
					------------------------------------ 
 ********************************************************************************************************************/



#ifndef _TSL1401_c
#define _TSL1401_c

#include "headfile.h"

extern uint16 ccd_data[128];                   //CCD����


void ccd_init(void);
void ccd_collect(void);
















#endif

