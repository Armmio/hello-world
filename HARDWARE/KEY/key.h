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

#define KEY3  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2)//��ȡ����3
#define KEY2  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)//��ȡ����2
#define KEY1  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)//��ȡ����1
#define KEY4  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)//��ȡ����4

#define KEY1_PRES 	1	//KEY1����
#define KEY2_PRES		2	//KEY2����
#define KEY3_PRES		3	//KEY3����
#define KEY4_PRES   4	//KEY4����(��WK_UP/KEY_UP)


void KEY_Init(void);
//uint8_t KEY_Scan(void);

#endif