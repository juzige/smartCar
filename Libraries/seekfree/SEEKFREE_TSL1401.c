/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2016,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��179029047
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		SEEKFREE_TSL1401.c
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



#include "SEEKFREE_TSL1401.h"




#define CCD_CLK_PORT   E4                     	//CCD CLK���Ŷ���
#define CCD_SI_PORT    E5                     	//CCD SI ���Ŷ���
#define AD_CHANNEL     ADC0_SE8                 //����AD�ɼ�ͨ��  B0
#define EXPOSURE_TIME  10                       //����CCD�ع�ʱ�� ��λMS



#define CCD_CLK(x)     gpio_set (CCD_CLK_PORT, x)
#define CCD_SI(x)      gpio_set (CCD_SI_PORT,  x)


uint16 ccd_data[128];                           //CCD����

//-------------------------------------------------------------------------------------------------------------------
//  @brief      TSL1401����CCD��ʼ��
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:			
//-------------------------------------------------------------------------------------------------------------------
void ccd_init(void)     
{       
    adc_init (AD_CHANNEL);                      //B0 1��
    gpio_init(CCD_CLK_PORT, GPO, 1);            //CLK
    gpio_init(CCD_SI_PORT,  GPO, 1);            //SI
    
    DisableInterrupts;
    pit_init_ms(pit0, EXPOSURE_TIME);   		//��ʱEXPOSURE_TIME(ms) ���ж�
    enable_irq(PIT0_IRQn);                      //ʹ��PIT0�ж�
    EnableInterrupts;
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      TSL1401����CCD���ݲɼ�
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:				��isr.c�����ȴ�����Ӧ���жϺ�����Ȼ����øú���(֮�����������жϱ�־λ)
//-------------------------------------------------------------------------------------------------------------------
void ccd_collect(void)
{
    uint16 i = 0;

 
    CCD_CLK(1);
    CCD_SI(0);
    

    CCD_SI(1);
    CCD_CLK(0);
    

    CCD_CLK(1);
    CCD_SI(0);
    
    
    for(i=0;i<128;i++)
    {
        CCD_CLK(0);   
        ccd_data[i] = adc_once(AD_CHANNEL, ADC_12bit);     
        CCD_CLK(1);
    }
}
