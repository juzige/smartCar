#include "adjparameter.h"


/*�ٶȱ���*/
int8 i_speed;
int16 l_speed = 0, r_speed = 0;
int16 l_i_speed=0;
int16 r_i_speed=0;
int16 error, d_error, p_error;      //PID����
int32 u_k=0;                                            //��ֵ����
int16 c_errory,errory, d_errory, p_errory, error1y, d_error1y;      //PID����
int32 u_ky=0;                                            //��ֵ����
uint16 speed_k=0;


/*PD�㷨*/
int16  turn_D;
int16  turn_p;
uint8  speed_flag;
uint16 P,I,D;



/*��������*/
uint8 is_LOCKED = 0, is_CHOOSE = 0, select = 1;
uint8 KEY_U_flag = 0,KEY_D_flag = 0,KEY_DEFINITE_flag = 0,KEY_CANCEL_flag = 0,KEY_FLASH_flag = 0;
uint8 variable = 0;
uint8 a = 0, b = 0;
uint8 ch[2] = "-";

//���뿪�ز���
extern uint8 mode_flag=0;
uint8 mode_reset=0;
uint8 Set_Pid = 0;

//pid����
uint8  speed_1 = 0, speed_2 = 0, speed_3 = 0;
uint8 speed_P = 0, speed_I = 0, turn_P = 0, turn_DD = 0;
uint8 angle_P = 0.0, angle_D = 0.0, temp_2 = 0, temp_3 = 0;

//����ʱ��
uint8 debug_times = 0;

uint8 var[32] = {0};
uint8 car_qidong = 0;
//״̬
uint8 status;

typedef enum
{
    KEY_EEPEOM = 1,  //����eeprom

    KEY_U,  //��
    KEY_D,  //��

    KEY_DEFINITE,  //ȷ��
    KEY_CANCEL,  //ȡ��

}KEY; //����ö��


void Flash_Read(uint8 k)      //�ж϶�ȡflash
{
    FLASH_Init();
    mode_reset = gpio_get(E9);                //flash����
    Set_Pid = gpio_get(E7);                  //��λ
    if(mode_reset == 1) //����flash����ȡ���뿪��
    {
        switch(Set_Pid)   //Ĭ�ϱ��� 1Ϊ�ٶ� 2Ϊturn_P 3Ϊ��ֵ
        {
        case 0:  var[1]=10,var[2]=10,var[3]=75,var[4]=75,     //�ٶȡ�ת���⻷P��ת���⻷D��������DD

        var[5]=100,var[6]=60,var[7]=110,var[8]=100,  //�ٶ��ڻ�P��I�������޷�*100

        var[9]=20,var[10]=3,var[11]=0,var[12]=55,  //���ٶȻ�P��D

        var[13]=0,var[14]=4,var[15]=5,var[16]=3,    //�ٶȡ�ת���⻷P��ת���⻷D��������DD

        var[17]=4,var[18]=4,var[19]=5,var[20]=3,    //�ٶ��ڻ�P��I�������޷�*100

        var[21]=5,var[22]=25,var[23]=10,var[24]=5,  //���ٶȻ�P��D

        var[25]=4,var[26]=0,var[27]=9,var[28]=6,    //���ٶȻ�P��D

        var[29]=0x50;                               //����ͷ��ֵ
        }


       FLASH_EraseSector(123);               //256��������ǰ����������ڴ洢���򣬲��ɲ�����������Сһ������2k����С��λ1�ֽ�//����

       for(uint8 i = 1; i <= k; i++)
       {
            FLASH_WriteSector(123, &var[i],1,i*8);
       }


    }

    else //�������ã����Բ��뿪�أ���ȡflash
    {
        for(uint8 i=1;i<=k;i++)
        {
            var[i]=flash_read(123,i*8,uint8);
        }

    }
}
//PID������ֵ
static void Set_Pid_Date(void)
{
	 /*************��һҳ**************///����������ת���⻷
	speed_k=var[1];speed_1=var[2];
	speed_2=var[3];speed_3=var[4];

	/*************�ڶ�ҳ**************///�ٶ��ڻ�����
	speed_P=var[5];speed_I=var[6];
	turn_P=var[7];turn_DD=var[8];

	/*************����ҳ**************///���ٶȻ�����
	angle_P=var[9] ;angle_D=var[10];
	temp_2=var[11];temp_3=var[12];
}
static void setPidDate(void)
{
    speed = var[1];                //41
    lpid.Kp = rpid.Kp = var[2];      //p : 16  i: 1
    lpid.Ki = rpid.Ki = var[3];
    lpid.Kd = rpid.Kd = var[4];

    tpid.Kp = var[5];    //p:6 d:1
    tpid.Ki = var[6];
    tpid.Kd = var[7];

    debug_times = var[8];

}

//ģʽ��ȡ
void Mode_Read(void)
{
	mode_flag=gpio_get (E10)+gpio_get (E8)*2;
	//Set_Pid_Date();
    setPidDate();
	if(mode_flag==2)
	{
		car_qidong=1;                                      //����pid ȫ�ּ���
	}
}
//��������
void parameter_adj_bykey()
{

	/*********************��������*************************/
	if(!is_LOCKED)
	{
		if(KEY_Read(0) == KEY_D && !KEY_D_flag)
		{
			buzzer(30);
			KEY_D_flag = 1;
			if(is_CHOOSE) var[select]++;
			else
			{
				if(++select > PARANUM) select = 1;
			}
		}
		else if (KEY_Read(0) == 0 && KEY_D_flag) KEY_D_flag = 0;

		else if(KEY_Read(0) == KEY_U && !KEY_U_flag)
		{
			buzzer(30);
			KEY_U_flag = 1;
			if(is_CHOOSE) var[select]--;
			else
			{
				if(--select < 1) select = PARANUM;
			}
		}
		else if (KEY_Read(0) == 0 && KEY_U_flag) KEY_U_flag = 0;
	}

	/********************����ȷ��*************************/
	if(KEY_Read(0) == KEY_DEFINITE && !KEY_DEFINITE_flag)     buzzer(30),KEY_DEFINITE_flag = 1,is_CHOOSE = 1, ch[0] = '~';
	else if(KEY_Read(0) == 0 && KEY_DEFINITE_flag)            KEY_DEFINITE_flag = 0;
	else if (KEY_Read(0) == KEY_CANCEL && !KEY_CANCEL_flag)   buzzer(30),KEY_CANCEL_flag = 1,is_CHOOSE = 0, ch[0] = '-';
	else if(KEY_Read(0) == 0 && KEY_CANCEL_flag)              KEY_CANCEL_flag = 0;

	/********************����flash***********************/
	if(KEY_Read(0) == KEY_EEPEOM && !KEY_FLASH_flag)
	{
		buzzer(30);
		KEY_FLASH_flag = 1;
		is_LOCKED = 1;
        FLASH_EraseSector(123);
       for(uint8 i = 1; i <= 20; i++)
       {
            FLASH_WriteSector(123, &var[i],1,i*8);
       }
	}
	else if(KEY_Read(0) == 0 && KEY_FLASH_flag) KEY_FLASH_flag = 0, is_LOCKED = 0;
#if 1
	/********************����ѡ��λ��*******************/
	ips114_showint16(90,0,select);
	ips114_showstr(b,a," ");
	if((select+4)%4==1||(select+4)%4==2)a=1;         else a=4;//1234
	if((select+2)%2==0)b=60;                         else b=0; //1324

/*********************��һҳ********************************/  //���������ٶȡ�ת��
	if(select<5)
	{
	ips114_showstr(b,a,ch);
	ips114_showstr(10,1,"speed ");ips114_showint16(10,3,var[1]);
	ips114_showstr(70,1,"speed1");ips114_showint16(70,3,var[2]);
	ips114_showstr(10,4,"speed2");ips114_showint16(10,6,var[3]);
	ips114_showstr(70,4,"speed3");ips114_showint16(70,6,var[4]);
	}
/*********************�ڶ�ҳ********************************/  //�ٶȻ�PID
	else if(select<9)
	{
	ips114_showstr(b,a,ch);
	ips114_showstr(10,1,"spee_P");ips114_showint16(10,3,var[5]);
	ips114_showstr(70,1,"spee_I");ips114_showint16(70,3,var[6]);
	ips114_showstr(10,4,"turn_P");ips114_showint16(10,6,var[7]);
	ips114_showstr(70,4,"turndd");ips114_showint16(70,6,var[8]);
	}

/*********************����ҳ********************************/  //���ٶȻ�PID
	else if(select<13)
	{
	ips114_showstr(b,a,ch);
	ips114_showstr(10,1,"angl_P");ips114_showint16(10,3,var[9] );
	ips114_showstr(70,1,"angl_D");ips114_showint16(70,3,var[10]);
	ips114_showstr(10,4,"temp_2");ips114_showint16(10,6,var[11]);
	ips114_showstr(70,4,"temp_3");ips114_showint16(70,6,var[12] );
	}
/*********************����ҳ********************************/ //Բ��
	else if(select<17)
	{
	ips114_showstr(b,a,ch);
	ips114_showstr(10,1,"L_FIRS");ips114_showint16(10,3,var[13] );
	ips114_showstr(70,1,"L_SECO");ips114_showint16(70,3,var[14]);
	ips114_showstr(10,4,"L_THIR");ips114_showint16(10,6,var[15]);
	ips114_showstr(70,4,"L_FOUR");ips114_showint16(70,6,var[16] );
	}
/*********************����ҳ********************************/
	else if(select<21)
	{
	ips114_showstr(b,a,ch);
	ips114_showstr(10,1,"R_FIRS");ips114_showint16(10,3,var[17] );
	ips114_showstr(70,1,"R_SECO");ips114_showint16(70,3,var[18]);
	ips114_showstr(10,4,"R_THIR");ips114_showint16(10,6,var[19]);
	ips114_showstr(70,4,"R_FOUR");ips114_showint16(70,6,var[20] );
	}
/*********************����ҳ********************************/
	else if(select<25)
	{
	ips114_showstr(b,a,ch);
	ips114_showstr(10,1,"BROK_1");ips114_showint16(10,3,var[21]);
	ips114_showstr(70,1,"BROK_2");ips114_showint16(70,3,var[22]);
	ips114_showstr(10,4,"BROK_3");ips114_showint16(10,6,var[23]);
	ips114_showstr(70,4,"BROK_4");ips114_showint16(70,6,var[24]);
	}
	/********************�ڰ�ҳ********************************/


#endif

}

void buzzer(uint8 x)                    //��ʱ=x*2ms
{
	gpio_set(B20,1);          //������Ҫ���øߵ�ƽ
	buzzer_flag = x;
}

void debug_mode()
{
    //if(!debug_flag) return;




}



