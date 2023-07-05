#include "key.h"
#include "delay.h"



/*******************************************************************************
* 函 数 名         : KEY_Init
* 函数功能		   : 按键初始化
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void KEY_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; //定义结构体变量	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOE,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=KEY_UP_Pin;	   //选择你要设置的IO口
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;//下拉输入  
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	   //设置传输速率
	GPIO_Init(KEY_UP_Port,&GPIO_InitStructure);		  /* 初始化GPIO */
	
	GPIO_InitStructure.GPIO_Pin=KEY_DOWN_Pin|KEY_LEFT_Pin|KEY_RIGHT_Pin;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;	//上拉输入
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(KEY_Port,&GPIO_InitStructure);
	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_0|GPIO_Pin_2 |GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOA,GPIO_Pin_1 | GPIO_Pin_0|GPIO_Pin_2 |GPIO_Pin_4);
	

	
}

/*******************************************************************************
* 函 数 名         : KEY_Scan
* 函数功能		   : 按键扫描检测
* 输    入         : mode=0:单次按下按键
					 mode=1：连续按下按键
* 输    出         : 0：未有按键按下
					 KEY_UP：K_UP键按下
					 KEY_DOWN：K_DOWN键按下
					 KEY_LEFT：K_LEFT键按下
					 KEY_RIGHT：K_RIGHT键按下
*******************************************************************************/
u8 KEY_Scan(u8 mode)
{
	static u8 key=1;
	if(key==1&&(K_UP==0||K_DOWN==0||K_LEFT==0||K_RIGHT==0)) //任意一个按键按下
	{
		delay_ms(10);  //消抖
		key=0;
		if(K_UP==0)
		{
			return KEY_UP; 
		}
		else if(K_DOWN==0)
		{
			return KEY_DOWN; 
		}
		else if(K_LEFT==0)
		{
			return KEY_LEFT; 
		}
		else
		{
			return KEY_RIGHT;
		}
	}
	else if(K_UP==1&&K_DOWN==1&&K_LEFT==1&&K_RIGHT==1)    //无按键按下
	{
		key=1;
	}
	if(mode==1) //连续按键按下
	{
		key=1;
	}
	return 0;
}


void Key_Control(void)//按键控制
{
	u8 temp = 0;
	temp =	KEY_Scan(0);
	switch(temp)
	{
		case 1:	Alarmclock.hour+=1;
						if(Alarmclock.hour>=24) Alarmclock.hour=0;
						break;
		case 2: Alarmclock.min+=1;
						if(Alarmclock.min>=60) Alarmclock.min=0;
						break;
		case 3: Alarmclock.sec+=1;
						if(Alarmclock.sec>=60) Alarmclock.sec=0;
						break;
		case 4: flag.sure_flag =~ flag.sure_flag;
						break;
	}

}



uint8_t Key_GetNum0(void)
{
	uint8_t KeyNum = 0;
	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 0)
	{
		delay_ms(2);
		while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 0);
		delay_ms(2);
		KeyNum = 1;
	}
	
	return KeyNum;
}

uint8_t Key_GetNum1(void)
{
	uint8_t KeyNum = 0;
	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) == 0)
	{
		delay_ms(2);
		while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) == 0);
		delay_ms(2);
		KeyNum = 1;
	}

	return KeyNum;
}

uint8_t Key_GetNum2(void)
{
	uint8_t KeyNum = 0;
	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2) == 0)
	{
		delay_ms(2);
		while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2) == 0);
		delay_ms(2);
		KeyNum = 1;
	}

	return KeyNum;
}

uint8_t Key_GetNum3(void)
{
	uint8_t KeyNum = 0;
	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4) == 0)
	{
		delay_ms(2);
		while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4) == 0);
		delay_ms(2);
		KeyNum = 1;
	}

	return KeyNum;
}
/**********************************************/




//u8 KEY_get(u8 mode)
//{
//	static u8 key=1;
//	if(key==1&&(K_0==0||K_1==0||K_2==0||K_3==0)) //任意一个按键按下
//	{
//		delay_ms(10);  //消抖
//		key=0;
//		if(K_0==0)
//		{
//			return KEY_UP; 
//		}
//		else if(K_1==0)
//		{
//			return KEY_DOWN; 
//		}
//		else if(K_2==0)
//		{
//			return KEY_LEFT; 
//		}
//		else
//		{
//			return KEY_RIGHT;
//		}
//	}
//	else if(K_UP==1&&K_DOWN==1&&K_LEFT==1&&K_RIGHT==1)    //无按键按下
//	{
//		key=1;
//	}
//	if(mode==1) //连续按键按下
//	{
//		key=1;
//	}
//	return 0;
//}














