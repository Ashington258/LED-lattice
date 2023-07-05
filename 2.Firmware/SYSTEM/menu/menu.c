#include "system.h"
#include "key.h"
#include "usart.h"
#include "ds3231.h"
#include "BME280.h"
#include "MPU6050.h"
#include "max7219.h"
#include "key.h"
#include "menu.h"
#include "mpu6050suanfa.h" 
#include "delay.h"

//���嵱ǰ��ʾ�Ĳ˵������͹��λ�ñ���
uint8_t currentMenuIndex = 0; //��ǰ��ʾ�Ĳ˵�������Ĭ��Ϊ���˵�
uint8_t cursorPos = 0; //���λ�ã�Ĭ��Ϊ��һ��



//����˵��ṹ��
typedef struct
{
    char *name; //�˵�����
	u8 MenuNum;
    uint8_t subMenuNum; //�Ӳ˵�����
    uint8_t subMenuIndex[10]; //�Ӳ˵�����
    uint8_t parentIndex; //���˵�����
    void (*func)(void); //�˵����ܺ���
} Menu;





//����ȫ�����飬������в˵��Ľṹ��
Menu menu[10] = {
    {"���˵�",0, 2, {1,2,3,4,5}, 0, Max7219_showtime}, //��0���˵������˵�
    {"�Ӳ˵�1-1",1, 0, {1}, 1, Max7219_showtemp}, //��3���˵����Ӳ˵�1-1���й��ܺ���
	{"�Ӳ˵�1-1",2, 0, {2}, 1, Max7219_showPressure}, //��4���˵����Ӳ˵�1-1���й��ܺ���
	{"�Ӳ˵�1-1",3, 0, {3}, 1, max7219showtianqi}, //��4���˵����Ӳ˵�1-1���й��ܺ���
	{"�Ӳ˵�1-1",4, 0, {5}, 1, Max7219_2},    //bell
	{"�Ӳ˵�2-1",5, 0, {5}, 4, Max7219_showclock}, //��5���˵����Ӳ˵�2-1���й��ܺ���
};


void displayMenu(void)
{
	
	uint8_t MenuNum=menu[currentMenuIndex].MenuNum;
	menu[MenuNum].func();
	delay_ms(500);
	if (MenuNum==5)
	{
	menu[currentMenuIndex].MenuNum=3;
	}
}


/***************************************************/
//���尴��������
void keyProcess(void)
{
	u8 x;
	x=getmpu6050_fleg();
	if(Key_GetNum0()==1|x==3)
	{
		if(menu[currentMenuIndex].MenuNum ==0)
			menu[currentMenuIndex].MenuNum=4;
		menu[currentMenuIndex].MenuNum--;   //���Ϸ�
		
			
	}
	if(Key_GetNum1()==1|x==4)
	{
		if(menu[currentMenuIndex].MenuNum ==4)
			menu[currentMenuIndex].MenuNum=0;
		menu[currentMenuIndex].MenuNum++;   //���·�
	}
	if(Key_GetNum2()==1)					//ȷ��
	{
		menu[currentMenuIndex].MenuNum=menu[currentMenuIndex].subMenuIndex[4];
		
	}
	if(Key_GetNum3()==1)					//����
	{
		menu[currentMenuIndex].MenuNum=menu[currentMenuIndex].parentIndex;
	}
	displayMenu(); //ˢ����ʾ��ǰ�˵�
}































































































