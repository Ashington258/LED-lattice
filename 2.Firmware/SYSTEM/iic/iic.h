#ifndef __IIC_H
#define __IIC_H
#include "sys.h"
#include "stm32f10x.h"  
/*****************/
////IO��������������ã�����CRL�Ĵ���
#define SDA_IN()  {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)8<<28;}
#define SDA_OUT() {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)3<<28;}

/***************************/

//����IIC�����ߺ�ʱ���ߵ�����
#define IIC_SCL    PBout(6) 		//SCL
#define IIC_SDA    PBout(7) 		//SDA	 
#define READ_SDA   PBin(7) 	 		//SDA 



//IIC��������
void IIC_Init(void);                //��ʼ��IIC��IO�� 
void IIC_Start(void);				//����IIC��ʼ�ź�
void IIC_Stop(void);	  			//����IICֹͣ�ź�
void IIC_Send_Byte(u8 txd);	//����һ���ֽ�����		
u8 IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
u8 IIC_Wait_Ack(void); 	//IIC�ȴ�Ӧ���ź�
void IIC_Ack(void);					//IIC����Ӧ���ź�
void IIC_NAck(void);			//IIC������Ӧ���ź� 
#endif
