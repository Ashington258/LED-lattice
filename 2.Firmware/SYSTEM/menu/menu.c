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

//定义当前显示的菜单索引和光标位置变量
uint8_t currentMenuIndex = 0; //当前显示的菜单索引，默认为主菜单
uint8_t cursorPos = 0; //光标位置，默认为第一行



//定义菜单结构体
typedef struct
{
    char *name; //菜单名称
	u8 MenuNum;
    uint8_t subMenuNum; //子菜单数量
    uint8_t subMenuIndex[10]; //子菜单索引
    uint8_t parentIndex; //父菜单索引
    void (*func)(void); //菜单功能函数
} Menu;





//定义全局数组，存放所有菜单的结构体
Menu menu[10] = {
    {"主菜单",0, 2, {1,2,3,4,5}, 0, Max7219_showtime}, //第0个菜单，主菜单
    {"子菜单1-1",1, 0, {1}, 1, Max7219_showtemp}, //第3个菜单，子菜单1-1，有功能函数
	{"子菜单1-1",2, 0, {2}, 1, Max7219_showPressure}, //第4个菜单，子菜单1-1，有功能函数
	{"子菜单1-1",3, 0, {3}, 1, max7219showtianqi}, //第4个菜单，子菜单1-1，有功能函数
	{"子菜单1-1",4, 0, {5}, 1, Max7219_2},    //bell
	{"子菜单2-1",5, 0, {5}, 4, Max7219_showclock}, //第5个菜单，子菜单2-1，有功能函数
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
//定义按键处理函数
void keyProcess(void)
{
	u8 x;
	x=getmpu6050_fleg();
	if(Key_GetNum0()==1|x==3)
	{
		if(menu[currentMenuIndex].MenuNum ==0)
			menu[currentMenuIndex].MenuNum=4;
		menu[currentMenuIndex].MenuNum--;   //向上翻
		
			
	}
	if(Key_GetNum1()==1|x==4)
	{
		if(menu[currentMenuIndex].MenuNum ==4)
			menu[currentMenuIndex].MenuNum=0;
		menu[currentMenuIndex].MenuNum++;   //向下翻
	}
	if(Key_GetNum2()==1)					//确定
	{
		menu[currentMenuIndex].MenuNum=menu[currentMenuIndex].subMenuIndex[4];
		
	}
	if(Key_GetNum3()==1)					//返回
	{
		menu[currentMenuIndex].MenuNum=menu[currentMenuIndex].parentIndex;
	}
	displayMenu(); //刷新显示当前菜单
}































































































