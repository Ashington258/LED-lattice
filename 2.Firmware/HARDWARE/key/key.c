#include "key.h"
#include "delay.h"



/*******************************************************************************
* �� �� ��         : KEY_Init
* ��������		   : ������ʼ��
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void KEY_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; //����ṹ�����	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOE,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=KEY_UP_Pin;	   //ѡ����Ҫ���õ�IO��
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;//��������  
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	   //���ô�������
	GPIO_Init(KEY_UP_Port,&GPIO_InitStructure);		  /* ��ʼ��GPIO */
	
	GPIO_InitStructure.GPIO_Pin=KEY_DOWN_Pin|KEY_LEFT_Pin|KEY_RIGHT_Pin;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;	//��������
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
* �� �� ��         : KEY_Scan
* ��������		   : ����ɨ����
* ��    ��         : mode=0:���ΰ��°���
					 mode=1���������°���
* ��    ��         : 0��δ�а�������
					 KEY_UP��K_UP������
					 KEY_DOWN��K_DOWN������
					 KEY_LEFT��K_LEFT������
					 KEY_RIGHT��K_RIGHT������
*******************************************************************************/
u8 KEY_Scan(u8 mode)
{
	static u8 key=1;
	if(key==1&&(K_UP==0||K_DOWN==0||K_LEFT==0||K_RIGHT==0)) //����һ����������
	{
		delay_ms(10);  //����
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
	else if(K_UP==1&&K_DOWN==1&&K_LEFT==1&&K_RIGHT==1)    //�ް�������
	{
		key=1;
	}
	if(mode==1) //������������
	{
		key=1;
	}
	return 0;
}


void Key_Control(void)//��������
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
//	if(key==1&&(K_0==0||K_1==0||K_2==0||K_3==0)) //����һ����������
//	{
//		delay_ms(10);  //����
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
//	else if(K_UP==1&&K_DOWN==1&&K_LEFT==1&&K_RIGHT==1)    //�ް�������
//	{
//		key=1;
//	}
//	if(mode==1) //������������
//	{
//		key=1;
//	}
//	return 0;
//}














