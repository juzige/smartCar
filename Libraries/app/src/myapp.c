#include "myapp.h"

void main_init(void)
{
    gpio_init (B20, GPO,0);     //������

    gpio_init (A17, GPO,1);      //ָʾ��

    gpio_init (C0, GPO,1);      //ָʾ��

    gpio_init (E26, GPO,1);      //ָʾ��

    gpio_init (D15, GPO,1);      //ָʾ��

    gpio_init (E9,  GPI,1);      //���뿪��

    gpio_init (E10,  GPI,1);      //���뿪��

    gpio_init (E7,  GPI,1);      //���뿪��

    gpio_init (E8,  GPI,1);      //���뿪��

    KEY_Init();

    ips114_init();				//IPS114 ��ʼ��

    ips114_clear(0xffff);        //��ȫ��

    ftm_pwm_init(ftm0, ftm_ch2,15*1000,0);           //������ת

    ftm_pwm_init(ftm0, ftm_ch3,15*1000,0);           //������ת

    ftm_pwm_init(ftm0, ftm_ch0,15*1000,0);           //�ҵ����ת

    ftm_pwm_init(ftm0, ftm_ch1,15*1000,0);           //�ҵ����ת

    ftm_quad_init(ftm1);                             //�������

    ftm_quad_init(ftm2);                             //�ұ�����

    camera_init();	                                //����ͷ

    uart_init (uart4, 115200);                     //��ʼ�����������ͨ��

}


