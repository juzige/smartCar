/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2018,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：一群：179029047(已满)  二群：244861897
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		main
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ3184284598)
 * @version    		查看doc内version文件 版本说明
 * @Software 		IAR 8.3 or MDK 5.24
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2019-10-28
 * @note
 ********************************************************************************************************************/

#include "headfile.h"

uint8  buzzer_flag;         //蜂鸣器标记位
uint8 tingche_flag;
int32 speed_control_count=0;
uint32 time_count = 0;
uint16  bianma_count=0;
uint8 debug_flag = 0;
uint8 temp = 0;

int main(void)
{
    get_clk();//上电后必须运行一次这个函数，获取各个频率信息，便于后面各个模块的参数设置

    main_init();

    Flash_Read(20);                         //flash重置赋初值

    Mode_Read();

    pit_init_ms(pit0,2);                             //定时 2ms 后中断

    enable_irq(PIT0_IRQn);                           //使能PIT中断hc

    buzzer(100);

    while(1)
    {
        if(mode_flag==1)         //模式1 按键调参
        {
            parameter_adj_bykey();
        }

        else if(mode_flag == 3)    //模式2 显示摄像头处理图像
        {
           if(mt9v032_finish_flag)
           {
                mt9v032_finish_flag = 0;
                img_threshold = my_adapt_threshold(image[0], COL, ROW);
                ips114_show_cam();
                eage_search();
                //Cal_dis2Straight();
                ips_show_middleline();
           }

        }
        else if(mode_flag == 0)         //模式0 传输图像
        {
            //ips114_showstr(40, 0, "UARTING...");

           if(mt9v032_finish_flag)
           {
                mt9v032_finish_flag = 0;
                ips114_displayimage032(image[0], COL, ROW);
                //seekfree_sendimg_032();
           }
        }

        else if(mode_flag == 2)       //启动全局计数
        {
            debug_flag = 1;
            ips114_showint16(0,1,l_speed);
            ips114_showint16(90,1,r_speed);


            ips114_showint16(0,2,L_Duty);
            ips114_showint16(90,2,R_Duty);

            ips114_showint16(0,4,Centerline_diff);

            if(debug_times == 0) speed = 0;

            //CarControl();

            pit_time_start(pit1) ;

            if(mt9v032_finish_flag)
            {
                img_threshold = my_adapt_threshold(image[0], COL, ROW);
                eage_search();
            }
            ips114_showint16(0,6,pit_time_get(pit1));
            pit_close(pit1);


#if 0
            float data[3];
            uint8 tail[4] = {0x00, 0x00, 0x80, 0x7f};
            data[0] = l_speed;
            data[1] = r_speed;
            data[2] = var[1];
//            data[3] = 3;
//            data[4] = l_speed;
//            data[5] = 41;
            uart_putbuff(uart4,(uint8 *)data,12);
            uart_putbuff(uart4,tail,4);

#endif

        }
    }
}

void PIT0_IRQHandler(void)      //中断函数
{

    PIT_FlAG_CLR(pit0);
    //只用一个通道时，关闭PIT，稳定，清超时标志位
    //bianma_count++;
    if(debug_times && (++bianma_count) == 500)
    {
        bianma_count = 0;
        debug_times--;
    }

    if(buzzer_flag>0)         //蜂鸣器响
    {
        buzzer_flag--;
        if(buzzer_flag==0)
            gpio_set (B20, 0);   //高电平响，这里需要设置成低电平
    }
    speed_control_count++;
    if(speed_control_count>5000)  speed_control_count=5000;    //防止溢出


    time_count++;

    if(time_count>65532)time_count=65533;    //防止溢出

    /*编码器*/
    l_speed=-ftm_quad_get(ftm2);          //2ms测一次

    ftm_quad_clean  (ftm2);

    r_speed=ftm_quad_get(ftm1);          //2ms测一次

    ftm_quad_clean  (ftm1);

    CarControl();

}



