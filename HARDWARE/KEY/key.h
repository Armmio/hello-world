#ifndef _KEY_H
#define _KEY_H

//#include "stm32f10x.h"


/*************************************************
PE4--S1
PE3--S2
PE2--S3
PA0--S4
@ZET6
**************************************************/

#define KEY3  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2)//读取按键3
#define KEY2  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)//读取按键2
#define KEY1  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)//读取按键1
#define KEY4  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)//读取按键4

#define KEY1_PRES 	1	//KEY1按下
#define KEY2_PRES		2	//KEY2按下
#define KEY3_PRES		3	//KEY3按下
#define KEY4_PRES   4	//KEY4按下(即WK_UP/KEY_UP)


void KEY_Init(void);
//uint8_t KEY_Scan(void);

#endif