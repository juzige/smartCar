#include "camara.h"
#include "math.h"

#define LINE_GOOD    0X00u
#define LEFT_LOST_W  0x01u	//white
#define LEFT_LOST_B  0x02u   //black
#define RIGHT_LOST_W 0x04u
#define RIGHT_LOST_B 0x08u
#define ALL_LOST_W   0x10u
#define ALL_LOST_B   0x20u

uint8 img_threshold;
void ips114_show_cam(void)   //��ʾ�Ҷ�ͼ��
{
    uint8 i,j,k;
    uint8 black = 0xff, white = 0x00;

    ips114_writeIndex(0x2a);//�е�ַ����
    ips114_writedata_16bit(0+40);
    ips114_writedata_16bit(187+40);
    ips114_writeIndex(0x2b);//�е�ַ����
    ips114_writedata_16bit(0+52);
    ips114_writedata_16bit(119+52);
    ips114_writeIndex(0x2c);//������д

    IPS114_DC(1);

    for(i = 0; i < 120; i++)
    {
        for(j = 0; j < 188; j++)
        {


            if(image[i][j] > img_threshold)
            {
                spi_mosi(spi2,SPI_PCS0,&black,NULL,1); //д���8λ����
                spi_mosi(spi2,SPI_PCS0,&black,NULL,1);	//д���8λ����
            }

            else
            {
                spi_mosi(spi2,SPI_PCS0,&white,NULL,1); //д���8λ����
                spi_mosi(spi2,SPI_PCS0,&white,NULL,1);	//д���8λ����
            }
        }
    }
}

uint8 my_adapt_threshold(uint8 *image, uint16 col, uint16 row)   //ע�������ֵ��һ��Ҫ��ԭͼ��
{
#define GrayScale 256
    uint16 width = col;
    uint16 height = row;
    int pixelCount[GrayScale];
    float pixelPro[GrayScale];
    int i, j, pixelSum = width * height/2/2;        //����4������
    uint8 threshold = 0;
    uint8* data = image;  //ָ���������ݵ�ָ��
    for (i = 0; i < GrayScale; i++)
    {
        pixelCount[i] = 0;
        pixelPro[i] = 0;
    }

    uint32 gray_sum=0;
    //ͳ�ƻҶȼ���ÿ������������ͼ���еĸ���
    for (i = 0; i < height; i+=2)      //��������ֵ���Ƶģ�ÿ��+2
    {
        for (j = 0; j < width; j+=2)   //��������ֵ���Ƶģ�ÿ��+2
        {
            pixelCount[(int)data[i * width + j]]++;  //����ǰ�ĵ������ֵ��Ϊ����������±�
            gray_sum+=(int)data[i * width + j];       //�Ҷ�ֵ�ܺ�
        }
    }

    //����ÿ������ֵ�ĵ�������ͼ���еı���

    for (i = 0; i < GrayScale; i++)
    {
        pixelPro[i] = (float)pixelCount[i] / pixelSum;

    }

    //�����Ҷȼ�[0,255]
    float w0, w1, u0tmp, u1tmp, u0, u1, u, deltaTmp, deltaMax = 0.0;



    w0 = w1 = u0tmp = u1tmp = u0 = u1 = u = deltaTmp = 0;
    for (j = 0; j < GrayScale; j++)
    {

        w0 += pixelPro[j];  //��������ÿ���Ҷ�ֵ�����ص���ռ����֮��   ���������ֵı���
        u0tmp += j * pixelPro[j];  //�������� ÿ���Ҷ�ֵ�ĵ�ı��� *�Ҷ�ֵ

        w1=1-w0;
        u1tmp=gray_sum/pixelSum-u0tmp;

        u0 = u0tmp / w0;              //����ƽ���Ҷ�
        u1 = u1tmp / w1;              //ǰ��ƽ���Ҷ�
        u = u0tmp + u1tmp;            //ȫ��ƽ���Ҷ�
        deltaTmp = w0 * pow((u0 - u), 2) + w1 * pow((u1 - u), 2);
        if (deltaTmp > deltaMax)
        {
            deltaMax = deltaTmp;
            threshold = j;
        }
        if (deltaTmp < deltaMax)
        {
            break;
        }

    }
    return threshold;

}

void search_whiteline()
{
    int8 i = 0;
    uint8 j = 0, k = 0;
    uint8 Startingpoint = 93;



    if(image[79][187] < img_threshold)
    {
        //Startingpoint = 187;
         for(i = 79, k = 0; i > 0; i-=2, k++)
         {
            for(j = Startingpoint; j > 2; j--)
            {
                if(image[i][j]    > img_threshold &&
                  image[i][j - 1] > img_threshold &&
                  image[i][j - 2] > img_threshold &&
                 image[i][j - 3] > img_threshold )
                {
                   Model_line[k] = j;
                   break;
                }
            }

            Startingpoint =  Model_line[k] + 4;
         }

         return;

    }
    else if(image[79][187] > img_threshold)
    {
       // buzzer(20);
        for(i = 79, k =0; i > 0; i-=2, k++)
        {
            for(j = Startingpoint; j < 185; j++)
            {
                if(image[i][j]     > img_threshold &&
                   image[i][j + 1] > img_threshold &&
                   image[i][j + 2] > img_threshold &&
                   image[i][j + 3] > img_threshold)
                {
                    Model_line[k] = j;
                    break;
                }
            }

            Startingpoint =  Model_line[k] - 4;
        }
        return;
    }
}


#define MUTATIONLIMIT      10     //ͻ���в�ֵ����
uint8 left_line[40];        //�洢���ܵ��߽磬��ͼ��ɫ�ߡ�
uint8 Right_line[40];       //�洢���ܵ��߽磬��ͼ��ɫ�ߡ�
uint8 Model_line[40];       //�洢�м��ܵ���Ϣ����ͼ��ɫ�ߡ�
uint8 left_line_flag[40];   //�洢�Ƿ�ɨ�赽��߽���Ϣ
uint8 right_line_flag[40];  //�洢�Ƿ�ɨ�赽�ұ߽���Ϣ
uint8 middleline = 93;
uint8 lost_w_count,lost_b_count,hang_flag[40];      //black, white
uint8 err_num = 25;
uint8 Mutationline = 0;   //ͻ����
uint8 search_finished = 0;

void eage_search(void)    //0-79��������Ϣ��ȡ
{
    int16 j = 0;
    int8 i = 0;
    uint8 k = 0;
    uint8 left_flag  = 0;
    uint8 right_flag = 0;
    lost_b_count = lost_w_count = 0;
    //Mutationline = 0;
    middleline = 93;
    memset(left_line_flag, 0, sizeof(uint8) * 40);
    memset(right_line_flag, 0, sizeof(uint8) * 40);
    /*********************��һ��ɨ��***********************/
    for(j = middleline; j < 184; j++)  //���Һڱ�
    {
        if( image[79][j]    < img_threshold &&
           image[79][j + 1] < img_threshold &&
           image[79][j + 2] < img_threshold )
        {
            right_flag = 1;
            Right_line[0] = j;
            break;
        }

    }

    if(right_flag == 0)
    {
        Right_line[0] = 183;     //����

    }

    for(j = middleline; j > 3; j--)  //���Һڱ�
    {
        if( image[79][j]    < img_threshold &&
           image[79][j - 1] < img_threshold &&
           image[79][j - 2] < img_threshold )
        {
            left_flag = 1;
            left_line[0] = j;
            break;
        }

    }
    if(left_flag == 0)
    {
        left_line[0] = 4;   //����
    }

    if(left_flag == 0 || right_flag == 0) lost_b_count++;                         //���ڱ�
    if(left_line[0] == middleline && Right_line[0] == middleline) lost_w_count++;  // ���ױ�

    right_flag = left_flag = 0;  //��������Ѱ�ұ���ֵ��־λ

    Model_line[0] = (left_line[0] + Right_line[0]) >> 1;

    middleline = Model_line[0];   //��������ֵ

    /********************�ǵ�һ��ɨ��****************/
    for(i = 77, k = 1; i > 0; i-=2, k++)       //40 - 80
    {
        for(j = middleline; j < 184; j++)  //���Һڱ�
        {
            if( image[i][j]  < img_threshold &&
             image[i][j + 1] < img_threshold &&
             image[i][j + 2] < img_threshold )
            {
                right_flag = 1;
                Right_line[k] = j;
                break;
            }

        }

        if(right_flag == 0)
        {
            Right_line[k] = 183;     //����
        }

        for(j = middleline; j > 3; j--)  //���Һڱ�
        {
            if( image[i][j]  < img_threshold &&
             image[i][j - 1] < img_threshold &&
             image[i][j - 2] < img_threshold )
            {
                left_flag = 1;
                left_line[k] = j;
                break;
            }

        }
        if(left_flag == 0)
        {
            left_line[k] = 4;   //����
        }

        if(left_flag == 0 || right_flag == 0) lost_b_count++;                         //���ڱ�
        if(left_line[k] == middleline && Right_line[k] == middleline) lost_w_count++;  // ���ױ�

        if(lost_w_count > 25 &&
           Model_line[0] == Model_line[1] &&
           Model_line[1] == Model_line[2] &&
           Model_line[2] == Model_line[3] &&
           Model_line[3] == Model_line[4])
        {
           goto LOOP;
        }


        right_flag = left_flag = 0;

//        if(abs(left_line[k] - left_line[k-1]) > err_num)
//            left_line[k] =  left_line[k-1];
//
//        if(abs(Right_line[k] - Right_line[k-1]) > err_num)
//            Right_line[k] =  Right_line[k-1];


        Model_line[k] = (left_line[k] + Right_line[k]) >> 1;

        if ( abs(Model_line[k] -  Model_line[k - 1]) > err_num)      //����ɨ��������ͻ�䣬��ά���ϸ�����
            Model_line[k] =  Model_line[k - 1];

//        if ( abs(Model_line[k] -  Model_line[k - 1] > MUTATIONLIMIT) )       //��¼ͻ����
//            Mutationline =  i + 2;

        middleline = Model_line[k];   //��������ֵ

    }
    search_finished = 1;   //�������ؽ���
    if(search_finished == 1)  return;
LOOP: search_whiteline();
    search_finished = 1;

}

/**************��С���˷�ֱ�����*******************/
/***  y = a*x + b
****  a = Para[0][2]
***   b = Para[1][2]
***/
static float Para[2][3] = {0};
static int ParaInit(uint8* X, uint8 Y, int8 Amount)    //������ʼ��
{
    memset(Para, 0, sizeof(Para));
    Para[1][1] = Amount;
    for ( ; Amount; Amount--, X++, Y+=2)
    {
        Para[0][0] += pow(*X, 2);
        Para[0][1] += (*X);
        Para[0][2] += (*X) * (Y);
        Para[1][2] += (Y);
    }
    Para[1][0] = Para[0][1];
    return 0;
}
static int ParaDeal(void)
{
        Para[0][0] -= Para[1][0] * (Para[0][1] / Para[1][1]);
        Para[0][2] -= Para[1][2] * (Para[0][1] / Para[1][1]);
        Para[0][1] = 0;
        Para[1][2] -= Para[0][2] * (Para[1][0] / Para[0][0]);
        Para[1][0] = 0;
        Para[0][2] /= Para[0][0];
        Para[1][2] /= Para[1][1];
        return 0;
}

uint16 left_sumofSQ = 0;
uint16 right_sumofSQ = 0;
uint16 total_sumofSQ = 0;
uint16 mid_sumofSQ = 0;
int16 Cal_dis2Straight(void)      //����ͼ���Զ���ܵ��ĵ㵽ֱ�߾����ƽ����
{
    uint8 i,j = 0;
    int16 err = 0;
    ParaInit(&Model_line[0], 0, 5);
    ParaDeal();
    mid_sumofSQ = 0;
    for(i = 80-Mutationline-1,j = 0; j < 3; i+=2, j++)
    {
        err = Para[0][2] * Model_line[i >> 1]+ Para[1][2];
        mid_sumofSQ += pow(err - i, 2);
    }

    mid_sumofSQ = mid_sumofSQ / 3;

//    total_sumofSQ = (right_sumofSQ + left_sumofSQ) / 3;

    return total_sumofSQ;

}
#define LIMITERR 10
uint8 Mutationline_num = 0;
void Mutationline_search()
{
     Mutationline_num = 0;
     for(int8 i = 39; i > -1; i--)
     {

     }
}

void ips_show_middleline(void)  //ips��ʾ����
{
    for(int8 i = 79, j = 0; i > -1; i-=2, j++)
    {

        ips114_drawpoint(Model_line[j], i, 0x001F);  //��ɫ �м���
        ips114_drawpoint( left_line[j], i, 0xF800); //��ɫ   �����
        ips114_drawpoint(Right_line[j], i, 0xFFE0); //��ɫ   �ұ���

//        ips114_set_region(Model_line[i],i,Model_line[i],i);   //��ɫ �м���
//        ips114_writedata_16bit(0x001f);
//
//        ips114_set_region(left_line[i],i,left_line[i],i);    //��ɫ �����
//        ips114_writedata_16bit(0xf800);
//
//        ips114_set_region(Right_line[i],i,Right_line[i],i);   //��ɫ �ұ���
//        ips114_writedata_16bit(0xffe0);

    }

    for(uint8 i  = 80; i > 60; i--)
    {
        //ips114_drawpoint(94, i, 0xF800);
    }
//
//    ips114_showuint16(188, 7, lost_b_count);
//    ips114_showuint16(188, 5, lost_w_count);
//    ips114_showint16(188, 1, Centerline_diff);      //Mutationline
//    ips114_showint16(188, 0, turn);
//    ips114_showint16(188, 3, temp1);

}


#if 0
uint8 oled_valid_line;
void earge_search(void)
{
    int i,j;
    int right_start,right_end,last_right;
    int left_start,left_end,last_left;
    lost_w_count=lost_b_count=0;
    valid_line=0;
    for(i=63;i>=0;i--)
    {

        if(i>55)
        {
            for(j=middle_start;j>=0;j--)
            {
                if(j>0)  //�ҵ�1Ϊ�� 2Ϊ�׵�ʱ��
                {
                    if( get_img(i,j-1)==0x00&&get_img(i,j)==0x00&&get_img(i,j+1)==0xFF )    //1Ϊ��0Ϊ��  ���¼1
                    {
                        Left_Board[i]=j+1;
                        hang_flag[i]=0;
                        break;
                    }
                }
                else if (j==0)  //�ҵ�1��ʱ������1��2��  ѭ��û��������д��ж�
                {
                    Left_Board[i]=0;
                    hang_flag[i] |= LEFT_LOST_W;  //���ڱ�
                }
            }

            for(j=middle_start;j<=187;j++)//���м����ұ�ɨ
            {
                if(j<187)
                {
                    if (get_img(i,j-1)==0xFF&& get_img(i,j)==0x00&& get_img(i,j+1)==0x00)
                    {
                        Right_Board[i] = j-1;
                        hang_flag[i]=0;
                        break;
                    }
                }
                if(j==187)
                {
                    Right_Board[i] = 187; //�ұ߶�����
                    hang_flag[i] |= RIGHT_LOST_W;
                }
            }

        }                   //����Ѱ�߽���

        else               //Զ������Ѱ�߿�ʼ
        {

            if(i==55)
            {
                last_right=Right_Board[56];
                last_left=Left_Board[56];
            }
            /******�ұ�******/
            right_start=last_right-25;
            right_end=last_right+10;
			if(right_start<0)
                right_start=0;
			if(right_end>187)
				right_end=187;
            if( ( (get_img(i,right_start))==0 )&& ( (get_img(i,right_start+1))==0 ))
            {
                j=right_start;
            }
            else
            {
                for(j=right_start;j<=right_end;j++)
                {
                    if( (get_img(i,j-1)==255) && ( get_img(i,j)==0 ) && ( get_img(i,j+1)==0 ) )
                    {
                        Right_Board[i]=j-1;
                        hang_flag[i]=0;
                        last_right=j-1;            //���� last_right ��ֵ
                        break;
                    }
                }
            }
            if(j==right_start)
            {
                hang_flag[i] |= RIGHT_LOST_B;
                Right_Board[i]=Right_Board[i+1];
                last_right=Right_Board[i+1];
            }
            else if(j==right_end)
            {
                hang_flag[i] |= RIGHT_LOST_W;
                Right_Board[i]=187;
                last_right=Right_Board[i+1];
            }
            /*******���********/
            left_start=last_left+25;
            left_end=last_left-10;
			if(left_end<0)
                left_end=0;
			if(left_start>187)
				left_start=187;
            if( ( (get_img(i,left_start))==0 )&& ( (get_img(i,left_start-1))==0 ))
            {
                j=left_start;
            }
            else
            {
                for(j=left_start;j>=left_end;j--)
                {
                    if( (get_img(i,j-1)==0) && ( get_img(i,j)==0 )  && ( get_img(i,j+1)==255 ) )
                    {
                        Left_Board[i]=j+1;
                        hang_flag[i]=0;
                        last_left=j+1;
                        break;
                    }
                }
            }
            if(j==left_start)
            {
                hang_flag[i] |= LEFT_LOST_B;
                Left_Board[i]=Left_Board[i+1];
                last_left=Left_Board[i+1];
            }
            else if(j==left_end)
            {
                hang_flag[i] |= LEFT_LOST_W;
                Left_Board[i]=0;
                last_left=Left_Board[i+1];
            }
        }


        if((hang_flag[i]&LEFT_LOST_W)&&(hang_flag[i]&RIGHT_LOST_W))
        {
            hang_flag[i] |= ALL_LOST_W;
            lost_w_count++;
        }
        else  if((hang_flag[i]&LEFT_LOST_B)&&(hang_flag[i]&RIGHT_LOST_B))  //����������ͷ��û�а׵�
        {
            hang_flag[i] |= ALL_LOST_B;
            lost_b_count++;
        }

        if(hang_flag[i]== 0x2A&&valid_line==0)  //��һ�γ���ȫ��
        { valid_line=i+2;    //��valid_lineӦ��ͼ��ͷ����
        oled_valid_line=valid_line;            //��OLED��ʾ
        }

        road_width[i]=Right_Board[i] - Left_Board[i];  //��¼���
        middleline[i]=(Left_Board[i]+Right_Board[i])/2;

        // img[i][middleline[i]]=0;��oled��ʾ

    }//һ��ͼ�����
    /* if (lost_b_count>45)
    {
    pid_motor_right.SetPoint=0;
    pid_motor_left.SetPoint=0;
}*/

}



void find_center(void)
{
  int i,j;
   lost_w_count=lost_b_count=0;
   valid_line=0;

 for(i=63;i>=0;i--)
 {
    if(middleline[i+1]!=0)
     middle_start=middleline[i+1];  ///����һ�ε��е㿪ʼ��Ѱ
    else middle_start=93;

    for(j=middle_start;j>=0;j--)
       {
        if(j>0)
        {
         if(get_img(i,j-1)==0x00&& get_img(i,j)==0x00&&get_img(i,j+1)==0xFF )    //1Ϊ��0Ϊ��  ���¼1
         {
           Left_Board[i]=j+1;
           hang_flag[i]=0;
           break;
         }
        }
        else if (j==0)
        {
          if(get_img(i,j)==0x00&& get_img(i,j+1)==0xFF)
              {
                  Left_Board[i] = j+1;
                  hang_flag[i]=0;

              }

          else if(get_img(i,j)==0xFF)
          {
            Left_Board[i]=0;
            hang_flag[i] |= LEFT_LOST_W;

          }
          else                           //ȫ����
          {
            Left_Board[i]=0;
            hang_flag[i] |= LEFT_LOST_B;

          }
        }
       }

       for(j=middle_start;j<=187;j++)//���м����ұ�ɨ
          {
            if(j<187)
            {
              if (get_img(i,j-1)==0xFF&& get_img(i,j)==0x00&& get_img(i,j+1)==0x00)
              {
                  Right_Board[i] = j-1;
                  hang_flag[i]=0;
                 break;
              }
            }
            else if(j==187)
            {
              if(get_img(i,j-1)==0xFF&& get_img(i,j)==0x00)
              {
                 Right_Board[i] = j-1;
                 hang_flag[i]=0;

              }
              else if(get_img(i,j)==0xFF)//�����ұ��ˣ�Ϊ�׵�
              {
                Right_Board[i] = 187; //�ұ߶�����
                 hang_flag[i] |= RIGHT_LOST_W;
              }
              else
              {
                Right_Board[i]=187;
                  hang_flag[i] |= RIGHT_LOST_B;  //ȫ����
              }
            }
          }


      if((hang_flag[i]&LEFT_LOST_W)&&(hang_flag[i]&RIGHT_LOST_W))
   {
            hang_flag[i] |= ALL_LOST_W;
            lost_w_count++;
   }
    else  if((hang_flag[i]&LEFT_LOST_B)&&(hang_flag[i]&RIGHT_LOST_B))  //����������ͷ��û�а׵�
   {
            hang_flag[i] |= ALL_LOST_B;                     //ֱ��break����ѭ����
            lost_b_count++;
   }

    if(i<63&&hang_flag[i]== 0x2A&&valid_line==0)  //��һ�γ���ȫ��
    { valid_line=i+2;    //��valid_lineӦ��ͼ��ͷ����
       oled_valid_line=valid_line;            //��OLED��ʾ
    }


         road_width[i]=Right_Board[i] - Left_Board[i];  //��¼���
         middleline[i]=(Left_Board[i]+Right_Board[i])/2;
         img[i][middleline[i]]=0;

 }//һ��ͼ�����


}

void edge_filter(uint8_t h_start,uint8_t value)
{
  uint8_t h;
  for(h=h_start;h>=valid_line;h--)
  {

	  if(((Left_Board[h]-Left_Board[h-1])>value)&&((Left_Board[h]-Left_Board[h+1])>value))


	  {
	  	Left_Board[h]=(Left_Board[h-1]+Left_Board[h+1])/2;
	  }

	else  if(((Left_Board[h-1]-Left_Board[h])>value) &&((Left_Board[h+1]-Left_Board[h])>value))

	  {
			Left_Board[h]=(Left_Board[h-1]+Left_Board[h+1])/2;
	   }


	  if(((Right_Board[h]-Right_Board[h-1])>value)&&((Right_Board[h]-Right_Board[h+1])>value))


	  {
		Right_Board[h]=(Right_Board[h-1]+Right_Board[h+1])/2;
	  }

	else  if(((Right_Board[h-1]-Right_Board[h])>value) &&((Right_Board[h+1]-Right_Board[h])>value))

	  {
			Right_Board[h]=(Right_Board[h-1]+Right_Board[h+1])/2;
	   }

   }
}

uint8 zhidao=0;

uint8 oled_interval=0;
void zebra_stop(int Black_Line_Half_Number)
{


  int width_temp=0;
  for (int i=40;i>valid_line;i--)
  {
    int interval_count=0;      //ÿ�н�����
    int interval_flag=0;
   // if((img[i][39]==0||img[i][40]==
   // {
            for(width_temp=Left_Board[i+5];width_temp<Right_Board[i+5];width_temp+=2)
            {
              if(get_img(i,width_temp)==0)
              {
                interval_count++;
              }
              else if(interval_count>2&&get_img(i,width_temp)==255)
              {
                interval_count=0;
                interval_flag++;
              }
            }
            oled_interval=interval_flag;    //    һ�к��OLED

           if(interval_flag>Black_Line_Half_Number)         //һ�к���������������
            {

//             pid_motor_right.SetPoint=0;
//             pid_motor_left.SetPoint=0;
//             gpio_set(B22,0);


            }
            else
            gpio_set(B22,1);
  }





}

uint8 x_l_count=0,x_r_count=0,z_count=0, road_flag=0;  //��OLED


uint8 meeting_time=0;
int meeting_value=0,oled_meeting_value=0;

uint8 Error_orginal = 0;

void find_xuxian(void)
{
  meeting_time++;

  int xuxian_left_count=0,xuxian_right_count=0,zhidao_count=0,xuxian_flag=0;
  int h_end;
    if(valid_line>10)                   //
           h_end=valid_line;
          else
           h_end=10;
  if(
     abs(middleline[20]-middleline[50])<15                       //30
     &&abs(middleline[40]-middleline[50])<15                 //valid_line  ����Զ��Ҫֱ�� ����Զ������
     &&abs(Error_orginal)<10&&
     valid_line<25&&lost_w_count<5)       //������ֱ��
  {zhidao=1;}
  else zhidao=0;

    if(zhidao)
    {

            for(int h=45;h>h_end;h--)
             {
               if(hang_flag[h]==0)   //û����
               {

                   if((Left_Board[h]-Left_Board[h+1]>2 || Left_Board[h]<Left_Board[h+1])
                      && abs(Left_Board[h+1]-Left_Board[h])<7
                      && abs(Left_Board[h]-Left_Board[h-1])<3
                      && Left_Board[h-2]-Left_Board[h-1]<3
                      && Left_Board[h-2]-Left_Board[h-1]>0)

                   {xuxian_left_count++;}

                   if((Right_Board[h+1]-Right_Board[h]>2 || Right_Board[h]>Right_Board[h+1])
                      && abs(Right_Board[h+1]-Right_Board[h])<7
                      && abs(Right_Board[h]-Right_Board[h-1])<3
                      && Right_Board[h-1]-Right_Board[h-2]<3
                      && Right_Board[h-2]-Right_Board[h-1]<0)

                   {xuxian_right_count++;}
                   if(h>30)
                   {
                    if(road_width[h] <= road_width[h-1])
                       xuxian_flag++;
                   }

               }
             }
                x_l_count=xuxian_left_count;            //�����㣬����OLED��ʾ����
                x_r_count=xuxian_right_count;
                z_count=zhidao_count;
                road_flag=xuxian_flag;



              if(xuxian_left_count>2)
              {
               meeting_value++;
              }

              if(xuxian_right_count>2)
              {
               meeting_value++;
              }

    }





     if(meeting_time==5)
     {
       oled_meeting_value=meeting_value;
         if(meeting_value>5)
         {
//           gpio_set(B20,0);
//           pid_motor_right.SetPoint=0;
//            pid_motor_left.SetPoint=0;
         }
         else
            gpio_set(B20,1);

         meeting_value=0;
         meeting_time=0;
     }
}








void shizi(void)
{
 if(lost_w_count>5)
   gpio_set(B21,0);
 else
   gpio_set(B21,1);
}

#endif
