/*
*@auth:sun zixiong
*@func: main program
*2019.6.24
*/


#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "timer.h"
#include "Lcd_Driver.h"
//#include "GUI.h"
#include"lcd.h"
#include "delay.h"
#include "exti.h"
#include "key.h"
#include "beep.h"
#include "dht22.h"
#include "number.h"

//--------FreeRTOS----------------------------
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"


/*Variable definition*/
volatile u32 time;  //ms计时变量
int i = 0;
float Temperature = 0, Humidity = 0;
int Temperature_Min = 10, Temperature_Max = 45, Humidity_Min = 20, Humidity_Max = 100;
u8 Temperature_data[17] = {'0', '0', '0', '0', '0', '0', '0', ' ', 'C', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};
u8 Humidity_data[17] = {'0', '0', '0', '0', '0', '0', '0', ' ', '%', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};
u8 R_buf[4] = {'0', '0', '0', '0'};
int Temperature_Alarm_Flag = 0, Humidity_Alarm_Flag = 0;
int Key_Value = 0;

//********************************************************************************
//接线说明
#define LCD_CTRL   	  	GPIOB				//
#define LCD_LED        	GPIO_Pin_9  //MCU_PB9   		TFT --PIN_LED
#define LCD_CS        	GPIO_Pin_11 //MCU_PB11			TFT --CS
#define LCD_SCL        	GPIO_Pin_13	//MCU_PB13			TFT --SCL
#define LCD_SDA        	GPIO_Pin_15	//MCU_PB15 MOSI	TFT --SDA 
#define LCD_SDO        	GPIO_Pin_14	//MCU_PB14 MISO	TFT --SDO 
#define LCD_RS         	GPIO_Pin_10	//MCU_PB10			TFT --RS/DC
#define LCD_RST     	  GPIO_Pin_12	//MCU_PB12			TFT --RST


//二值信号量句柄
SemaphoreHandle_t Alarm_BinarySemaphore;	//报警二值信号量句柄
SemaphoreHandle_t Get_Data_BinarySemaphore;	//数据采集二值信号量句柄
	



//任务优先级
#define START_TASK_PRIO		1
//任务堆栈大小	
#define START_STK_SIZE 		50  
//任务句柄
TaskHandle_t StartTask_Handler;
//任务函数
void start_task(void *pvParameters);

#define KEY_TASK_PRIO		2
#define KEY_STK_SIZE 		54  
TaskHandle_t KeyTask_Handler;
void key_task(void *pvParameters);


#define TASK1_TASK_PRIO		3 
#define TASK1_STK_SIZE 		48  
TaskHandle_t Task1Task_Handler;
void task1_display(void *pvParameters);

#define QUERY_TASK_TASK_PRIO		4
#define QUERY_STK_SIZE 		60  
TaskHandle_t QueryTask_Handler;
void query_task(void *pvParameters);


#define task3_get_data_PRIO	6
#define TASK3_STK_SIZE 		44  
TaskHandle_t Task3Task_Handler;
void task3_get_data(void *pvParameters);


#define TASK4_ALARM_PRIO		5
#define TASK4_STK_SIZE 		44  
TaskHandle_t Task4Task_Handler;
void task4_Alarm(void *pvParameters);


int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//设置系统中断优先级分组4
	delay_init();	    					//延时函数初始化
	uart_init(115200);					//初始化串口
	LED_Init();		  						//初始化LED
	BEEP_Init();								//初始化蜂鸣器
	LCD_Init();	
	EXTI_Key_Init();						//外部中断初始化
	
    xTaskCreate((TaskFunction_t )start_task,            //
                (const char*    )"start_task",          //
                (uint16_t       )START_STK_SIZE,        //
                (void*          )NULL,                  //
                (UBaseType_t    )START_TASK_PRIO,       //
                (TaskHandle_t*  )&StartTask_Handler);   //
										
   vTaskStartScheduler();          //
}


//
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           	//进入临界区
	
	//创建报警二值信号量
	Alarm_BinarySemaphore=xSemaphoreCreateBinary();			//创建报警信号量
	if(Alarm_BinarySemaphore == NULL)
	{
		printf("Create Alarm_BinarySemaphore failed!!\r\n");
	}else{
		printf("Create Alarm_BinarySemaphore success!!\r\n");
	}
	
	//创建数据采集二值信号量
	Get_Data_BinarySemaphore=xSemaphoreCreateBinary();			//创建报警信号量
	if(Alarm_BinarySemaphore == NULL)
	{
		printf("Create Get_Data_BinarySemaphore failed!!\r\n");
	}else{
		printf("Create Get_Data_BinarySemaphore success!!\r\n");
	}
	
    xTaskCreate((TaskFunction_t )key_task,             
                (const char*    )"key_task",           
                (uint16_t       )KEY_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )KEY_TASK_PRIO,        
                (TaskHandle_t*  )&KeyTask_Handler);  
	
	

    xTaskCreate((TaskFunction_t )task1_display,             
                (const char*    )"task1_display",           
                (uint16_t       )TASK1_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )TASK1_TASK_PRIO,        
                (TaskHandle_t*  )&Task1Task_Handler); 
									
    //
    xTaskCreate((TaskFunction_t )query_task,     
                (const char*    )"query_task",   
                (uint16_t       )QUERY_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )QUERY_TASK_TASK_PRIO,
                (TaskHandle_t*  )&QueryTask_Handler); 
								
		//
    xTaskCreate((TaskFunction_t )task3_get_data,     
                (const char*    )"task3_get_data",   
                (uint16_t       )TASK3_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )task3_get_data_PRIO,
                (TaskHandle_t*  )&Task3Task_Handler); 
										
			//
    xTaskCreate((TaskFunction_t )task4_Alarm,     
                (const char*    )"task4_Alarm",   
                (uint16_t       )TASK4_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )TASK4_ALARM_PRIO,
                (TaskHandle_t*  )&Task4Task_Handler); 

    vTaskDelete(StartTask_Handler); //
    taskEXIT_CRITICAL();            //
}


//key_task
void key_task(void *pvParameters)
{
	
	while(1)
	{
		if(Key_Value == 1)
		{
			printf("Key 1 is pressed!	\n");
			Temperature_Min -= 1;
			printf("Temperature_Min = %0.1d			", Temperature_Min);
			Key_Value = 0;
		}
		else if(Key_Value == 2){
			printf("Key 2 is pressed!	\n");
			Temperature_Min += 1;
			printf("Temperature_Min = %0.1d			", Temperature_Min);
			Key_Value = 0;
		}
		else if(Key_Value == 3){
			printf("Key 3 is pressed!	\n");
			Temperature_Max -= 1;
			printf("Temperature_Max = %0.1d			", Temperature_Max);
			Key_Value = 0;
		}
		else if(Key_Value == 4){
			printf("Key 4 is pressed!	\n");
			Temperature_Max += 1;
			printf("Temperature_Max = %0.1d			", Temperature_Max);
			Key_Value = 0;
		}
		vTaskDelay(1000);
	}
	
	
}


//task1
void task1_display(void *pvParameters)
{
	//int i = 0;
	
	while(1) 
  {			
		if(Alarm_BinarySemaphore != NULL)
		{
			xSemaphoreTake(Alarm_BinarySemaphore, portMAX_DELAY);		//获取二值信号量Alarm_BinarySemaphore，一直等待知道获取到该信号量
			LCD_Clear(WHITE);
			POINT_COLOR=RED;
			LCD_ShowString(20,20,200,16,16,"Temperature and Humidity");
			LCD_ShowString(40,40,200,16,16,"Monitoring System");
			
			POINT_COLOR=BLUE;
			LCD_ShowString(20,80,200,16,16,"Temperature: ");
			Show_number(&Temperature, Temperature_data);
			POINT_COLOR=RED;
			LCD_ShowString(120,80,200,16,16, Temperature_data);
			
			POINT_COLOR=BLUE;
			LCD_ShowString(20,100,200,16,16, "Humidity: ");
			Show_number(&Humidity, Humidity_data);
			POINT_COLOR=RED;
			LCD_ShowString(120,100,200,16,16, Humidity_data);
			
			POINT_COLOR=BLUE;
			LCD_ShowString(20,140,200,16,16, "Range T:");
			Trans_number(&Temperature_Min, R_buf);
			POINT_COLOR=RED;
			LCD_ShowString(95,140,200,16,16, R_buf);
			POINT_COLOR=RED;
			LCD_ShowString(135,140,200,16,16, "-");
			Trans_number(&Temperature_Max, R_buf);
			LCD_ShowString(145,140,200,16,16, R_buf);
			LCD_ShowString(195,140,200,16,16, "C");

			POINT_COLOR=BLUE;
			LCD_ShowString(20,160,200,16,16, "Range H:");
			Trans_number(&Humidity_Min, R_buf);
			POINT_COLOR=RED;
			LCD_ShowString(95,160,200,16,16, R_buf);
			POINT_COLOR=RED;
			LCD_ShowString(135,160,200,16,16, "-");
			Trans_number(&Humidity_Max, R_buf);
			LCD_ShowString(145,160,200,16,16, R_buf);
			LCD_ShowString(195,160,200,16,16, "%");
			
			
			if(Temperature_Alarm_Flag == 1)
			{
				LCD_ShowString(20,200,200,16,16, "Temperature is out of range!!!");
				//Display_String_GBK16(20,200,RED,GRAY0, "Temperature is out of range!!!");
				BEEP = 1;
				Temperature_Alarm_Flag = 0;
				//delay_xms(100);
			}
			
			if(Humidity_Alarm_Flag == 1)
			{
				LCD_ShowString(20,220,200,16,16, "Humity is out of range!!!");
				//Display_String_GBK16(20,220,RED,GRAY0, "Humity is out of range!!!");
				BEEP = 1;
				Humidity_Alarm_Flag = 0;
				//delay_xms(100);
			}
		
			//min-temprature
			LCD_DrawRectangle(60, 250, 120, 300);
			LCD_DrawRectangle(60, 270, 90, 300);
			LCD_DrawRectangle(90, 270, 120, 300);
			POINT_COLOR=BLUE;
			LCD_ShowString(75,253,200,16,16, "Min-T");
			LCD_ShowString(65,280,200,16,16, "-S1");
			LCD_ShowString(95,280,200,16,16, "+S2");
			
			//max-temprature
			LCD_DrawRectangle(120, 250, 180, 300);
			LCD_DrawRectangle(120, 270, 150, 300);
			LCD_DrawRectangle(150, 270, 180, 300);
			POINT_COLOR=BLUE;
			LCD_ShowString(135,253,200,16,16, "Max-T");
			LCD_ShowString(125,280,200,16,16, "-S3");
			LCD_ShowString(155,280,200,16,16, "+S4");
			
			vTaskDelay(1000); 
		}else{
			vTaskDelay(10); 
	}
  }
}


//
void query_task(void *pvParameters)
{
	UBaseType_t query_task_StackMinSize;
	UBaseType_t task1_display_StackMinSize;
	UBaseType_t key_task_StackMinSize;
	UBaseType_t start_task_StackMinSize;
	UBaseType_t task3_get_data_StackMinSize;
	UBaseType_t task4_Alarm_StackMinSize;
	
	
	//u8 task2_num=0;
	while(1)
	{
		query_task_StackMinSize  = uxTaskGetStackHighWaterMark(QueryTask_Handler);	//任务堆栈使用剩余最小大小
		printf("query_task min_stack = %d    \r\n ", query_task_StackMinSize);
		
		task1_display_StackMinSize  = uxTaskGetStackHighWaterMark(Task1Task_Handler);	//任务堆栈使用剩余最小大小
		printf("task1_display min_stack = %d \r\n ", task1_display_StackMinSize);
		
		key_task_StackMinSize  = uxTaskGetStackHighWaterMark(KeyTask_Handler);	//任务堆栈使用剩余最小大小
		printf("key_task min_stack = %d      \r\n ", key_task_StackMinSize);
		
		start_task_StackMinSize  = uxTaskGetStackHighWaterMark(StartTask_Handler);	//任务堆栈使用剩余最小大小
		printf("start_task min_stack = %d    \r\n ", start_task_StackMinSize);
		
		task3_get_data_StackMinSize  = uxTaskGetStackHighWaterMark(Task3Task_Handler);	//任务堆栈使用剩余最小大小
		printf("task3_get_data min_stack = %d\r\n ", task3_get_data_StackMinSize);
		
		task4_Alarm_StackMinSize  = uxTaskGetStackHighWaterMark(Task4Task_Handler);	//任务堆栈使用剩余最小大小
		printf("task4_Alarm min_stack = %d   \r\n ", task4_Alarm_StackMinSize);
		vTaskDelay(2000);
	}
}

void task3_get_data(void *pvParameters)
{
	while(1)
	{
		if(DHT22_Read_Data(&Temperature, &Humidity) == 0)
		{
			xSemaphoreGive(Get_Data_BinarySemaphore);		//释放二值信号量Get_Data_BinarySemaphore
			vTaskDelay(2000);		//温湿度数据采集间隔
		}else{
			vTaskDelay(2000);		//温湿度数据采集间隔
		}	
	}
	
}

void task4_Alarm(void *pvParameters)
{
	//用信号量同步报警
	while(1)
	{
		if(Alarm_BinarySemaphore != NULL)
		{
			xSemaphoreTake(Get_Data_BinarySemaphore, portMAX_DELAY);		//获取二值信号量Get_Data_BinarySemaphore，一直等待知道获取到该信号量
			BEEP = 0;
			if( (Temperature > Temperature_Max) || (Temperature < Temperature_Min) )
			{
				Temperature_Alarm_Flag = 1;	
			}
			if( (Humidity > Humidity_Max) || (Humidity < Humidity_Min))
			{
				Humidity_Alarm_Flag = 1;
			}
			if(Alarm_BinarySemaphore != NULL)
			{
				xSemaphoreGive(Alarm_BinarySemaphore);		//当完成温湿度值是否在监测范围内判断后发送二值信号量Alarm_BinarySemaphore
			}
			
			vTaskDelay(1000);
		}else{
			vTaskDelay(100);
		}
	}
	
}

