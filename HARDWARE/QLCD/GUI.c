//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//测试硬件：QDtech STM32开发板
//2.2寸天马原装串口液晶屏综合测试程序
//基于@深圳市全动电子技术有限公司  V1.2版

//********************************************************************************
//STM32程序使用说明：
//1.如需改程序默认IO接线，请修改lcd_deiver.h中的IO配置，同时要修改Lcd_driver.c 中GPIO初始化配置
//2.本程序定义了横竖屏切换宏，默认是竖屏演示，如需开启横屏请将lcd_config.h中的//#define H_VIEW启用

//********************************************************************************
//默认程序接线说明：
#define LCD_CTRL   	  	GPIOB				//定义TFT数据端口为PB组
#define LCD_LED        	GPIO_Pin_9  //MCU_PB9   		对应接液晶屏(或者模块)TFT --PIN_LED背光正极（背光可以由IO口提供电流，或者外接3.3V电压）
#define LCD_CS        	GPIO_Pin_11 //MCU_PB11			对应接液晶屏(或者模块)TFT --CS
#define LCD_SCL        	GPIO_Pin_13	//MCU_PB13			对应接液晶屏(或者模块)TFT --SCL
#define LCD_SDA        	GPIO_Pin_15	//MCU_PB15 MOSI	对应接液晶屏(或者模块)TFT --SDA 
#define LCD_SDO        	GPIO_Pin_14	//MCU_PB14 MISO	对应接液晶屏(或者模块)TFT --SDO 
#define LCD_RS         	GPIO_Pin_10	//MCU_PB10			对应接液晶屏(或者模块)TFT --RS/DC
#define LCD_RST     	  GPIO_Pin_12	//MCU_PB12			对应接液晶屏(或者模块)TFT --RST

//********************************************************************************

//本测试程序演示功能包括：
//1.纯色填充测试
//2.英文显示测试示例
//3.中文显示测试示例
//4.2D按钮显示测试示例
//5.数码管字体数字显示测试示例
//6.图片显示测试示例
//7.横竖屏切换测试示例

//////////////////////////////////////////////////////////////////////////////////	
#include "stm32f10x.h"
#include "Lcd_Driver.h"
#include "GUI.h"
#include "delay.h"
#include "font.h"

//从ILI93xx读出的数据为GBR格式，而我们写入的时候为RGB格式。
//通过该函数转换
//c:GBR格式的颜色值
//返回值：RGB格式的颜色值
u16 LCD_BGR2RGB(u16 c)
{
  u16  r,g,b,rgb;   
  b=(c>>0)&0x1f;
  g=(c>>5)&0x3f;
  r=(c>>11)&0x1f;	 
  rgb=(b<<11)+(g<<5)+(r<<0);		 
  return(rgb);

}

/*
draw a Circle
fc-----圆圈颜色
(x, y)圆心坐标
*/
void Draw_Circle(u16 X,u16 Y,u16 R,u16 fc) 
{//Bresenham算法 
    unsigned short  a,b; 
    int c; 
    a=0; 
    b=R; 
    c=3-2*R; 
    while (a<b) 
    { 
        Gui_DrawPoint(X+a,Y+b,fc);     //        7 
        Gui_DrawPoint(X-a,Y+b,fc);     //        6 
        Gui_DrawPoint(X+a,Y-b,fc);     //        2 
        Gui_DrawPoint(X-a,Y-b,fc);     //        3 
        Gui_DrawPoint(X+b,Y+a,fc);     //        8 
        Gui_DrawPoint(X-b,Y+a,fc);     //        5 
        Gui_DrawPoint(X+b,Y-a,fc);     //        1 
        Gui_DrawPoint(X-b,Y-a,fc);     //        4 

        if(c<0) c=c+4*a+6; 
        else 
        { 
            c=c+4*(a-b)+10; 
            b-=1; 
        } 
       a+=1; 
    } 
    if (a==b) 
    { 
        Gui_DrawPoint(X+a,Y+b,fc); 
        Gui_DrawPoint(X+a,Y+b,fc); 
        Gui_DrawPoint(X+a,Y-b,fc); 
        Gui_DrawPoint(X-a,Y-b,fc); 
        Gui_DrawPoint(X+b,Y+a,fc); 
        Gui_DrawPoint(X-b,Y+a,fc); 
        Gui_DrawPoint(X+b,Y-a,fc); 
        Gui_DrawPoint(X-b,Y-a,fc); 
    } 
	
} 
//画线函数，使用Bresenham 画线算法
void Draw_Line(u16 x0, u16 y0,u16 x1, u16 y1,u16 Color)   
{
int dx,             // difference in x's
    dy,             // difference in y's
    dx2,            // dx,dy * 2
    dy2, 
    x_inc,          // amount in pixel space to move during drawing
    y_inc,          // amount in pixel space to move during drawing
    error,          // the discriminant i.e. error i.e. decision variable
    index;          // used for looping	


	Lcd_SetXY(x0,y0);
	dx = x1-x0;//计算x距离
	dy = y1-y0;//计算y距离

	if (dx>=0)
	{
		x_inc = 1;
	}
	else
	{
		x_inc = -1;
		dx    = -dx;  
	} 
	
	if (dy>=0)
	{
		y_inc = 1;
	} 
	else
	{
		y_inc = -1;
		dy    = -dy; 
	} 

	dx2 = dx << 1;
	dy2 = dy << 1;

	if (dx > dy)//x距离大于y距离，那么每个x轴上只有一个点，每个y轴上有若干个点
	{//且线的点数等于x距离，以x轴递增画点
		// initialize error term
		error = dy2 - dx; 

		// draw the line
		for (index=0; index <= dx; index++)//要画的点数不会超过x距离
		{
			//画点
			Gui_DrawPoint(x0,y0,Color);
			
			// test if error has overflowed
			if (error >= 0) //是否需要增加y坐标值
			{
				error-=dx2;

				// move to next line
				y0+=y_inc;//增加y坐标值
			} // end if error overflowed

			// adjust the error term
			error+=dy2;

			// move to the next pixel
			x0+=x_inc;//x坐标值每次画点后都递增1
		} // end for
	} // end if |slope| <= 1
	else//y轴大于x轴，则每个y轴上只有一个点，x轴若干个点
	{//以y轴为递增画点
		// initialize error term
		error = dx2 - dy; 

		// draw the line
		for (index=0; index <= dy; index++)
		{
			// set the pixel
			Gui_DrawPoint(x0,y0,Color);

			// test if error overflowed
			if (error >= 0)
			{
				error-=dy2;

				// move to next line
				x0+=x_inc;
			} // end if error overflowed

			// adjust the error term
			error+=dx2;

			// move to the next pixel
			y0+=y_inc;
		} // end for
	} // end else |slope| > 1
}


/*
画一个矩形
bc------矩形颜色
*/
void Draw_box(u16 x, u16 y, u16 w, u16 h,u16 bc)
{
	Draw_Line(x,y,x+w,y,0xEF7D);
	Draw_Line(x+w-1,y+1,x+w-1,y+1+h,0x2965);
	Draw_Line(x,y+h,x+w,y+h,0x2965);
	Draw_Line(x,y,x,y+h,0xEF7D);
    Draw_Line(x+1,y+1,x+1+w-2,y+1+h-2,bc);
}


void Draw_box2(u16 x,u16 y,u16 w,u16 h, u8 mode)
{
	if (mode==0)	{
		Draw_Line(x,y,x+w,y,0xEF7D);
		Draw_Line(x+w-1,y+1,x+w-1,y+1+h,0x2965);
		Draw_Line(x,y+h,x+w,y+h,0x2965);
		Draw_Line(x,y,x,y+h,0xEF7D);
		}
	if (mode==1)	{
		Draw_Line(x,y,x+w,y,0x2965);
		Draw_Line(x+w-1,y+1,x+w-1,y+1+h,0xEF7D);
		Draw_Line(x,y+h,x+w,y+h,0xEF7D);
		Draw_Line(x,y,x,y+h,0x2965);
	}
	if (mode==2)	{
		Draw_Line(x,y,x+w,y,0xffff);
		Draw_Line(x+w-1,y+1,x+w-1,y+1+h,0xffff);
		Draw_Line(x,y+h,x+w,y+h,0xffff);
		Draw_Line(x,y,x,y+h,0xffff);
	}
}



/**************************************************************************************
功能描述: 在屏幕显示一凹下的按钮框
输    入: u16 x1,y1,x2,y2 按钮框左上角和右下角坐标
输    出: 无
**************************************************************************************/
void Draw_ButtonUp(u16 x1,u16 y1,u16 x2,u16 y2)
{
	Draw_Line(x1,  y1,  x2,y1, GRAY1); //
	Draw_Line(x1,  y2,  x2,y2, GRAY1); //
	
	Draw_Line(x1,  y1+1,  x1,y2-1, GRAY1); //
	Draw_Line(x2,  y1+1,  x2,y2-1, GRAY1); //
}

/*
文字显示函数
(x, y)显示起点坐标
fc,bc-----设置字体颜色
s-------显示字符串开始地址
*/
void Display_String_GBK16(u16 x, u16 y, u16 fc, u16 bc, u8 *s)
{
	unsigned char i,j;
	unsigned short k,x0;
	x0=x;

	while(*s) 
	{	
		if((*s) < 128) 
		{
			k=*s;
			if (k==13) 
			{
				x=x0;
				y+=16;
			}
			else 
			{
				if (k>32) k-=32; else k=0;
	
			    for(i=0;i<16;i++)
				for(j=0;j<8;j++) 
					{
				    	if(asc16[k*16+i]&(0x80>>j))	Gui_DrawPoint(x+j,y+i,fc);
						else 
						{
							if (fc!=bc) Gui_DrawPoint(x+j,y+i,bc);
						}
					}
				x+=8;
			}
			s++;
		}
			
		else 
		{
		

			for (k=0;k<hz16_num;k++) 
			{
			  if ((hz16[k].Index[0]==*(s))&&(hz16[k].Index[1]==*(s+1)))
			  { 
				    for(i=0;i<16;i++)
				    {
						for(j=0;j<8;j++) 
							{
						    	if(hz16[k].Msk[i*2]&(0x80>>j))	Gui_DrawPoint(x+j,y+i,fc);
								else {
									if (fc!=bc) Gui_DrawPoint(x+j,y+i,bc);
								}
							}
						for(j=0;j<8;j++) 
							{
						    	if(hz16[k].Msk[i*2+1]&(0x80>>j))	Gui_DrawPoint(x+j+8,y+i,fc);
								else 
								{
									if (fc!=bc) Gui_DrawPoint(x+j+8,y+i,bc);
								}
							}
				    }
				}
			  }
			s+=2;x+=16;
		} 
		
	}
}

void Display_String_GBK24(u16 x, u16 y, u16 fc, u16 bc, u8 *s)
{
	unsigned char i,j;
	unsigned short k;

	while(*s) 
	{
		if( *s < 0x80 ) 
		{
			k=*s;
			if (k>32) k-=32; else k=0;

		    for(i=0;i<16;i++)
			for(j=0;j<8;j++) 
				{
			    	if(asc16[k*16+i]&(0x80>>j))	
					Gui_DrawPoint(x+j,y+i,fc);
					else 
					{
						if (fc!=bc) Gui_DrawPoint(x+j,y+i,bc);
					}
				}
			s++;x+=8;
		}
		else 
		{

			for (k=0;k<hz24_num;k++) 
			{
			  if ((hz24[k].Index[0]==*(s))&&(hz24[k].Index[1]==*(s+1)))
			  { 
				    for(i=0;i<24;i++)
				    {
						for(j=0;j<8;j++) 
							{
						    	if(hz24[k].Msk[i*3]&(0x80>>j))
								Gui_DrawPoint(x+j,y+i,fc);
								else 
								{
									if (fc!=bc) Gui_DrawPoint(x+j,y+i,bc);
								}
							}
						for(j=0;j<8;j++) 
							{
						    	if(hz24[k].Msk[i*3+1]&(0x80>>j))	Gui_DrawPoint(x+j+8,y+i,fc);
								else {
									if (fc!=bc) Gui_DrawPoint(x+j+8,y+i,bc);
								}
							}
						for(j=0;j<8;j++) 
							{
						    	if(hz24[k].Msk[i*3+2]&(0x80>>j))	
								Gui_DrawPoint(x+j+16,y+i,fc);
								else 
								{
									if (fc!=bc) Gui_DrawPoint(x+j+16,y+i,bc);
								}
							}
				    }
			  }
			}
			s+=2;x+=24;
		}
	}
}


/*
显示数字
*/
void Display_String_Num32(u16 x, u16 y, u16 fc, u16 bc, u16 num)
{
	unsigned char i,j,k,c;

    for(i=0;i<32;i++)
	{
		for(j=0;j<4;j++) 
		{
			c=*(sz32+num*32*4+i*4+j);
			for (k=0;k<8;k++)	
			{
	
		    	if(c&(0x80>>k))	Gui_DrawPoint(x+j*8+k,y+i,fc);
				else {
					if (fc!=bc) Gui_DrawPoint(x+j*8+k,y+i,bc);
				}
			}
		}
	}
}
































////////////////////////////////////////////////////////////////////////////////////	 
////本程序只供学习使用，未经作者许可，不得用于其它任何用途
////测试硬件：QDtech STM32开发板
////2.2寸天马原装串口液晶屏综合测试程序
////xiao冯@QDtech., LTD

////QQ交流群:324828016
////固话  +86 0755-23594567 
////传真  +86 0755-23594567 
////邮箱  QDtech2008@gmail.com 
////MSN:  QDtech2008@gmail.com
////Skype:QDtech2008
////订单联系:18823372746
////技术支持:15989313508
////淘宝店址：http://qdtech.taobao.com/
////博客http://qdtech2013.blog.163.com/
////新浪邮箱：qdtech2013@sina.com

////修改日期:2013/5/27
////版本：V1.2
////版权所有，盗版必究。
////Copyright(C) 深圳市全动电子技术有限公司 2008-2020
////All rights reserved

////********************************************************************************
////STM32程序使用说明：
////1.如需改程序默认IO接线，请修改lcd_deiver.h中的IO配置，同时要修改Lcd_driver.c 中GPIO初始化配置
////2.本程序定义了横竖屏切换宏，默认是竖屏演示，如需开启横屏请将lcd_config.h中的//#define H_VIEW启用
////3.如果您的单片机容量小于64K，可能会导致程序装载不下，请尽量选择大容量的STM32芯片来完成图片显示测试


////********************************************************************************
////默认程序接线说明：
//#define LCD_CTRL   	  	GPIOB				//定义TFT数据端口为PB组
//#define LCD_LED        	GPIO_Pin_9  //MCU_PB9   		对应接液晶屏(或者模块)TFT --PIN_LED背光正极（背光可以由IO口提供电流，或者外接3.3V电压）
//#define LCD_CS        	GPIO_Pin_11 //MCU_PB11			对应接液晶屏(或者模块)TFT --CS
//#define LCD_SCL        	GPIO_Pin_13	//MCU_PB13			对应接液晶屏(或者模块)TFT --SCL
//#define LCD_SDA        	GPIO_Pin_15	//MCU_PB15 MOSI	对应接液晶屏(或者模块)TFT --SDA 
//#define LCD_SDO        	GPIO_Pin_14	//MCU_PB14 MISO	对应接液晶屏(或者模块)TFT --SDO 
//#define LCD_RS         	GPIO_Pin_10	//MCU_PB10			对应接液晶屏(或者模块)TFT --RS/DC
//#define LCD_RST     	  GPIO_Pin_12	//MCU_PB12			对应接液晶屏(或者模块)TFT --RST

////********************************************************************************

////本测试程序演示功能包括：
////1.纯色填充测试
////2.英文显示测试示例
////3.中文显示测试示例
////4.2D按钮显示测试示例
////5.数码管字体数字显示测试示例
////6.图片显示测试示例
////7.横竖屏切换测试示例

////////////////////////////////////////////////////////////////////////////////////	
//#include "stm32f10x.h"
//#include "Lcd_Driver.h"
//#include "GUI.h"
//#include "delay.h"
//#include "font.h"

////从ILI93xx读出的数据为GBR格式，而我们写入的时候为RGB格式。
////通过该函数转换
////c:GBR格式的颜色值
////返回值：RGB格式的颜色值
//u16 LCD_BGR2RGB(u16 c)
//{
//  u16  r,g,b,rgb;   
//  b=(c>>0)&0x1f;
//  g=(c>>5)&0x3f;
//  r=(c>>11)&0x1f;	 
//  rgb=(b<<11)+(g<<5)+(r<<0);		 
//  return(rgb);

//}

///*
//draw a Circle
//fc-----圆圈颜色
//(x, y)圆心坐标
//*/
//void Gui_Circle(u16 X,u16 Y,u16 R,u16 fc) 
//{//Bresenham算法 
//    unsigned short  a,b; 
//    int c; 
//    a=0; 
//    b=R; 
//    c=3-2*R; 
//    while (a<b) 
//    { 
//        Gui_DrawPoint(X+a,Y+b,fc);     //        7 
//        Gui_DrawPoint(X-a,Y+b,fc);     //        6 
//        Gui_DrawPoint(X+a,Y-b,fc);     //        2 
//        Gui_DrawPoint(X-a,Y-b,fc);     //        3 
//        Gui_DrawPoint(X+b,Y+a,fc);     //        8 
//        Gui_DrawPoint(X-b,Y+a,fc);     //        5 
//        Gui_DrawPoint(X+b,Y-a,fc);     //        1 
//        Gui_DrawPoint(X-b,Y-a,fc);     //        4 

//        if(c<0) c=c+4*a+6; 
//        else 
//        { 
//            c=c+4*(a-b)+10; 
//            b-=1; 
//        } 
//       a+=1; 
//    } 
//    if (a==b) 
//    { 
//        Gui_DrawPoint(X+a,Y+b,fc); 
//        Gui_DrawPoint(X+a,Y+b,fc); 
//        Gui_DrawPoint(X+a,Y-b,fc); 
//        Gui_DrawPoint(X-a,Y-b,fc); 
//        Gui_DrawPoint(X+b,Y+a,fc); 
//        Gui_DrawPoint(X-b,Y+a,fc); 
//        Gui_DrawPoint(X+b,Y-a,fc); 
//        Gui_DrawPoint(X-b,Y-a,fc); 
//    } 
//	
//} 
////画线函数，使用Bresenham 画线算法
//void Gui_DrawLine(u16 x0, u16 y0,u16 x1, u16 y1,u16 Color)   
//{
//int dx,             // difference in x's
//    dy,             // difference in y's
//    dx2,            // dx,dy * 2
//    dy2, 
//    x_inc,          // amount in pixel space to move during drawing
//    y_inc,          // amount in pixel space to move during drawing
//    error,          // the discriminant i.e. error i.e. decision variable
//    index;          // used for looping	


//	Lcd_SetXY(x0,y0);
//	dx = x1-x0;//计算x距离
//	dy = y1-y0;//计算y距离

//	if (dx>=0)
//	{
//		x_inc = 1;
//	}
//	else
//	{
//		x_inc = -1;
//		dx    = -dx;  
//	} 
//	
//	if (dy>=0)
//	{
//		y_inc = 1;
//	} 
//	else
//	{
//		y_inc = -1;
//		dy    = -dy; 
//	} 

//	dx2 = dx << 1;
//	dy2 = dy << 1;

//	if (dx > dy)//x距离大于y距离，那么每个x轴上只有一个点，每个y轴上有若干个点
//	{//且线的点数等于x距离，以x轴递增画点
//		// initialize error term
//		error = dy2 - dx; 

//		// draw the line
//		for (index=0; index <= dx; index++)//要画的点数不会超过x距离
//		{
//			//画点
//			Gui_DrawPoint(x0,y0,Color);
//			
//			// test if error has overflowed
//			if (error >= 0) //是否需要增加y坐标值
//			{
//				error-=dx2;

//				// move to next line
//				y0+=y_inc;//增加y坐标值
//			} // end if error overflowed

//			// adjust the error term
//			error+=dy2;

//			// move to the next pixel
//			x0+=x_inc;//x坐标值每次画点后都递增1
//		} // end for
//	} // end if |slope| <= 1
//	else//y轴大于x轴，则每个y轴上只有一个点，x轴若干个点
//	{//以y轴为递增画点
//		// initialize error term
//		error = dx2 - dy; 

//		// draw the line
//		for (index=0; index <= dy; index++)
//		{
//			// set the pixel
//			Gui_DrawPoint(x0,y0,Color);

//			// test if error overflowed
//			if (error >= 0)
//			{
//				error-=dy2;

//				// move to next line
//				x0+=x_inc;
//			} // end if error overflowed

//			// adjust the error term
//			error+=dx2;

//			// move to the next pixel
//			y0+=y_inc;
//		} // end for
//	} // end else |slope| > 1
//}


///*
//画一个矩形
//bc------矩形颜色
//*/
//void Gui_box(u16 x, u16 y, u16 w, u16 h,u16 bc)
//{
//	Gui_DrawLine(x,y,x+w,y,0xEF7D);
//	Gui_DrawLine(x+w-1,y+1,x+w-1,y+1+h,0x2965);
//	Gui_DrawLine(x,y+h,x+w,y+h,0x2965);
//	Gui_DrawLine(x,y,x,y+h,0xEF7D);
//    Gui_DrawLine(x+1,y+1,x+1+w-2,y+1+h-2,bc);
//}


//void Gui_box2(u16 x,u16 y,u16 w,u16 h, u8 mode)
//{
//	if (mode==0)	{
//		Gui_DrawLine(x,y,x+w,y,0xEF7D);
//		Gui_DrawLine(x+w-1,y+1,x+w-1,y+1+h,0x2965);
//		Gui_DrawLine(x,y+h,x+w,y+h,0x2965);
//		Gui_DrawLine(x,y,x,y+h,0xEF7D);
//		}
//	if (mode==1)	{
//		Gui_DrawLine(x,y,x+w,y,0x2965);
//		Gui_DrawLine(x+w-1,y+1,x+w-1,y+1+h,0xEF7D);
//		Gui_DrawLine(x,y+h,x+w,y+h,0xEF7D);
//		Gui_DrawLine(x,y,x,y+h,0x2965);
//	}
//	if (mode==2)	{
//		Gui_DrawLine(x,y,x+w,y,0xffff);
//		Gui_DrawLine(x+w-1,y+1,x+w-1,y+1+h,0xffff);
//		Gui_DrawLine(x,y+h,x+w,y+h,0xffff);
//		Gui_DrawLine(x,y,x,y+h,0xffff);
//	}
//}



///**************************************************************************************
//功能描述: 在屏幕显示一凹下的按钮框
//输    入: u16 x1,y1,x2,y2 按钮框左上角和右下角坐标
//输    出: 无
//**************************************************************************************/
//void DisplayButtonUp(u16 x1,u16 y1,u16 x2,u16 y2)
//{
//	Gui_DrawLine(x1,  y1,  x2,y1, GRAY1); //
//	Gui_DrawLine(x1,  y2,  x2,y2, GRAY1); //
//	
//	Gui_DrawLine(x1,  y1+1,  x1,y2-1, GRAY1); //
//	Gui_DrawLine(x2,  y1+1,  x2,y2-1, GRAY1); //
//}

///*
//文字显示函数
//(x, y)显示起点坐标
//fc,bc-----设置字体颜色
//s-------显示字符串开始地址
//*/
//void Gui_DrawFont_GBK16(u16 x, u16 y, u16 fc, u16 bc, u8 *s)
//{
//	unsigned char i,j;
//	unsigned short k,x0;
//	x0=x;

//	while(*s) 
//	{	
//		if((*s) < 128) 
//		{
//			k=*s;
//			if (k==13) 
//			{
//				x=x0;
//				y+=16;
//			}
//			else 
//			{
//				if (k>32) k-=32; else k=0;
//	
//			    for(i=0;i<16;i++)
//				for(j=0;j<8;j++) 
//					{
//				    	if(asc16[k*16+i]&(0x80>>j))	Gui_DrawPoint(x+j,y+i,fc);
//						else 
//						{
//							if (fc!=bc) Gui_DrawPoint(x+j,y+i,bc);
//						}
//					}
//				x+=8;
//			}
//			s++;
//		}
//			
//		else 
//		{
//		

//			for (k=0;k<hz16_num;k++) 
//			{
//			  if ((hz16[k].Index[0]==*(s))&&(hz16[k].Index[1]==*(s+1)))
//			  { 
//				    for(i=0;i<16;i++)
//				    {
//						for(j=0;j<8;j++) 
//							{
//						    	if(hz16[k].Msk[i*2]&(0x80>>j))	Gui_DrawPoint(x+j,y+i,fc);
//								else {
//									if (fc!=bc) Gui_DrawPoint(x+j,y+i,bc);
//								}
//							}
//						for(j=0;j<8;j++) 
//							{
//						    	if(hz16[k].Msk[i*2+1]&(0x80>>j))	Gui_DrawPoint(x+j+8,y+i,fc);
//								else 
//								{
//									if (fc!=bc) Gui_DrawPoint(x+j+8,y+i,bc);
//								}
//							}
//				    }
//				}
//			  }
//			s+=2;x+=16;
//		} 
//		
//	}
//}

//void Gui_DrawFont_GBK24(u16 x, u16 y, u16 fc, u16 bc, u8 *s)
//{
//	unsigned char i,j;
//	unsigned short k;

//	while(*s) 
//	{
//		if( *s < 0x80 ) 
//		{
//			k=*s;
//			if (k>32) k-=32; else k=0;

//		    for(i=0;i<16;i++)
//			for(j=0;j<8;j++) 
//				{
//			    	if(asc16[k*16+i]&(0x80>>j))	
//					Gui_DrawPoint(x+j,y+i,fc);
//					else 
//					{
//						if (fc!=bc) Gui_DrawPoint(x+j,y+i,bc);
//					}
//				}
//			s++;x+=8;
//		}
//		else 
//		{

//			for (k=0;k<hz24_num;k++) 
//			{
//			  if ((hz24[k].Index[0]==*(s))&&(hz24[k].Index[1]==*(s+1)))
//			  { 
//				    for(i=0;i<24;i++)
//				    {
//						for(j=0;j<8;j++) 
//							{
//						    	if(hz24[k].Msk[i*3]&(0x80>>j))
//								Gui_DrawPoint(x+j,y+i,fc);
//								else 
//								{
//									if (fc!=bc) Gui_DrawPoint(x+j,y+i,bc);
//								}
//							}
//						for(j=0;j<8;j++) 
//							{
//						    	if(hz24[k].Msk[i*3+1]&(0x80>>j))	Gui_DrawPoint(x+j+8,y+i,fc);
//								else {
//									if (fc!=bc) Gui_DrawPoint(x+j+8,y+i,bc);
//								}
//							}
//						for(j=0;j<8;j++) 
//							{
//						    	if(hz24[k].Msk[i*3+2]&(0x80>>j))	
//								Gui_DrawPoint(x+j+16,y+i,fc);
//								else 
//								{
//									if (fc!=bc) Gui_DrawPoint(x+j+16,y+i,bc);
//								}
//							}
//				    }
//			  }
//			}
//			s+=2;x+=24;
//		}
//	}
//}


///*
//显示数字
//*/
//void Gui_DrawFont_Num32(u16 x, u16 y, u16 fc, u16 bc, u16 num)
//{
//	unsigned char i,j,k,c;

//    for(i=0;i<32;i++)
//	{
//		for(j=0;j<4;j++) 
//		{
//			c=*(sz32+num*32*4+i*4+j);
//			for (k=0;k<8;k++)	
//			{
//	
//		    	if(c&(0x80>>k))	Gui_DrawPoint(x+j*8+k,y+i,fc);
//				else {
//					if (fc!=bc) Gui_DrawPoint(x+j*8+k,y+i,bc);
//				}
//			}
//		}
//	}
//}


