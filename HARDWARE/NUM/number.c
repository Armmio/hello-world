/*
将数字填充成字符串
Sun Zixiong 2019.6
@zet6
*/

#include "number.h"


void Show_number(float* data, u8* str)
{
	int i = 0;
	int Data[7] = {0, 0, 0, 0, 0, 0, 0};
	int i_1, i_2;
	float decimal, d_2;
	d_2 = *data;
	
	//将数字组合成对应的字符串 111.00
	if(d_2 >= 0)
	{
		i_1 = (int)d_2;
		decimal = (d_2 - i_1)*100;
		i_2 = (int)decimal;
		str[0] = ' ';
		Data[1] = (int) i_1 / 100;
		Data[2] = (int) i_1 / 10;
		Data[3] = (int) i_1 % 10;
		str[4] = '.';
		Data[5] = (int) i_2 / 10;
		Data[6] = (int) i_2 % 10;
		
		for(i=1; i<=3; i++)
		{
			if(Data[i] == 0)
			{
				str[i] = '0';
			}else if(Data[i] == 1){
				str[i] = '1';
			}else if(Data[i] == 2){
				str[i] = '2';
			}else if(Data[i] == 3){
				str[i] = '3';
			}else if(Data[i] == 4){
				str[i] = '4';
			}else if(Data[i] == 5){
				str[i] = '5';
			}else if(Data[i] == 6){
				str[i] = '6';
			}else if(Data[i] == 7){
				str[i] = '7';
			}else if(Data[i] == 8){
				str[i] = '8';
			}else if(Data[i] == 9){
				str[i] = '9';
			}
		}
		for(i=5; i<=6; i++)
		{
			if(Data[i] == 0)
			{
				str[i] = '0';
			}else if(Data[i] == 1){
				str[i] = '1';
			}else if(Data[i] == 2){
				str[i] = '2';
			}else if(Data[i] == 3){
				str[i] = '3';
			}else if(Data[i] == 4){
				str[i] = '4';
			}else if(Data[i] == 5){
				str[i] = '5';
			}else if(Data[i] == 6){
				str[i] = '6';
			}else if(Data[i] == 7){
				str[i] = '7';
			}else if(Data[i] == 8){
				str[i] = '8';
			}else if(Data[i] == 9){
				str[i] = '9';
			}
		}
		
	}else{
		i_1 = (int)d_2;
		decimal = (d_2 - i_1)*100;
		i_2 = (int)decimal;
		str[0] = '-';
		Data[1] = (int) i_1 / 100;
		Data[2] = (int) i_1 / 10;
		Data[3] = (int) i_1 % 10;
		str[4] = '.';
		Data[5] = (int) i_2 / 10;
		Data[6] = (int) i_2 % 10;
		
		for(i=1; i<=3; i++)
		{
			if(Data[i] == 0)
			{
				str[i] = '0';
			}else if(Data[i] == 1){
				str[i] = '1';
			}else if(Data[i] == 2){
				str[i] = '2';
			}else if(Data[i] == 3){
				str[i] = '3';
			}else if(Data[i] == 4){
				str[i] = '4';
			}else if(Data[i] == 5){
				str[i] = '5';
			}else if(Data[i] == 6){
				str[i] = '6';
			}else if(Data[i] == 7){
				str[i] = '7';
			}else if(Data[i] == 8){
				str[i] = '8';
			}else if(Data[i] == 9){
				str[i] = '9';
			}
		}
		for(i=5; i<=6; i++)
		{
			if(Data[i] == 0)
			{
				str[i] = '0';
			}else if(Data[i] == 1){
				str[i] = '1';
			}else if(Data[i] == 2){
				str[i] = '2';
			}else if(Data[i] == 3){
				str[i] = '3';
			}else if(Data[i] == 4){
				str[i] = '4';
			}else if(Data[i] == 5){
				str[i] = '5';
			}else if(Data[i] == 6){
				str[i] = '6';
			}else if(Data[i] == 7){
				str[i] = '7';
			}else if(Data[i] == 8){
				str[i] = '8';
			}else if(Data[i] == 9){
				str[i] = '9';
			}
		}
	}
}




void Trans_number(int* data, u8* str)
{
	int i = 0;
	int Data[4] = {0, 0, 0, 0};
	int i_1 = 0;
	float decimal, d_2;
	d_2 = *data;
	
	if(d_2 >= 0)
	{
		str[0] = ' ';
		Data[1] = (int) d_2 / 100;
		Data[2] = (int) d_2 / 10;
		Data[3] = (int) d_2 % 10;
		
		for(i=1; i<=3; i++)
		{
			if(Data[i] == 0)
			{
				str[i] = '0';
			}else if(Data[i] == 1){
				str[i] = '1';
			}else if(Data[i] == 2){
				str[i] = '2';
			}else if(Data[i] == 3){
				str[i] = '3';
			}else if(Data[i] == 4){
				str[i] = '4';
			}else if(Data[i] == 5){
				str[i] = '5';
			}else if(Data[i] == 6){
				str[i] = '6';
			}else if(Data[i] == 7){
				str[i] = '7';
			}else if(Data[i] == 8){
				str[i] = '8';
			}else if(Data[i] == 9){
				str[i] = '9';
			}
		}
		
	}else{
		str[0] = '-';
		Data[1] = (int) d_2 / 100;
		Data[2] = (int) d_2 / 10;
		Data[3] = (int) d_2 % 10;
		
		for(i=1; i<=3; i++)
		{
			if(Data[i] == 0)
			{
				str[i] = '0';
			}else if(Data[i] == 1){
				str[i] = '1';
			}else if(Data[i] == 2){
				str[i] = '2';
			}else if(Data[i] == 3){
				str[i] = '3';
			}else if(Data[i] == 4){
				str[i] = '4';
			}else if(Data[i] == 5){
				str[i] = '5';
			}else if(Data[i] == 6){
				str[i] = '6';
			}else if(Data[i] == 7){
				str[i] = '7';
			}else if(Data[i] == 8){
				str[i] = '8';
			}else if(Data[i] == 9){
				str[i] = '9';
			}
		}
	}
}



//void Show_number(float* data, u8* str)
//{
//	int i = 0;
//	char Te[7] = {'0', '0', '0', '0', '0', '0', '0'};
//	int Data[7] = {0, 0, 0, 0, 0, 0, 0};
//	int i_1, i_2;
//	float decimal, d_2;
//	d_2 = *data;
//	
//	//将数字组合成对应的字符串 111.00
//	if(d_2 >= 0)
//	{
//		i_1 = (int)d_2;
//		decimal = (d_2 - i_1)*100;
//		i_2 = (int)decimal;
//		Te[0] = ' ';
//		Data[1] = (int) i_1 / 100;
//		Data[2] = (int) i_1 / 10;
//		Data[3] = (int) i_1 % 10;
//		Te[4] = '.';
//		Data[5] = (int) i_2 / 10;
//		Data[6] = (int) i_2 % 10;
//		
//		for(i=1; i<=3; i++)
//		{
//			if(Data[i] == 0)
//			{
//				Te[i] = '0';
//			}else if(Data[i] == 1){
//				Te[i] = '1';
//			}else if(Data[i] == 2){
//				Te[i] = '2';
//			}else if(Data[i] == 3){
//				Te[i] = '3';
//			}else if(Data[i] == 4){
//				Te[i] = '4';
//			}else if(Data[i] == 5){
//				Te[i] = '5';
//			}else if(Data[i] == 6){
//				Te[i] = '6';
//			}else if(Data[i] == 7){
//				Te[i] = '7';
//			}else if(Data[i] == 8){
//				Te[i] = '8';
//			}else if(Data[i] == 9){
//				Te[i] = '9';
//			}
//		}
//		for(i=5; i<=6; i++)
//		{
//			if(Data[i] == 0)
//			{
//				Te[i] = '0';
//			}else if(Data[i] == 1){
//				Te[i] = '1';
//			}else if(Data[i] == 2){
//				Te[i] = '2';
//			}else if(Data[i] == 3){
//				Te[i] = '3';
//			}else if(Data[i] == 4){
//				Te[i] = '4';
//			}else if(Data[i] == 5){
//				Te[i] = '5';
//			}else if(Data[i] == 6){
//				Te[i] = '6';
//			}else if(Data[i] == 7){
//				Te[i] = '7';
//			}else if(Data[i] == 8){
//				Te[i] = '8';
//			}else if(Data[i] == 9){
//				Te[i] = '9';
//			}
//		}
//		
//	}else{
//		i_1 = (int)d_2;
//		decimal = (d_2 - i_1)*100;
//		i_2 = (int)decimal;
//		Te[0] = '-';
//		Data[1] = (int) i_1 / 100;
//		Data[2] = (int) i_1 / 10;
//		Data[3] = (int) i_1 % 10;
//		Te[4] = '.';
//		Data[5] = (int) i_2 / 10;
//		Data[6] = (int) i_2 % 10;
//		
//		for(i=1; i<=3; i++)
//		{
//			if(Data[i] == 0)
//			{
//				Te[i] = '0';
//			}else if(Data[i] == 1){
//				Te[i] = '1';
//			}else if(Data[i] == 2){
//				Te[i] = '2';
//			}else if(Data[i] == 3){
//				Te[i] = '3';
//			}else if(Data[i] == 4){
//				Te[i] = '4';
//			}else if(Data[i] == 5){
//				Te[i] = '5';
//			}else if(Data[i] == 6){
//				Te[i] = '6';
//			}else if(Data[i] == 7){
//				Te[i] = '7';
//			}else if(Data[i] == 8){
//				Te[i] = '8';
//			}else if(Data[i] == 9){
//				Te[i] = '9';
//			}
//		}
//		for(i=5; i<=6; i++)
//		{
//			if(Data[i] == 0)
//			{
//				Te[i] = '0';
//			}else if(Data[i] == 1){
//				Te[i] = '1';
//			}else if(Data[i] == 2){
//				Te[i] = '2';
//			}else if(Data[i] == 3){
//				Te[i] = '3';
//			}else if(Data[i] == 4){
//				Te[i] = '4';
//			}else if(Data[i] == 5){
//				Te[i] = '5';
//			}else if(Data[i] == 6){
//				Te[i] = '6';
//			}else if(Data[i] == 7){
//				Te[i] = '7';
//			}else if(Data[i] == 8){
//				Te[i] = '8';
//			}else if(Data[i] == 9){
//				Te[i] = '9';
//			}
//		}
//	}
// str = Te;
//}

//int Bit_select(int i)
//{
//	int j = 0;
//	if(i == 0)
//	{}else if(){
//		j = 0;
//		return j;
//	}
//	else if(i == 1){
//		j = 1;
//		return j;
//	}else if(i == 2){
//		j = 2;
//		return j;
//	}else if(i == 3){
//		j = 3;
//		return j;
//	}else if(i == 4){
//		j = 4;
//		return j;
//	}else if(i == 5){
//		j = 5;
//		return j;
//	}else if(i == 6){
//		j = 6;
//		return j;
//	}else if(i == 7){
//		j = 7;
//		return j;
//	}else if(i == 8){
//		j = 8;
//		return j;
//	}else if(i == 9){
//		j = 9;
//		return j;
//	}
//}

