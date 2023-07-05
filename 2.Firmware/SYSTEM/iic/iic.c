#include "iic.h"
#include "delay.h"
//IIC初始化
void IIC_Init(void)
{					     

	GPIO_InitTypeDef  GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //使能B端口时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;	 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz
 	GPIO_Init(GPIOB, &GPIO_InitStructure);	  //初始化PA0,1

	
}
//产生IIC起始信号
//IIC起始信号产生的条件为：SCL为高电平时，SDA变为低电平
void IIC_Start(void)
{
	SDA_OUT();     //设置SDA为输出模式
	IIC_SDA=1;	  	//设置初始状态都为高电平  
	IIC_SCL=1;
	delay_us(4);
 	IIC_SDA=0;//起始信号，SDA由高变低
	delay_us(4);
	IIC_SCL=0; //钳住I2C总线，准备发送或接收数据
}	  
//产生IIC停止信号
//产生停止信号的条件为：SCL为高电平时，SDA由低变高
void IIC_Stop(void)
{
	SDA_OUT();//SDA设置为输出
	IIC_SCL=0;
	IIC_SDA=0;//起始都是低电平
 	delay_us(4);
	IIC_SCL=1; //SCL变为高电平
	IIC_SDA=1;//SDA由低电平转变为高电平产生停止信号
	delay_us(4);							   	
}

//IIC主设备传输一个数据完成后，从设备产生应答信号，主设备等待应答信号到来
//产生条件：SCL为高电平期间，SDA时钟保持低电平。
//返回值：1，接收应答失败；0，接收应答成功
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();      //SDA设置为输入
	IIC_SDA=1;delay_us(1);	 //刚开始都为高电平
	IIC_SCL=1;delay_us(1);	 
	while(READ_SDA)//读取数据线SDA的电平状态，如果持续低电平，则不会产生IIC_Stop信号，返回0
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();//如果在SCL高电平期间，SDA信号线产生了一定时间的高电平则认为应答失败
			return 1;
		}
	}
	IIC_SCL=0;//应答结束，时钟输出0
	return 0;  
} 
//产生ACK应答信号
//产生条件为：SCL为高电平期间，SDA始终保持低电平
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
//产生非应答信号
//产生条件为：SCL为高电平期间，SDA也出现了高电平  
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
//IIC发送一个字节  
//发送条件为：SCL为低电平期间准备好数据，SCL为高电平期间保持数据
void IIC_Send_Byte(u8 txd)

{                        
  u8 t;   
	SDA_OUT(); 	  //SDA设置为输出
    IIC_SCL=0;//拉低时钟准备数据
    for(t=0;t<8;t++)
    {              
      if((txd&0x80)>>7) //从数据的最高位开始传输
      		IIC_SDA=1;	//如果为1，则数据位为1
      	else IIC_SDA=0; //不为1，数据位为0
        txd<<=1; 	  //逐个传输
		delay_us(2);   
		IIC_SCL=1;
		delay_us(2); 
		IIC_SCL=0;	
		delay_us(2);
    }	 
} 	    
//读一个字节，ack=1时，发送ACK,ack=0，发送nACK
//读取条件为：SCL为高电平期间，读取SDA的电平状态
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//设置SDA为输入
    for(i=0;i<8;i++ ) //逐个读8位
	{
        IIC_SCL=0; 
        delay_us(2);
		IIC_SCL=1; //SCL为高电平
        receive<<=1; //逐个移动数据位
        if(READ_SDA)receive++;   //如果SDA为高，则相应的数据为+1，反之为0
		delay_us(1); 
    }					 
    if (!ack)
        IIC_NAck();//不产生ACK应答
    else
        IIC_Ack(); //产生ACK应答
    return receive;
}
