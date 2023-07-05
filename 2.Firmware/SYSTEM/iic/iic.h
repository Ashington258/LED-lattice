#ifndef __IIC_H
#define __IIC_H
#include "sys.h"
#include "stm32f10x.h"  
/*****************/
////IO输入输出方向设置，操作CRL寄存器
#define SDA_IN()  {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)8<<28;}
#define SDA_OUT() {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)3<<28;}

/***************************/

//设置IIC数据线和时钟线的引脚
#define IIC_SCL    PBout(6) 		//SCL
#define IIC_SDA    PBout(7) 		//SDA	 
#define READ_SDA   PBin(7) 	 		//SDA 



//IIC操作函数
void IIC_Init(void);                //初始化IIC的IO口 
void IIC_Start(void);				//发送IIC开始信号
void IIC_Stop(void);	  			//发送IIC停止信号
void IIC_Send_Byte(u8 txd);	//发送一个字节数据		
u8 IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
u8 IIC_Wait_Ack(void); 	//IIC等待应答信号
void IIC_Ack(void);					//IIC产生应答信号
void IIC_NAck(void);			//IIC产生非应答信号 
#endif
