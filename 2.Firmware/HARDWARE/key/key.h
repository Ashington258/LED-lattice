#ifndef _key_H
#define _key_H


#include "system.h"
#include "ds3231.h"

 
#define KEY_LEFT_Pin    GPIO_Pin_13    //定义K_LEFT管脚
#define KEY_DOWN_Pin    GPIO_Pin_14   //定义K_DOWN管脚
#define KEY_RIGHT_Pin   GPIO_Pin_15   //定义K_RIGHT管脚
#define KEY_UP_Pin      GPIO_Pin_0  //定义KEY_UP管脚

#define KEY_Port (GPIOC) //定义端口
#define KEY_UP_Port (GPIOA) //定义端口


//使用位操作定义
#define K_UP PAin(15)
#define K_DOWN PCin(13)
#define K_LEFT PCin(14)
#define K_RIGHT PCin(15)

#define K_0 PAin(0)
#define K_1 PCin(1)
#define K_3 PCin(2)
#define K_4 PCin(3)


//#define K_Sure 	K_UP
//#define K_Hour 	K_DOWN
//#define K_Min	 	K_LEFT
//#define	K_Sec		K_RIGHT

//使用读取管脚状态库函数定义 
//#define K_UP      GPIO_ReadInputDataBit(KEY_UP_Port,KEY_UP_Pin)
//#define K_DOWN    GPIO_ReadInputDataBit(KEY_Port,KEY_DOWN_Pin)
//#define K_LEFT    GPIO_ReadInputDataBit(KEY_Port,KEY_LEFT_Pin)
//#define K_RIGHT   GPIO_ReadInputDataBit(KEY_Port,KEY_RIGHT_Pin)


//定义各个按键值  

#define KEY_DOWN 1
#define KEY_LEFT 2
#define KEY_RIGHT 3  
#define KEY_UP 4


void KEY_Init(void);
u8 KEY_Scan(u8 mode);
uint8_t Key_GetNum0(void);
uint8_t Key_GetNum1(void);
uint8_t Key_GetNum2(void);
uint8_t Key_GetNum3(void);


#endif


