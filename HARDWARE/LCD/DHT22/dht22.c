#include "dht22.h"
#include "timer.h"
#include "usart.h"
#include "delay.h"

char dht22_data[5] = {0,0,0,0,0};    		//传感器温湿度数据存储变量
char checked_data[5] = {0,0,0,0,0};     //校验后的温湿度数据
char checksum; 
//extern u16 TimeCounter;

void SetAsOutput(void)
{
        GPIO_InitTypeDef GPIO_InitStructure_B;                 
        
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE); 	//enable APB2 external clock
        GPIO_InitStructure_B.GPIO_Pin =   SDA;                  //设置IO口
        GPIO_InitStructure_B.GPIO_Mode =  GPIO_Mode_Out_PP;      //推挽输出模式
        GPIO_InitStructure_B.GPIO_Speed = GPIO_Speed_50MHz;      //引脚频率
        GPIO_Init(GPIOB,&GPIO_InitStructure_B);                  //init GPIOB SDA by GPIO_InitStructure structure       
}

void SetAsIutput(void)
{
        GPIO_InitTypeDef GPIO_InitStructure_B;                        

        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE); 	//enable APB2 external clock
                /*  ??GPIO_A????IO? */
        GPIO_InitStructure_B.GPIO_Pin =   SDA;             
				GPIO_InitStructure_B.GPIO_Mode =  GPIO_Mode_IN_FLOATING ;              //上拉输入
        GPIO_InitStructure_B.GPIO_Speed = GPIO_Speed_50MHz;          //50MHZ
        GPIO_Init(GPIOB,&GPIO_InitStructure_B);                      //init GPIOB SDA by GPIO_InitStructure structure      
}

/*******************************************************************************
* 函数名： Read_Data
* 功能：从DHT22读取8bits温湿度数据
*输入参数: 无
*输出参数: 无
*******************************************************************************/ 
char Read_Data(void)
{
	char data = 0;
	int i = 0;
	
	
	for(i=1; i<9; i++)
	{
		while(!GPIO_ReadInputDataBit(GPIOB, SDA));
		delay_us(32);

    if(GPIO_ReadInputDataBit(GPIOB, SDA))		//读出第i位为1
    {
			data |= (1 << (8-i));
			while(GPIO_ReadInputDataBit(GPIOB, SDA));	
			//while(!GPIO_ReadInputDataBit(GPIOB, SDA));				
		}                                                        
    else			//读出第i位为0
    {
			data |= (1 << (8-i));
			//while(!GPIO_ReadInputDataBit(GPIOB, SDA));
		}  
	}
	return data;
}


/*******************************************************************************
* 函数名： HDataRead
* 功能：从DHT22读取温湿度数据
*输入参数: 无
*输出参数: 无
*******************************************************************************/        
        
void DataRead(void)                           
{
	int i = 0, response_flag = 0, start_signal_flag = 0;
	delay_us(1000);		//等待 1s 以越过不稳定状态
	SetAsOutput();                  //端口配置为输出模式
	GPIO_ResetBits(GPIOB,SDA);          //拉低引脚
	delay_us(18000);				//拉低18ms
	printf("Delay 18ms fineshed      \n");
	
	GPIO_SetBits(GPIOB,SDA);          //拉低引脚
	delay_us(30);	
	
	
	SetAsIutput();      //主机设为输入模式（释放总线）
		
	
	while(1)		//DHT22
	{
		 if(!GPIO_ReadInputDataBit(GPIOB, SDA))                //DHT22有响应信号输出
		{
			printf("DHT22 send response signal		\n\n");
			response_flag = 1;
			break;
		}
	}
	
	if(response_flag)
	{
		while(!GPIO_ReadInputDataBit(GPIOB, SDA));      //等待DHT22的80us响应信号结束
		while(1)
	{
		 if(GPIO_ReadInputDataBit(GPIOB, SDA))                //DHT22有开始信号输出
		{
			//printf("DHT22 send start signal		\n\n");
			start_signal_flag = 1;
			break;
		}
	}
	}else{
		printf("DHT22 no response ");
	}
	
	if(start_signal_flag)
	{
		printf("DHT22 send start signal			\n\n");
		while(GPIO_ReadInputDataBit(GPIOB, SDA));      //等待DHT22的80us响应信号结束
		printf("DHT22 start signal finished			\n\n");
		
		dht22_data[0] = Read_Data();	//温度高8位
		printf("dht22_data[0]:");
		printf("%d\n\n", (int)dht22_data[0]);
					
		dht22_data[1] = Read_Data();
		printf("dht22_data[1]:");
		printf("%d\n\n", (int)dht22_data[1]);
			
		dht22_data[2]  = Read_Data();		//湿度高8位
		printf("dht22_data[2]:");
		printf("%d\n\n", (int)dht22_data[2]);
			
    dht22_data[3]  = Read_Data();
		printf("dht22_data[3]:");
		printf("%d\n\n", (int)dht22_data[3]);
			
    dht22_data[4] = Read_Data();		//校验和
		printf("dht22_data[4]:");
		printf("%d\n\n", (int)dht22_data[4]);
					
    delay_us(50);
		
		for(i=0; i<4; i++)
		{
					checksum += dht22_data[i];   
		}			
    if(checksum == dht22_data[4])
    {
						checked_data[0]  = dht22_data[0];
            checked_data[1]  = dht22_data[1];
            checked_data[2]  = dht22_data[2];
            checked_data[3]  = dht22_data[3];
            checked_data[4]  = dht22_data[4];
						printf("checked_data[0]:");
						printf("%d！\r\n", (int)checked_data[0]);
						printf("checked_data[1]:");
						printf("%d！\r\n", (int)checked_data[1]);
						printf("checked_data[2]:");
						printf("%d！\r\n", (int)checked_data[2]);
						printf("checked_data[3]:");
						printf("%d！\r\n", (int)checked_data[3]);
						printf("checked_data[4]:");
						printf("%d！\r\n", (int)checked_data[4]);
		}

	}else{
		printf("DHT22 start signal do not finish   ");
	}
	
	
  		
				
			

}
 
