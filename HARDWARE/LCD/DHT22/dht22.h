#ifndef __DHT22_H
#define __DHT22_H	 
//#include "sys.h"
#include "stm32f10x.h"

#define SDA GPIO_Pin_7


void SetAsOutput(void);
void SetAsIutput(void);
char Read_Data(void);
void DataRead(void);

		 				    
#endif
