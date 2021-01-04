#ifndef __CAMARA_H__
#define __CAMARA_H__

#include "headfile.h"


extern uint8 Model_line[40];
extern uint8 left_line[40];
extern uint8 Right_line[40];
extern uint8 img_threshold;
extern uint8 lost_w_count,lost_b_count;
extern uint8 search_finished;
extern uint8 Mutationline;



void ips114_show_cam(void);
uint8 my_adapt_threshold(uint8 *image, uint16 col, uint16 row);
void eage_search(void);
int16 Cal_dis2Straight(void);
void ips_show_middleline(void);







#  endif