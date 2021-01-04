/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2016,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��179029047
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		SEEKFREE_IPS114_SPI.c
 * @brief      		IPS Һ������
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		Go For It(1325536866)
 * @version    		v1.0
 * @Software 		IAR 8.3 or MDK 5.24
 * @Target core		MK60DN512VLL10
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2019-10-10
 * @note		
					IPSҺ�����߶��壺
					------------------------------------ 
						ģ��ܽ�            ��Ƭ���ܽ�
						SCL                 �鿴SEEKFREE_IPS114_SPI.h�ļ��ڵ�IPS114_SCL		�궨��     Ӳ��SPI���Ų��������л�
						SDA                 �鿴SEEKFREE_IPS114_SPI.h�ļ��ڵ�IPS114_SDA		�궨��     Ӳ��SPI���Ų��������л�
						RES                 �鿴SEEKFREE_IPS114_SPI.h�ļ��ڵ�IPS114_REST_PIN�궨��    
						DC                  �鿴SEEKFREE_IPS114_SPI.h�ļ��ڵ�IPS114_DC_PIN	�궨��  
						CS                  �鿴SEEKFREE_IPS114_SPI.h�ļ��ڵ�IPS114_CS		�궨��     Ӳ��SPI���Ų��������л�
						
						��Դ����
						BL  3.3V��Դ������������ţ�Ҳ���Խ�PWM���������ȣ�
						VCC 3.3V��Դ
						GND ��Դ��
						���ֱ���135*240
					------------------------------------ 

 ********************************************************************************************************************/
 

#include "SEEKFREE_FONT.h"
#include "SEEKFREE_IPS114_SPI.h"


//-------------------------------------------------------------------------------------------------------------------
//  @brief      �ڲ����ã��û��������
//  @param      void 		    
//  @return     				
//  @since      v1.0
//  Sample usage:               
//-------------------------------------------------------------------------------------------------------------------
void ips114_writeIndex(uint8 dat)			//д����
{	
    IPS114_DC(0);
    spi_mosi(spi2,SPI_PCS0,&dat,&dat,1);			

}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      �ڲ����ã��û��������
//  @param      void 		    
//  @return     				
//  @since      v1.0
//  Sample usage:               
//-------------------------------------------------------------------------------------------------------------------
void ips114_writeData(uint8 dat)			//д����
{
    IPS114_DC(1);
    spi_mosi(spi2,SPI_PCS0,&dat,&dat,1);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      �ڲ����ã��û��������
//  @param      void 		    
//  @return     				
//  @since      v1.0
//  Sample usage:               
//-------------------------------------------------------------------------------------------------------------------
void ips114_writedata_16bit(uint16 dat)	//��Һ����дһ��16λ����
{
    uint8 h,l;
    h = dat >> 8;
    l = (uint8)dat;
    
    IPS114_DC(1);
    spi_mosi(spi2,SPI_PCS0,&h,&h,1); 	//д���8λ����
    spi_mosi(spi2,SPI_PCS0,&l,&l,1);	//д���8λ����
}




//-------------------------------------------------------------------------------------------------------------------
//  @brief      1.14�� IPSҺ����ʼ��
//  @return     void
//  @since      v1.0
//  Sample usage:               
//-------------------------------------------------------------------------------------------------------------------
void ips114_init(void)
{
    (void)spi_init(spi2,SPI_PCS0, MASTER,25*1000*1000);//Ӳ��SPI��ʼ��

    gpio_init(IPS114_DC_PIN,GPO,0);
    gpio_init(IPS114_REST_PIN,GPO,0);
//    gpio_init(IPS114_BL_PIN,GPO,1);

    IPS114_REST(0);
    systick_delay_ms(200);
    
    IPS114_REST(1);
    systick_delay_ms(100);
	
    ips114_writeIndex(0x36);
    
    if      (IPS114_DISPLAY_DIR==0)    ips114_writeData(0x00);
    else if (IPS114_DISPLAY_DIR==1)    ips114_writeData(0xC0);
    else if (IPS114_DISPLAY_DIR==2)    ips114_writeData(0x70);
    else                            ips114_writeData(0xA0);
    
    ips114_writeIndex(0x3A); 
    ips114_writeData(0x05);

    ips114_writeIndex(0xB2);
    ips114_writeData(0x0C);
    ips114_writeData(0x0C);
    ips114_writeData(0x00);
    ips114_writeData(0x33);
    ips114_writeData(0x33); 

    ips114_writeIndex(0xB7); 
    ips114_writeData(0x35);  

    ips114_writeIndex(0xBB);
    ips114_writeData(0x37);

    ips114_writeIndex(0xC0);
    ips114_writeData(0x2C);

    ips114_writeIndex(0xC2);
    ips114_writeData(0x01);

    ips114_writeIndex(0xC3);
    ips114_writeData(0x12);   

    ips114_writeIndex(0xC4);
    ips114_writeData(0x20);  

    ips114_writeIndex(0xC6); 
    ips114_writeData(0x0F);    

    ips114_writeIndex(0xD0); 
    ips114_writeData(0xA4);
    ips114_writeData(0xA1);

    ips114_writeIndex(0xE0);
    ips114_writeData(0xD0);
    ips114_writeData(0x04);
    ips114_writeData(0x0D);
    ips114_writeData(0x11);
    ips114_writeData(0x13);
    ips114_writeData(0x2B);
    ips114_writeData(0x3F);
    ips114_writeData(0x54);
    ips114_writeData(0x4C);
    ips114_writeData(0x18);
    ips114_writeData(0x0D);
    ips114_writeData(0x0B);
    ips114_writeData(0x1F);
    ips114_writeData(0x23);

    ips114_writeIndex(0xE1);
    ips114_writeData(0xD0);
    ips114_writeData(0x04);
    ips114_writeData(0x0C);
    ips114_writeData(0x11);
    ips114_writeData(0x13);
    ips114_writeData(0x2C);
    ips114_writeData(0x3F);
    ips114_writeData(0x44);
    ips114_writeData(0x51);
    ips114_writeData(0x2F);
    ips114_writeData(0x1F);
    ips114_writeData(0x1F);
    ips114_writeData(0x20);
    ips114_writeData(0x23);

    ips114_writeIndex(0x21); 

    ips114_writeIndex(0x11); 
    systick_delay_ms(120); 

    ips114_writeIndex(0x29);
    ips114_clear(IPS114_BGCOLOR);
}



void ips114_set_region(uint16 x1,uint16 y1,uint16 x2,uint16 y2)
{
	if(IPS114_DISPLAY_DIR==0)
	{
		ips114_writeIndex(0x2a);//�е�ַ����
		ips114_writedata_16bit(x1+52);
		ips114_writedata_16bit(x2+52);
		ips114_writeIndex(0x2b);//�е�ַ����
		ips114_writedata_16bit(y1+40);
		ips114_writedata_16bit(y2+40);
		ips114_writeIndex(0x2c);//������д
	}
	else if(IPS114_DISPLAY_DIR==1)
	{
		ips114_writeIndex(0x2a);//�е�ַ����
		ips114_writedata_16bit(x1+53);
		ips114_writedata_16bit(x2+53);
		ips114_writeIndex(0x2b);//�е�ַ����
		ips114_writedata_16bit(y1+40);
		ips114_writedata_16bit(y2+40);
		ips114_writeIndex(0x2c);//������д
	}
	else if(IPS114_DISPLAY_DIR==2)
	{
		ips114_writeIndex(0x2a);//�е�ַ����
		ips114_writedata_16bit(x1+40);
		ips114_writedata_16bit(x2+40);
		ips114_writeIndex(0x2b);//�е�ַ����
		ips114_writedata_16bit(y1+53);
		ips114_writedata_16bit(y2+53);
		ips114_writeIndex(0x2c);//������д
	}
	else
	{
		ips114_writeIndex(0x2a);//�е�ַ����
		ips114_writedata_16bit(x1+40);
		ips114_writedata_16bit(x2+40);
		ips114_writeIndex(0x2b);//�е�ַ����
		ips114_writedata_16bit(y1+52);
		ips114_writedata_16bit(y2+52);
		ips114_writeIndex(0x2c);//������д
	}
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      Һ����������
//  @param      color     		��ɫ����
//  @return     void
//  @since      v1.0
//  Sample usage:               ips114_clear(IPS114_YELLOW);// ȫ������Ϊ��ɫ
//-------------------------------------------------------------------------------------------------------------------
void ips114_clear(uint16 color)
{
    uint16 i,j;
	ips114_set_region(0,0,IPS114_X_MAX-1,IPS114_Y_MAX-1);
    for(i=0;i<IPS114_X_MAX;i++)
    {
        for (j=0;j<IPS114_Y_MAX;j++)
	   	{
        	ips114_writedata_16bit(color);	 			 
	    }
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      Һ������
//  @param      x     	        ����x��������
//  @param      y     	        ����y��������
//  @param      dat       	    ��Ҫ��ʾ����ɫ
//  @return     void
//  @since      v1.0
//  Sample usage:               ips114_drawpoint(0,0,IPS114_RED);  //����0,0��һ����ɫ�ĵ�
//-------------------------------------------------------------------------------------------------------------------
void ips114_drawpoint(uint16 x,uint16 y,uint16 color)
{
    ips114_set_region(x,y,x,y);
    ips114_writedata_16bit(color);
}



//-------------------------------------------------------------------------------------------------------------------
//  @brief      Һ����ʾ�ַ�
//  @param      x     	        ����x�������� ������Χ 0 -��IPS114_X_MAX-1��
//  @param      y     	        ����y�������� ������Χ 0 -��IPS114_Y_MAX-1��
//  @param      dat       	    ��Ҫ��ʾ���ַ�
//  @return     void
//  @since      v1.0
//  Sample usage:               ips114_showchar(0,0,'x');//����0,0дһ���ַ�x
//-------------------------------------------------------------------------------------------------------------------
void ips114_showchar(uint16 x,uint16 y,uint8 dat)
{
	uint8 i,j;
	uint8 temp;
    
	for(i=0; i<16; i++)
	{
		ips114_set_region(x,y+i,x+7,y+i);
		temp = tft_ascii[dat-32][i];//��32��Ϊ��ȡģ�Ǵӿո�ʼȡ�� �ո���ascii�������32
		for(j=0; j<8; j++)
		{
			if(temp&0x01)	
            {
                ips114_writedata_16bit(IPS114_PENCOLOR);
            }
			else			ips114_writedata_16bit(IPS114_BGCOLOR);
			temp>>=1;
		}
	}
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      Һ����ʾ�ַ���
//  @param      x     	        ����x��������  ������Χ 0 -��IPS114_X_MAX-1��
//  @param      y     	        ����y��������  ������Χ 0 -��IPS114_Y_MAX/16-1��
//  @param      dat       	    ��Ҫ��ʾ���ַ���
//  @return     void
//  @since      v1.0
//  Sample usage:               ips114_showstr(0,0,"seekfree");
//-------------------------------------------------------------------------------------------------------------------
void ips114_showstr(uint16 x,uint16 y,uint8 dat[])
{
	uint16 j;
	
	j = 0;
	while(dat[j] != '\0')
	{
		ips114_showchar(x+8*j,y*16,dat[j]);
		j++;
	}
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      Һ����ʾ8λ�з���
//  @param      x     	        ����x��������  ������Χ 0 -��IPS114_X_MAX-1��
//  @param      y     	        ����y��������  ������Χ 0 -��IPS114_Y_MAX/16-1��
//  @param      dat       	    ��Ҫ��ʾ�ı�������������int8
//  @return     void
//  @since      v1.0
//  Sample usage:               ips114_showint8(0,0,x);//xΪint8����
//-------------------------------------------------------------------------------------------------------------------
void ips114_showint8(uint16 x,uint16 y,int8 dat)
{
	uint8 a[3];
	uint8 i;
	if(dat<0)
	{
		ips114_showchar(x,y*16,'-');
		dat = -dat;
	}
	else	ips114_showchar(x,y*16,' ');
	
	a[0] = dat/100;
	a[1] = dat/10%10;
	a[2] = dat%10;
	i = 0;
	while(i<3)
	{
		ips114_showchar(x+(8*(i+1)),y*16,'0' + a[i]);
		i++;
	}
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      Һ����ʾ8λ�޷���
//  @param      x     	        ����x�������� ������Χ 0 -��IPS114_X_MAX-1��
//  @param      y     	        ����y�������� ������Χ 0 -��IPS114_Y_MAX/16-1��
//  @param      dat       	    ��Ҫ��ʾ�ı�������������uint8
//  @return     void
//  @since      v1.0
//  Sample usage:               ips114_showuint8(0,0,x);//xΪuint8����
//-------------------------------------------------------------------------------------------------------------------
void ips114_showuint8(uint16 x,uint16 y,uint8 dat)
{
	uint8 a[3];
	uint8 i;
	
	a[0] = dat/100;
	a[1] = dat/10%10;
	a[2] = dat%10;
	i = 0;
	while(i<3)
	{
		ips114_showchar(x+(8*i),y*16,'0' + a[i]);
		i++;
	}
	
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      Һ����ʾ16λ�з���
//  @param      x     	        ����x�������� ������Χ 0 -��IPS114_X_MAX-1��
//  @param      y     	        ����y�������� ������Χ 0 -��IPS114_Y_MAX/16-1��
//  @param      dat       	    ��Ҫ��ʾ�ı�������������int16
//  @return     void
//  @since      v1.0
//  Sample usage:               ips114_showint16(0,0,x);//xΪint16����
//-------------------------------------------------------------------------------------------------------------------
void ips114_showint16(uint16 x,uint16 y,int16 dat)
{
	uint8 a[5];
	uint8 i;
	if(dat<0)
	{
		ips114_showchar(x,y*16,'-');
		dat = -dat;
	}
	else	ips114_showchar(x,y*16,' ');

	a[0] = dat/10000;
	a[1] = dat/1000%10;
	a[2] = dat/100%10;
	a[3] = dat/10%10;
	a[4] = dat%10;
	
	i = 0;
	while(i<5)
	{
		ips114_showchar(x+(8*(i+1)),y*16,'0' + a[i]);
		i++;
	}
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      Һ����ʾ16λ�޷���
//  @param      x     	        ����x�������� ������Χ 0 -��IPS114_X_MAX-1��
//  @param      y     	        ����y�������� ������Χ 0 -��IPS114_Y_MAX/16-1��
//  @param      dat       	    ��Ҫ��ʾ�ı�������������uint16
//  @return     void
//  @since      v1.0
//  Sample usage:               ips114_showuint16(0,0,x);//xΪuint16����
//-------------------------------------------------------------------------------------------------------------------
void ips114_showuint16(uint16 x,uint16 y,uint16 dat)
{
	uint8 a[5];
	uint8 i;

	a[0] = dat/10000;
	a[1] = dat/1000%10;
	a[2] = dat/100%10;
	a[3] = dat/10%10;
	a[4] = dat%10;
	
	i = 0;
	while(i<5)
	{
		ips114_showchar(x+(8*i),y*16,'0' + a[i]);
		i++;
	}
}



//-------------------------------------------------------------------------------------------------------------------
//  @brief      �����(�Ҷ�����ͷ)Һ����ʾ����
//  @param      *p     			ͼ�������ַ
//  @param      width     	    ͼ����
//  @param      height     	    ͼ��߶�
//  @return     void
//  @since      v1.0
//  Sample usage:              
//  @note       ͼ��Ŀ���������Һ���Ŀ�ȣ����Զ�����������ʾ������������ʾȫ��Ұ
//-------------------------------------------------------------------------------------------------------------------
void ips114_displayimage032(uint8 *p, uint16 width, uint16 height) 
{
    uint32 i,j;
                
    uint16 color = 0;
	uint16 temp = 0;
	
    uint16 coord_x = 0;
    uint16 coord_y = 0;
    
    coord_x = width>IPS114_X_MAX?IPS114_X_MAX:width;
    coord_y = height>IPS114_Y_MAX?IPS114_Y_MAX:height;
    ips114_set_region(0,0,coord_x-1,coord_y-1);
    
    for(j=0;j<coord_y;j++)
    {
        for(i=0;i<coord_x;i++)
        {
            temp = *(p+j*width+i*width/coord_x);//��ȡ���ص�
            color=(0x001f&((temp)>>3))<<11;
            color=color|(((0x003f)&((temp)>>2))<<5);
            color=color|(0x001f&((temp)>>3));
            ips114_writedata_16bit(color); 
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      �����(�Ҷ�����ͷ)Һ��������ʾ����
//  @param      *p     			ͼ�������ַ
//  @param      width     	    ͼ����
//  @param      height     	    ͼ��߶�
//  @param      dis_width       ͼ����ʾ���  0 -��IPS114_X_MAX-1��
//  @param      dis_height      ͼ����ʾ�߶�  0 -��IPS114_Y_MAX-1��
//  @return     void
//  @since      v1.0
//  Sample usage:              
//  @note       ͼ��Ŀ���������Һ���Ŀ�ȣ����Զ�����������ʾ������������ʾȫ��Ұ
//-------------------------------------------------------------------------------------------------------------------
void ips114_displayimage032_zoom(uint8 *p, uint16 width, uint16 height, uint16 dis_width, uint16 dis_height)
{
    uint32 i,j;
                
    uint16 color = 0;
	uint16 temp = 0;

    ips114_set_region(0,0,dis_width-1,dis_height-1);//������ʾ���� 
    
    for(j=0;j<dis_height;j++)
    {
        for(i=0;i<dis_width;i++)
        {
            temp = *(p+(j*120/dis_height)*width+i*width/dis_width);//��ȡ���ص�
            color=(0x001f&((temp)>>3))<<11;
            color=color|(((0x003f)&((temp)>>2))<<5);
            color=color|(0x001f&((temp)>>3));
            ips114_writedata_16bit(color); 
        }
    }
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      С���(��ֵ������ͷ)Һ����ʾ����
//  @param      *p     			ͼ�������ַ
//  @param      width     	    ͼ����
//  @param      height     	    ͼ��߶�
//  @return     void
//  @since      v1.0
//  @note      	ͼ��ֱ�����Ҫ������120(��)*160(��)����       
//-------------------------------------------------------------------------------------------------------------------
void ips114_displayimage7725(uint8 *p, uint16 width, uint16 height) 
{
    int i,j; 
	
    uint16 temp = 0;
    ips114_set_region(0,0,width-1,height-1);
	for(i=0;i<height * (width/8);i++)
    {
        temp = *p;
        p++;
        for(j=0; j<8; j++)
        {
            if( (temp<<j)&0x80 )	ips114_writedata_16bit(WHITE);
            else					ips114_writedata_16bit(BLACK);
        }
    }
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      ������ʾ
//  @param      x       ������ 0-��IPS114_X_MAX-1��
//  @param      y       ������ 0-��IPS114_Y_MAX-1��
//  @param      size    ȡģ��ʱ�����õĺ��������С��Ҳ����һ������ռ�õĵ��󳤿�Ϊ���ٸ��㣬ȡģ��ʱ����Ҫ������һ���ġ�
//  @param      *p      ��Ҫ��ʾ�ĺ�������
//  @param      number  ��Ҫ��ʾ����λ
//  @param      color   ��ʾ��ɫ
//  @return     void
//  @since      v1.0
//  Sample usage:		ips114_display_chinese(0,0,16,chinese_test[0],4,RED);//��ʾfont�ļ������ ʾ��
//  @Note       ʹ��PCtoLCD2002���ȡģ		    ���롢����ʽ��˳��   16*16
//-------------------------------------------------------------------------------------------------------------------
void ips114_display_chinese(uint16 x, uint16 y, uint8 size, const uint8 *p, uint8 number, uint16 color)
{
    int i, j, k; 
    uint8 temp, temp1, temp2;
    const uint8 *p_data;
    
    temp2 = size/8;
    
    ips114_set_region(x,y,number*size-1+x,y+size-1);
    
    for(i=0;i<size;i++)
    {
        temp1 = number;
        p_data = p+i*temp2;
        while(temp1--)
        {
            for(k=0;k<temp2;k++)
            {
                for(j=8;j>0;j--)
                {
                    temp = (*p_data>>(j-1)) & 0x01;
                    if(temp)    ips114_writedata_16bit(color);
                    else        ips114_writedata_16bit(IPS114_BGCOLOR);
                }
                p_data++;
            }
            p_data = p_data - temp2 + temp2*size;
        }   
    }
}


