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


_calendar_part calendar;//声明时钟结构体
_Alarmclock_part Alarmclock;//声明闹钟结构体
_Alarm_flag flag;



void Init_All()
{	
	u8 x;
	delay_init();	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //中断优先级分组 分2组
	
	IIC_Init();
	OLED_Init();			//OLED初始化
	DS3231_Init();			//DS3231初始化
	OLED_Init();
	Bmp_Init();
	x=MPU_Init();
	KEY_Init();
	Max7219_GPIO_Init();
	Init_MAX7219();
	MPU6050_Init();
	
	uart_init(115200);

	OLED_ColorTurn(0);//0正常显示，1 反色显示
	OLED_DisplayTurn(0);//0正常显示 1 屏幕翻转显示
}



int main(void)
{	
	Init_All();
	gettianqi();
	Max7219_showtime();


/**************///不要删了!!!!!!!!!!!!!!!!!!!!!!!!!!	
//	ModifyTime(23,5,26,8,45,00);// ////先设置一次,烧录,再屏蔽后再烧录一次,才能使断电重启后时间不会重新开始
/****************/
	while(1)
	{	
		
		keyProcess();
		Bellmove();
	}
}


