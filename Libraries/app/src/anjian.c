#include "anjian.h"

void KEY_Init(void)
{
    //按键初始化
    gpio_init(E3,GPI,1);      //flash
    gpio_init(E2,GPI,1);      //up
    gpio_init(E5,GPI,1);      //down
    gpio_init(E4,GPI,1);      //true
    gpio_init(E6,GPI,1);      //cancel

}


uint8 KEY_Read(uint8 mode)
{
    uint8 key_up=1;
    if(mode_flag==1)
    {
        key_up=1;      //
    }
    if(key_up && (gpio_get(FLASH)==0 || gpio_get(UP)==0 || gpio_get(DOWN)==0 || gpio_get(TRUE)==0 || gpio_get(CANCEL)==0))
    {
        systick_delay_ms(10);   //按键延时
        key_up=0;
        if(gpio_get(FLASH)==0)
        {
            return 1;
        }

        else if(gpio_get(UP)==0)
        {
            return 2;
        }

        else if(gpio_get(DOWN)==0)
        {
            return 3;
        }
        else if(gpio_get(TRUE)==0)
        {
            return 4;
        }
        else if(gpio_get(CANCEL)==0)
        {
            return 5;
        }

    }
    if(key_up && gpio_get(FLASH) && gpio_get(UP) && gpio_get(DOWN) && gpio_get(TRUE) && gpio_get(CANCEL))
    {
        key_up=1;
    }
    return 0;   //无事件返回0

}