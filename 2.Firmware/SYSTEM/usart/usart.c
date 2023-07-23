#include "sys.h"
#include "usart.h"	  
#include "delay.h"



u8 n=7,tianqi_n=0;
u8 len;	
u16 t_oled; 
u8 tianqi[20]={0};



////////////////////////////////////////////////////////////////////////////////// 	 
//如果使用ucos,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos 使用	  
#endif
//////////////////////////////////////////////////////////////////////////////////	 

//支持适应不同频率下的串口波特率设置.
//加入了对printf的支持
//增加了串口接收命令功能.
//修正了printf第一个字符丢失的bug
//V1.4修改说明
//1,修改串口初始化IO的bug
//2,修改了USART_RX_STA,使得串口最大接收字节数为2的14次方
//3,增加了USART_REC_LEN,用于定义串口最大允许接收的字节数(不大于2的14次方)
//4,修改了EN_USART1_RX的使能方式
//V1.5修改说明
//1,增加了对UCOSII的支持
////////////////////////////////////////////////////////////////////////////////// 	  
 

//////////////////////////////////////////////////////////////////
/*************************************************************************/
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 

/***************************************************************************/
 
#if EN_USART1_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART_RX_STA=0;       //接收状态标记	  
  
void uart_init(u32 bound){
  //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
  
	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.9
   
  //USART1_RX	  GPIOA.10初始化
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.10  

  //Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
   //USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

  USART_Init(USART1, &USART_InitStructure); //初始化串口1
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启串口接受中断
  USART_Cmd(USART1, ENABLE);                    //使能串口1 

}


void gettianqi(void)
{
	u8 t;
	printf("AT\r\n");  
	delay_ms(100);
	printf("AT+CIPSTART=\"TCP\",\"api.seniverse.com\",80\r\n");  
	delay_ms(100);
	printf("AT+CIPMODE=1\r\n");  
	delay_ms(100);
	printf("AT+CIPSEND\r\n");  
	delay_ms(100);
	printf("GET https://api.seniverse.com/v3/weather/now.json?key=Sw9ZH_ertLWhhY0qw&location=wuhan&language=en&unit=c\r\n");  
	delay_ms(100);	

	if(USART_RX_STA&0x8000)
	{					   
		len=USART_RX_STA&0x3fff;//得到此次接收2到的数据长度
		for(t=0;t<len;t++)
		{
			USART1->DR=USART_RX_BUF[t];
			while((USART1->SR&0X40)==0);//等待发送结束
		}
		USART_RX_STA=0;
	}
	for (t_oled=200;t_oled<350;t_oled++)//显示天气
	{
	if(USART_RX_BUF[t_oled]==0x74)
		if(USART_RX_BUF[t_oled+1]==0x65)
			if(USART_RX_BUF[t_oled+2]==0x78)
				if(USART_RX_BUF[t_oled+3]==0x74)
				{
					for (;USART_RX_BUF[t_oled+n] !=0x22;n++)
					{
//					OLED_Refresh();
//					OLED_ShowChar((n-7)*8,0,USART_RX_BUF[t_oled+n],16,1);
					tianqi[tianqi_n]=USART_RX_BUF[t_oled+n];
					tianqi_n++;
					}
				}
	}
//	for (t_oled=250;t_oled<350;t_oled++)//显示温度
//	{
//	if(USART_RX_BUF[t_oled]==0x74)
//		if(USART_RX_BUF[t_oled+1]==0x75)
//			if(USART_RX_BUF[t_oled+2]==0x72)
//				if(USART_RX_BUF[t_oled+3]==0x65)
//				{
//				OLED_Refresh();
//				OLED_ShowChar(0,16,USART_RX_BUF[t_oled+7],16,1);
//				OLED_ShowChar(8,16,USART_RX_BUF[t_oled+8],16,1);
//					
//				}
//	}
	
}









void USART1_IRQHandler(void)                	//串口1中断服务程序
	{
	u8 Res;
#if SYSTEM_SUPPORT_OS 		//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
		{
		Res =USART_ReceiveData(USART1);	//读取接收到的数据
		USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
		USART_RX_STA++;
//		if((USART_RX_STA&0x8000)==0)//接收未完成
//			{
//			if(USART_RX_STA&0x4000)//接收到了0x0d
//				{
//				if(Res!=0x0a)USART_RX_STA=0;//接收错误,重新开始
//				else USART_RX_STA|=0x8000;	//接收完成了 
//				}
//			else //还没收到0X0D
//				{	
//				if(Res==0x0d)USART_RX_STA|=0x4000;
//				else
//					{

//					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//接收数据错误,重新开始接收	  
//					}		 
//				}
//			}   		 
     } 
#if SYSTEM_SUPPORT_OS 	//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntExit();  											 
#endif
} 
#endif	

