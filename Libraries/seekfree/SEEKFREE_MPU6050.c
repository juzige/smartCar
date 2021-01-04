/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2016,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��179029047
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		SEEKFREE_MPU6050.c
 * @brief      		MPU6050������
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		Go For It(1325536866)
 * @version    		v1.0
 * @Software 		IAR 7.7 or MDK 5.17
 * @Target core		MK60DN512VLL10
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2016-02-25
 * @note	
					MPU6050���߶���
					------------------------------------ 
						SDA                 C17
						SCL                 C16
					------------------------------------ 
 ********************************************************************************************************************/



#include "SEEKFREE_MPU6050.h"

int16 mpu_gyro_x,mpu_gyro_y,mpu_gyro_z;
int16 mpu_acc_x,mpu_acc_y,mpu_acc_z;



//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ʼ��MPU6050
//  @param      NULL
//  @return     void					
//  @since      v1.0
//  Sample usage:				���øú���ǰ�����ȵ���ģ��IIC�ĳ�ʼ��
//-------------------------------------------------------------------------------------------------------------------
void InitMPU6050(void)
{
    simiic_write_reg(MPU6050_DEV_ADDR, PWR_MGMT_1, 0x00);	   //�������״̬
    simiic_write_reg(MPU6050_DEV_ADDR, SMPLRT_DIV, 0x07);      //125HZ������
    simiic_write_reg(MPU6050_DEV_ADDR, CONFIG, 0x04);          //
    simiic_write_reg(MPU6050_DEV_ADDR, GYRO_CONFIG, 0x18);     //2000
    simiic_write_reg(MPU6050_DEV_ADDR, ACCEL_CONFIG, 0x10);    //8g
	simiic_write_reg(MPU6050_DEV_ADDR, User_Control, 0x00);
    simiic_write_reg(MPU6050_DEV_ADDR, INT_PIN_CFG, 0x02);
}




//�ڲ�ʹ���û��������
int16 GetData(uint8 REG_Address)
{
    uint8 L;   uint16 H ;
    H=simiic_read_reg(MPU6050_DEV_ADDR, REG_Address, IIC);
    L=simiic_read_reg(MPU6050_DEV_ADDR, REG_Address+1, IIC);
    return (H<<8)+L;   //�ϳ�����
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ȡMPU6050���ٶȼ�����
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:				ִ�иú�����ֱ�Ӳ鿴��Ӧ�ı�������
//-------------------------------------------------------------------------------------------------------------------
void Get_AccData(void)
{
    mpu_acc_x = GetData(ACCEL_XOUT_H)>>2;
    mpu_acc_y = GetData(ACCEL_YOUT_H)>>2;
    mpu_acc_z = GetData(ACCEL_ZOUT_H)>>2;
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ȡMPU6050����������
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:				ִ�иú�����ֱ�Ӳ鿴��Ӧ�ı�������
//-------------------------------------------------------------------------------------------------------------------
void Get_Gyro(void)
{
    mpu_gyro_z = GetData(GYRO_XOUT_H)>>3;
    mpu_gyro_y = GetData(GYRO_YOUT_H)>>3;
    mpu_gyro_x = GetData(GYRO_ZOUT_H)>>3;
}


