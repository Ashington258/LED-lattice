#include "iic.h"
#include "delay.h"
//IIC��ʼ��
void IIC_Init(void)
{					     

	GPIO_InitTypeDef  GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //ʹ��B�˿�ʱ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;	 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//�ٶ�50MHz
 	GPIO_Init(GPIOB, &GPIO_InitStructure);	  //��ʼ��PA0,1

	
}
//����IIC��ʼ�ź�
//IIC��ʼ�źŲ���������Ϊ��SCLΪ�ߵ�ƽʱ��SDA��Ϊ�͵�ƽ
void IIC_Start(void)
{
	SDA_OUT();     //����SDAΪ���ģʽ
	IIC_SDA=1;	  	//���ó�ʼ״̬��Ϊ�ߵ�ƽ  
	IIC_SCL=1;
	delay_us(4);
 	IIC_SDA=0;//��ʼ�źţ�SDA�ɸ߱��
	delay_us(4);
	IIC_SCL=0; //ǯסI2C���ߣ�׼�����ͻ��������
}	  
//����IICֹͣ�ź�
//����ֹͣ�źŵ�����Ϊ��SCLΪ�ߵ�ƽʱ��SDA�ɵͱ��
void IIC_Stop(void)
{
	SDA_OUT();//SDA����Ϊ���
	IIC_SCL=0;
	IIC_SDA=0;//��ʼ���ǵ͵�ƽ
 	delay_us(4);
	IIC_SCL=1; //SCL��Ϊ�ߵ�ƽ
	IIC_SDA=1;//SDA�ɵ͵�ƽת��Ϊ�ߵ�ƽ����ֹͣ�ź�
	delay_us(4);							   	
}

//IIC���豸����һ��������ɺ󣬴��豸����Ӧ���źţ����豸�ȴ�Ӧ���źŵ���
//����������SCLΪ�ߵ�ƽ�ڼ䣬SDAʱ�ӱ��ֵ͵�ƽ��
//����ֵ��1������Ӧ��ʧ�ܣ�0������Ӧ��ɹ�
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();      //SDA����Ϊ����
	IIC_SDA=1;delay_us(1);	 //�տ�ʼ��Ϊ�ߵ�ƽ
	IIC_SCL=1;delay_us(1);	 
	while(READ_SDA)//��ȡ������SDA�ĵ�ƽ״̬����������͵�ƽ���򲻻����IIC_Stop�źţ�����0
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();//�����SCL�ߵ�ƽ�ڼ䣬SDA�ź��߲�����һ��ʱ��ĸߵ�ƽ����ΪӦ��ʧ��
			return 1;
		}
	}
	IIC_SCL=0;//Ӧ�������ʱ�����0
	return 0;  
} 
//����ACKӦ���ź�
//��������Ϊ��SCLΪ�ߵ�ƽ�ڼ䣬SDAʼ�ձ��ֵ͵�ƽ
void IIC_Ack(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=0;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}
//������Ӧ���ź�
//��������Ϊ��SCLΪ�ߵ�ƽ�ڼ䣬SDAҲ�����˸ߵ�ƽ  
void IIC_NAck(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=1;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}					 				     
//IIC����һ���ֽ�  
//��������Ϊ��SCLΪ�͵�ƽ�ڼ�׼�������ݣ�SCLΪ�ߵ�ƽ�ڼ䱣������
void IIC_Send_Byte(u8 txd)

{                        
  u8 t;   
	SDA_OUT(); 	  //SDA����Ϊ���
    IIC_SCL=0;//����ʱ��׼������
    for(t=0;t<8;t++)
    {              
      if((txd&0x80)>>7) //�����ݵ����λ��ʼ����
      		IIC_SDA=1;	//���Ϊ1��������λΪ1
      	else IIC_SDA=0; //��Ϊ1������λΪ0
        txd<<=1; 	  //�������
		delay_us(2);   
		IIC_SCL=1;
		delay_us(2); 
		IIC_SCL=0;	
		delay_us(2);
    }	 
} 	    
//��һ���ֽڣ�ack=1ʱ������ACK,ack=0������nACK
//��ȡ����Ϊ��SCLΪ�ߵ�ƽ�ڼ䣬��ȡSDA�ĵ�ƽ״̬
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//����SDAΪ����
    for(i=0;i<8;i++ ) //�����8λ
	{
        IIC_SCL=0; 
        delay_us(2);
		IIC_SCL=1; //SCLΪ�ߵ�ƽ
        receive<<=1; //����ƶ�����λ
        if(READ_SDA)receive++;   //���SDAΪ�ߣ�����Ӧ������Ϊ+1����֮Ϊ0
		delay_us(1); 
    }					 
    if (!ack)
        IIC_NAck();//������ACKӦ��
    else
        IIC_Ack(); //����ACKӦ��
    return receive;
}
