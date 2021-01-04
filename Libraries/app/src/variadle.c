 /*========================上位机设置=============================*/
/*     可用蓝牙无线通讯和51下载器有线通讯                           */

#include "variadle.h"

//uint8 temp;
float var_data[20] = {0.0};

void my_putchar(char temp)
{
    uart_putchar(uart4,temp); //根据实际的串口号来修改
}

/*用来通知上位机新的一组数据开始，要保存数据必须发送它*/

void Send_Begin()
{
	my_putchar(0x55);
	my_putchar(0xaa);
	my_putchar(0xae);
}

//stc使用大端模式
//nxp使用小端模式
void Send_Variable()
{
	uint8 i=0,ch=0;
	float temp=0;
	uint8 variable_num=6;
	my_putchar(0x55);
	my_putchar(0xaa);
	my_putchar(0xad);
	my_putchar(variable_num);

	for(i=0;i<variable_num;i++)
	{
		temp=var_data[i];
		ch=BYTE0(temp);
		my_putchar(ch);
		ch=BYTE1(temp);
		my_putchar(ch);
		ch=BYTE2(temp);
		my_putchar(ch);
		ch=BYTE3(temp);
		my_putchar(ch);
	}
	my_putchar(0x0d);
}
