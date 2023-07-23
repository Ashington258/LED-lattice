#ifndef __MAX7219_H
#define __MAX7219_H

#include "stm32f10x.h"
#include "sys.h"

#define SPI_IS_HARDWARE 1  //1：硬件SPI；0：软件模拟SPI

#define Max7219_pinDIN_GPIO GPIO_Pin_7
#define Max7219_pinCS_GPIO  GPIO_Pin_3
#define Max7219_pinCLK_GPIO GPIO_Pin_5
//#define SPI_MISO_GPIO       GPIO_Pin_6//未使用到

#define Max7219_pinDIN PAout(7)
#define Max7219_pinCS  PAout(3)
#define Max7219_pinCLK PAout(5)

#define uchar unsigned char

void Max7219_GPIO_Init(void);
void Write_Max7219_byte(u8 data);
void Write_Max7219(u8 addr,u8 dat);
void Init_MAX7219(void);
void Write_Max7219_1(uchar add1,uchar dat1,uchar dat2);
void Write_Max7219_2(uchar add2,uchar dat1,uchar dat2);
void Write_Max7219_3(uchar add3,uchar dat1,uchar dat2);
void Write_Max7219_4(uchar add4,uchar dat1,uchar dat2);
void Max7219_showtime();
void Max7219_showchar(u8 x1,u8 x2,u8 x3,u8 x4);
void Max7219_showclock();
void Bellmove();
void Max7219_1();
void Max7219_2();
void max7219showtianqi(void);


















#endif
