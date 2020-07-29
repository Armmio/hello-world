/*
按键配置
Sun Zixiong 2019.5
@zet6
*/


#include "key.h"
#include "led.h"
#include "delay.h"


/*************************************************
PE4--S1
PE3--S2
PE2--S3
PA0--S4
@ZET6
**************************************************/
void KEY_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;												//
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOE, ENABLE);				//
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;										//
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;								//
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;						//
	GPIO_Init(GPIOA, &GPIO_InitStructure);											//
	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;				//
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;															//
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;													//
	GPIO_Init(GPIOE, &GPIO_InitStructure);																		//
	
}




uint8_t KEY_Scan(void)
{
	//监测是否有按键按下
	//S3
	if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_2) == 0)
	{
		delay_us(1000);
		if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_2) == 0)
		{
			while(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_2) == 0);
			return 3;
		}
		else 
			return 0;
	}
	//S2
	else if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_3) == 0)
	{
		delay_us(1000);
		if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_3) == 0)
		{
			while(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_3) == 0);
			return 2;
		}
		else 
			return 0;
	}
	//S1
	else if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4) == 0)
	{
		delay_us(1000);
		if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4) == 0)
		{
			while(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4) == 0);
			return 1;
		}
		else 
			return 0;
	}
	//S4
	else if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 0)
	{
		delay_us(1000);
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 0)
		{
			while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 0);
			return 4;
		}
		else 
			return 0;
	}
	else 
 {
   return 0;
 }
 
}