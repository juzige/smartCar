/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2016,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��179029047
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		SEEKFREE_L3G4200D.c
 * @brief      		L3G4200D������
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		Go For It(1325536866)
 * @version    		v1.0
 * @Software 		IAR 7.7 or MDK 5.17
 * @Target core		MK60DN512VLL10
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2016-02-25
 * @note	
					L3G4200D���߶���
					------------------------------------ 
						SDA                 C17
						SCL                 C16
					------------------------------------ 
 ********************************************************************************************************************/



#include "SEEKFREE_L3G4200D.h"

int16 gyro_x = 0, gyro_y = 0, gyro_z = 0;

uint8 Temperature = 0;




//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ʼ��L3G4200D������
//  @param      NULL
//  @return     void						
//  @since      v1.0
//  Sample usage:				���øú���ǰ�����ȵ���ģ��IIC�ĳ�ʼ��
//-------------------------------------------------------------------------------------------------------------------
void InitL3G4200D(void)
{
    simiic_write_reg(L3G4200_DEV_ADD, CTRL_REG1_4200, 0x8f);   //
    simiic_write_reg(L3G4200_DEV_ADD, CTRL_REG2_4200, 0x00);   //00
    simiic_write_reg(L3G4200_DEV_ADD, CTRL_REG3_4200, 0x08);   //
    simiic_write_reg(L3G4200_DEV_ADD, CTRL_REG4_4200, 0x30);   //+-2000dps
    simiic_write_reg(L3G4200_DEV_ADD, CTRL_REG5_4200, 0x00);
    Temperature = simiic_read_reg(L3G4200_DEV_ADD, OUT_TEMP_4200, IIC);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ȡL3G4200D����������
//  @param      NULL
//  @return     void						
//  @since      v1.0
//  Sample usage:				ִ�иú�����ֱ�Ӳ鿴��Ӧ�ı�������
//-------------------------------------------------------------------------------------------------------------------
void get_l3g4200d(void)
{
    uint8 gx, gy, gz;
    uint16 wgx, wgy, wgz;

    if( simiic_read_reg(L3G4200_DEV_ADD, STATUS_REG_4200, IIC)&0x80 )//�鿴�����Ƿ�׼�����ˡ����û��׼���ã���ȡ����ܶ�ȡ���������ݡ���ȡƵ������Ǹ����������Ƶ��
    {
        wgx = simiic_read_reg(L3G4200_DEV_ADD, OUT_X_H_4200, IIC);
		gx = simiic_read_reg(L3G4200_DEV_ADD, OUT_X_L_4200, IIC);
        gyro_x = (wgx<<8) + gx;
        gyro_x >>= 3;

        wgz = simiic_read_reg(L3G4200_DEV_ADD, OUT_Y_H_4200, IIC);
		gz = simiic_read_reg(L3G4200_DEV_ADD, OUT_Y_L_4200, IIC);
        gyro_z = (wgz<<8) + gz;
        gyro_z >>= 3;

        wgy = simiic_read_reg(L3G4200_DEV_ADD, OUT_Z_H_4200, IIC);
		gy = simiic_read_reg(L3G4200_DEV_ADD, OUT_Z_L_4200, IIC);
        gyro_y = (wgy<<8) + gy;
        gyro_y >>= 3;
    }
}
