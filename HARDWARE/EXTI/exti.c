/*
按键中断配置
Sun Zixiong 2019.6
@zet6
*/

#include "exti.h"
#include "led.h"
#include "key.h"
#include "delay.h"
#include "stm32f10x_exti.h"

extern int Key_Value;



void EXTI_Key_Init(void)		//PE2口外部中断配置函数
{
	EXTI_InitTypeDef EXTI_InitStucture;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	KEY_Init();				//按键端口初始化
	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_AFIO, ENABLE);		//

	//GPIOA.0
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);			//PA0连接到中断线0
	EXTI_InitStucture.EXTI_Line = EXTI_Line0; 			//外部中断线为0
	EXTI_InitStucture.EXTI_Mode = EXTI_Mode_Interrupt;			//设置EXTI线路为中断请求
	EXTI_InitStucture.EXTI_Trigger = EXTI_Trigger_Rising;		//上升沿触发中断
	EXTI_InitStucture.EXTI_LineCmd = ENABLE;		//使能LINE0
	EXTI_Init(&EXTI_InitStucture);						//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
	
	//GPIOE.2
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource2);				//PE2连接到中断线2
	EXTI_InitStucture.EXTI_Line = EXTI_Line2; 			//外部中断线为2
	EXTI_InitStucture.EXTI_Mode = EXTI_Mode_Interrupt;			//设置EXTI线路为中断请求
	EXTI_InitStucture.EXTI_Trigger = EXTI_Trigger_Falling;		//下降沿中断
	EXTI_InitStucture.EXTI_LineCmd = ENABLE;		//使能LINE0
	EXTI_Init(&EXTI_InitStucture);						//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
	
	//GPIOE.3
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource3);				//PE3连接到中断线3
	EXTI_InitStucture.EXTI_Line = EXTI_Line3; 			//外部中断线为3
	EXTI_InitStucture.EXTI_Mode = EXTI_Mode_Interrupt;			//设置EXTI线路为中断请求
	EXTI_InitStucture.EXTI_Trigger = EXTI_Trigger_Falling;		//下降沿中断
	EXTI_InitStucture.EXTI_LineCmd = ENABLE;		//使能LINE0
	EXTI_Init(&EXTI_InitStucture);						//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
	
	//GPIOE.4
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource4);				//PE4连接到中断线4
	EXTI_InitStucture.EXTI_Line = EXTI_Line4; 			//外部中断线为4
	EXTI_InitStucture.EXTI_Mode = EXTI_Mode_Interrupt;			//设置EXTI线路为中断请求
	EXTI_InitStucture.EXTI_Trigger = EXTI_Trigger_Falling;		//下降沿中断
	EXTI_InitStucture.EXTI_LineCmd = ENABLE;		//使能LINE0
	EXTI_Init(&EXTI_InitStucture);						//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;				//使能按键KEY0所在的外部中断通道     
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x06;			//抢占优先级6
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x04;			//子优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);							//根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;				//使能按键KEY0所在的外部中断通道     
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x06;			//抢占优先级6
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;			//子优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);							//根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;				//使能按键KEY0所在的外部中断通道     
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x06;			//抢占优先级6
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;			//子优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);							//根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;				//使能按键KEY0所在的外部中断通道     
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x06;			//抢占优先级6
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;			//子优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);							//根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

}


void EXTI0_IRQHandler(void)	//S4
	{
		delay_us(1000);					//按键消抖
		if(KEY4 == 1)   //确保是否产生了EXTI Line中断
		{
			Key_Value = 4;
		}
		
		 EXTI_ClearITPendingBit(EXTI_Line0);//清除中断标志
		
	}

void EXTI2_IRQHandler(void)	//S3
	{
		delay_us(1000);					//按键消抖
		if((KEY3 == 0))   //确保是否产生了EXTI Line中断
		{
			Key_Value = 3;
		}
		
		 EXTI_ClearITPendingBit(EXTI_Line2);//清除中断标志
		
	}


	void EXTI4_IRQHandler(void)	//S1
	{
		delay_us(1000);					//按键消抖
		if((KEY1 == 0) )  //确保是否产生了EXTI Line中断
		{
			Key_Value = 1;
		}
		
		 EXTI_ClearITPendingBit(EXTI_Line4);//清除中断标志
		
	}

		void EXTI3_IRQHandler(void)	//S2
	{
		delay_us(1000);					//按键消抖
		if((KEY2 == 0))   //确保是否产生了EXTI Line中断
		{
			//LED3取反
			Key_Value = 2;
		}
		 EXTI_ClearITPendingBit(EXTI_Line3);//??LINE4???????
		
	}







