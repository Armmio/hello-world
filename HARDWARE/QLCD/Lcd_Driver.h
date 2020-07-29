#ifndef __LCD_DRIVER_H
#define __LCD_DRIVER_H

//----------------includes--------------
#include "stm32f10x.h"

#define RED  	0xf800
#define GREEN	0x07e0
#define BLUE 	0x001f
#define WHITE	0xffff
#define BLACK	0x0000
#define YELLOW  0xFFE0
#define GRAY0   0xEF7D   	//灰色0 3165 00110 001011 00101
#define GRAY1   0x8410      	//灰色1      00000 000000 00000
#define GRAY2   0x4208      	//灰色2  1111111111011111


#define LCD_CTRL   	  	GPIOB				//定义TFT数据端口为PB组
#define LCD_LED        	GPIO_Pin_9  //MCU_PB9   		对应接液晶屏(或者模块)TFT --PIN_LED背光正极（背光可以由IO口提供电流，或者外接3.3V电压）
#define LCD_CS        	GPIO_Pin_11 //MCU_PB11			对应接液晶屏(或者模块)TFT --CS
#define LCD_SCL        	GPIO_Pin_13	//MCU_PB13			对应接液晶屏(或者模块)TFT --SCL
#define LCD_SDA        	GPIO_Pin_15	//MCU_PB15 MOSI	对应接液晶屏(或者模块)TFT --SDA 
#define LCD_SDO        	GPIO_Pin_14	//MCU_PB14 MISO	对应接液晶屏(或者模块)TFT --SDO 
#define LCD_RS         	GPIO_Pin_10	//MCU_PB10			对应接液晶屏(或者模块)TFT --RS/DC
#define LCD_RST     	  GPIO_Pin_12	//MCU_PB12			对应接液晶屏(或者模块)TFT --RST



//#define LCD_CS_SET(x) LCD_CTRL->ODR=(LCD_CTRL->ODR&~LCD_CS)|(x ? LCD_CS:0)

//液晶控制口置1操作语句宏定义
#define	LCD_CS_SET  	LCD_CTRL->BSRR=LCD_CS    
#define	LCD_RS_SET  	LCD_CTRL->BSRR=LCD_RS    
#define	LCD_SDA_SET  	LCD_CTRL->BSRR=LCD_SDA    
#define	LCD_SCL_SET  	LCD_CTRL->BSRR=LCD_SCL    
#define	LCD_RST_SET  	LCD_CTRL->BSRR=LCD_RST    
#define	LCD_LED_SET  	LCD_CTRL->BSRR=LCD_LED    

//液晶控制口置0操作语句宏定义
#define	LCD_CS_CLR  	LCD_CTRL->BRR=LCD_CS    
#define	LCD_RS_CLR  	LCD_CTRL->BRR=LCD_RS    
#define	LCD_SDA_CLR  	LCD_CTRL->BRR=LCD_SDA    
#define	LCD_SCL_CLR  	LCD_CTRL->BRR=LCD_SCL    
#define	LCD_RST_CLR  	LCD_CTRL->BRR=LCD_RST    
#define	LCD_LED_CLR  	LCD_CTRL->BRR=LCD_LED  


#define LCD_DATAOUT(x) LCD_DATA->ODR=x; //数据输出
#define LCD_DATAIN     LCD_DATA->IDR;   //数据输入

#define LCD_WR_DATA(data){\
LCD_RS_SET;\
LCD_CS_CLR;\
LCD_DATAOUT(data);\
LCD_WR_CLR;\
LCD_WR_SET;\
LCD_CS_SET;\
} 



void LCD_GPIO_Init(void);
void Lcd_WriteIndex(u8 Index);
void Lcd_WriteData(u8 Data);
//void Lcd_WriteReg(u8 Index,u8 Data);
u16 Lcd_ReadReg(u8 LCD_Reg);
void Lcd_Reset(void);
void Lcd_Init(void);
void Lcd_Clear(u16 Color);
void Lcd_SetXY(u16 x,u16 y);
void Gui_DrawPoint(u16 x,u16 y,u16 Data);
unsigned int Lcd_ReadPoint(u16 x,u16 y);

#endif
