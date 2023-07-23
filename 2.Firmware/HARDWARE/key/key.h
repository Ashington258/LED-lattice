#ifndef _key_H
#define _key_H


#include "system.h"
#include "ds3231.h"

 
#define KEY_LEFT_Pin    GPIO_Pin_13    //����K_LEFT�ܽ�
#define KEY_DOWN_Pin    GPIO_Pin_14   //����K_DOWN�ܽ�
#define KEY_RIGHT_Pin   GPIO_Pin_15   //����K_RIGHT�ܽ�
#define KEY_UP_Pin      GPIO_Pin_0  //����KEY_UP�ܽ�

#define KEY_Port (GPIOC) //����˿�
#define KEY_UP_Port (GPIOA) //����˿�


//ʹ��λ��������
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

//ʹ�ö�ȡ�ܽ�״̬�⺯������ 
//#define K_UP      GPIO_ReadInputDataBit(KEY_UP_Port,KEY_UP_Pin)
//#define K_DOWN    GPIO_ReadInputDataBit(KEY_Port,KEY_DOWN_Pin)
//#define K_LEFT    GPIO_ReadInputDataBit(KEY_Port,KEY_LEFT_Pin)
//#define K_RIGHT   GPIO_ReadInputDataBit(KEY_Port,KEY_RIGHT_Pin)


//�����������ֵ  

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


