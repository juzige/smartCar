/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2016,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��179029047
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		SEEKFREE_MMA8451.c
 * @brief      		��ϸ��(MMA8451)������
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		Go For It(1325536866)
 * @version    		v1.0
 * @Software 		IAR 7.7 or MDK 5.17
 * @Target core		MK60DN512VLL10
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2016-02-25
 * @note	
					MMA8451���߶���
					------------------------------------ 
						SDA                 C17
						SCL                 C16
					------------------------------------ 
 ********************************************************************************************************************/



#include "SEEKFREE_MMA8451.h"


//SA0����ӵ�

int16 acc_x = 0, acc_y = 0, acc_z = 0;


//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ʼ��MMA8451���ٶȼ�
//  @param      NULL
//  @return     uint8			����1�ɹ�������0ʧ��						
//  @since      v1.0
//  Sample usage:				���øú���ǰ�����ȵ���ģ��IIC�ĳ�ʼ��
//-------------------------------------------------------------------------------------------------------------------
uint8 MMA845x_init(void)
{       
    uint16 v;	
    simiic_write_reg(MMA8451_DEV_ADD, CTRL_REG1,ASLP_RATE_20MS+DATA_RATE_2500US);	
          
    simiic_write_reg(MMA8451_DEV_ADD, XYZ_DATA_CFG_REG, FULL_SCALE_2G); //2G
         
    simiic_write_reg(MMA8451_DEV_ADD, CTRL_REG1, (ACTIVE_MASK+ASLP_RATE_20MS+DATA_RATE_5MS)&(~FREAD_MASK)); //����״̬   14bit
         

    v= simiic_read_reg(MMA8451_DEV_ADD, WHO_AM_I_REG, IIC);
    if((v == MMA8451Q_ID)||(v == MMA8452Q_ID)||(v == MMA8453Q_ID)) return 1;
	
	return 0;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ȡMMA8451���ٶȼ�����
//  @param      NULL
//  @return     void						
//  @since      v1.0
//  Sample usage:				ִ�иú�����ֱ�Ӳ鿴��Ӧ�ı�������
//-------------------------------------------------------------------------------------------------------------------
void get_mma8451(void)
{	    
    uint8 x,y,z;
    uint16 wx,wy,wz;
    
    x = simiic_read_reg(MMA8451_DEV_ADD, OUT_X_MSB_REG, IIC); 
    wx = ((simiic_read_reg(MMA8451_DEV_ADD, OUT_X_LSB_REG, IIC))|x<<8);  
    
    //����������ٶ�ԭʼ��Ӧ��ֵ
    if(x>0x7f)  acc_x=-(int16)((~(wx>>2) + 1)&0X3FFF); //��λȡ����һ��ȥ����Ч�ַ� 
    else        acc_x=(wx>>2)&0X3FFF; 	  //��λ����Ч�ַ� 	
    
    
    y = simiic_read_reg(MMA8451_DEV_ADD, OUT_Y_MSB_REG, IIC); 
    wy = ((simiic_read_reg(MMA8451_DEV_ADD, OUT_Y_LSB_REG, IIC))|y<<8);  
    
    //����������ٶ�ԭʼ��Ӧ��ֵ
    if(y>0x7f)  acc_y=-(int16)((~(wy>>2) + 1)&0X3FFF); //��λȡ����һ��ȥ����Ч�ַ� 
    else        acc_y=(wy>>2)&0X3FFF; 	  //��λ����Ч�ַ� 	
    
    
    z = simiic_read_reg(MMA8451_DEV_ADD, OUT_Z_MSB_REG, IIC); 
    wz = ((simiic_read_reg(MMA8451_DEV_ADD, OUT_Z_LSB_REG, IIC))|z<<8);  
    
    //����������ٶ�ԭʼ��Ӧ��ֵ
    if(z>0x7f)  acc_z=-(int16)((~(wz>>2) + 1)&0X3FFF); //��λȡ����һ��ȥ����Ч�ַ� 
    else        acc_z=(wz>>2)&0X3FFF; 	  //��λ����Ч�ַ� 	
    
    

}




    
















