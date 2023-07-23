//#include "Serial.h"
//#include "stm32f10x.h" 
//#include "stdio.h"	
//#include "usart.h"	
//	
//	
//	
//	
//	
//	
//	
//	
//	
//	u8 t,n=7;
//	u8 len;	
//	u16 times=0,t_oled; 
//	printf("AT\r\n");  
//	delay_ms(100);
//	printf("AT+CIPSTART=\"TCP\",\"api.seniverse.com\",80\r\n");  
//	delay_ms(100);
//	printf("AT+CIPMODE=1\r\n");  
//	delay_ms(100);
//	printf("AT+CIPSEND\r\n");  
//	delay_ms(100);
//	printf("GET https://api.seniverse.com/v3/weather/now.json?key=Sw9ZH_ertLWhhY0qw&location=wuhan&language=en&unit=c\r\n");  
//	delay_ms(100);	
//	

//void max7219_showtianqi(void)
//{
//		if(USART_RX_STA&0x8000)
//		{					   
//			len=USART_RX_STA&0x3fff;//得到此次接收2到的数据长度
//			for(t=0;t<len;t++)
//			{
//				USART1->DR=USART_RX_BUF[t];
//				while((USART1->SR&0X40)==0);//等待发送结束
//			}
//			USART_RX_STA=0;

//		}

//		for (t_oled=200;t_oled<350;t_oled++)//显示天气
//		{
//		if(USART_RX_BUF[t_oled]==0x74)
//			if(USART_RX_BUF[t_oled+1]==0x65)
//				if(USART_RX_BUF[t_oled+2]==0x78)
//					if(USART_RX_BUF[t_oled+3]==0x74)
//					{
//						if(USART_RX_BUF[t_oled+n] !=0x22)
//						{
//						OLED_Refresh();
//						OLED_ShowChar((n-7)*8,0,USART_RX_BUF[t_oled+n],16,1);
//						n++;
//						}
//					}
//		
//		
//		}
//		
//	}

//	
////		for (t_oled=250;t_oled<350;t_oled++)//显示温度
////		{
////		if(USART_RX_BUF[t_oled]==0x74)
////			if(USART_RX_BUF[t_oled+1]==0x75)
////				if(USART_RX_BUF[t_oled+2]==0x72)
////					if(USART_RX_BUF[t_oled+3]==0x65)
////					{
////					OLED_Refresh();
////					OLED_ShowChar(0,16,USART_RX_BUF[t_oled+7],16,1);
////					OLED_ShowChar(8,16,USART_RX_BUF[t_oled+8],16,1);
////						
////					}
////		
////		
////		}