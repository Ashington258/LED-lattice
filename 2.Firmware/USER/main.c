#include "stm32f10x.h"                  // Device header
#include "delay.h"
#include "sys.h"
#include "oled.h"
#include "bmp.h"
#include "iic.h"
#include "usart.h"
#include "ds3231.h"
#include "BME280.h"
#include "MPU6050.h"
#include "max7219.h"
#include "key.h"
#include "menu.h"
#include "mpu6050suanfa.h" 


_calendar_part calendar;//����ʱ�ӽṹ��
_Alarmclock_part Alarmclock;//�������ӽṹ��
_Alarm_flag flag;



void Init_All()
{	
	u8 x;
	delay_init();	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //�ж����ȼ����� ��2��
	
	IIC_Init();
	OLED_Init();			//OLED��ʼ��
	DS3231_Init();			//DS3231��ʼ��
	OLED_Init();
	Bmp_Init();
	x=MPU_Init();
	KEY_Init();
	Max7219_GPIO_Init();
	Init_MAX7219();
	MPU6050_Init();
	
	uart_init(115200);

	OLED_ColorTurn(0);//0������ʾ��1 ��ɫ��ʾ
	OLED_DisplayTurn(0);//0������ʾ 1 ��Ļ��ת��ʾ
}



int main(void)
{	
	Init_All();
	gettianqi();
	Max7219_showtime();


/**************///��Ҫɾ��!!!!!!!!!!!!!!!!!!!!!!!!!!	
//	ModifyTime(23,5,26,8,45,00);// ////������һ��,��¼,�����κ�����¼һ��,����ʹ�ϵ�������ʱ�䲻�����¿�ʼ
/****************/
	while(1)
	{	
		
		keyProcess();
		Bellmove();
	}
}


