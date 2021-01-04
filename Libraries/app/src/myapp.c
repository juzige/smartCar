#include "myapp.h"

void main_init(void)
{
    gpio_init (B20, GPO,0);     //蜂鸣器

    gpio_init (A17, GPO,1);      //指示灯

    gpio_init (C0, GPO,1);      //指示灯

    gpio_init (E26, GPO,1);      //指示灯

    gpio_init (D15, GPO,1);      //指示灯

    gpio_init (E9,  GPI,1);      //拨码开关

    gpio_init (E10,  GPI,1);      //拨码开关

    gpio_init (E7,  GPI,1);      //拨码开关

    gpio_init (E8,  GPI,1);      //拨码开关

    KEY_Init();

    ips114_init();				//IPS114 初始化

    ips114_clear(0xffff);        //屏全灭

    ftm_pwm_init(ftm0, ftm_ch2,15*1000,0);           //左电机反转

    ftm_pwm_init(ftm0, ftm_ch3,15*1000,0);           //左电机正转

    ftm_pwm_init(ftm0, ftm_ch0,15*1000,0);           //右电机反转

    ftm_pwm_init(ftm0, ftm_ch1,15*1000,0);           //右电机正转

    ftm_quad_init(ftm1);                             //左编码器

    ftm_quad_init(ftm2);                             //右编码器

    camera_init();	                                //摄像头

    uart_init (uart4, 115200);                     //初始换串口与电脑通信

}


