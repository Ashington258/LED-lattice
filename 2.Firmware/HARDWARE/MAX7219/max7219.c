#include "max7219.h"
#include "delay.h"
#include "ds3231.h"
#include "key.h"
#include "usart.h"

int time[6],colck[6];
u8 hour1,hour2,min1,min2,sec1,sec2,i,j;
extern u8 code_disp1[50][8];
extern u8 code_disp_cjh[38][8];
extern u8 currentMenuIndex;
extern u8 tianqi[20];


//{0x08,0x18,0x28,0x08,0x08,0x08,0x08,0x3E},//1
u8 code_disp1[50][8]={
{0x3C,0x42,0x42,0x42,0x42,0x42,0x42,0x3C},//0
{0x08,0x18,0x08,0x08,0x08,0x08,0x08,0x3E},//1
{0x7E,0x02,0x02,0x7E,0x40,0x40,0x40,0x7E},//2
{0x38,0x44,0x04,0x3C,0x04,0x04,0x44,0x38},//3
{0x08,0x18,0x28,0x48,0xFE,0x08,0x08,0x08},//4
{0x3C,0x40,0x40,0x78,0x04,0x04,0x44,0x38},//5
{0x38,0x44,0x40,0x78,0x44,0x44,0x44,0x38},//6
{0x3E,0x22,0x04,0x08,0x08,0x08,0x08,0x08},//7
{0x3C,0x42,0x42,0x7E,0x42,0x42,0x42,0x3C},//8
{0x3E,0x22,0x22,0x3E,0x02,0x02,0x02,0x3E},//9
{0x08,0x14,0x22,0x3E,0x22,0x22,0x22,0x22},//A
{0x78,0x44,0x44,0x78,0x44,0x44,0x44,0x78},//B//11
{0x3C,0x40,0x40,0x40,0x40,0x40,0x3C,0x00},//C
{0x7C,0x42,0x42,0x42,0x42,0x42,0x7C,0x00},//D
{0x7C,0x40,0x40,0x7C,0x40,0x40,0x40,0x7C},//E//14
{0x7C,0x40,0x40,0x7C,0x40,0x40,0x40,0x40},//F
{0x3C,0x40,0x40,0x40,0x40,0x44,0x44,0x3C},//G
{0x44,0x44,0x44,0x7C,0x44,0x44,0x44,0x44},//H
{0x7C,0x10,0x10,0x10,0x10,0x10,0x10,0x7C},//I
{0x3C,0x08,0x08,0x08,0x08,0x08,0x48,0x30},//J
{0x00,0x24,0x28,0x30,0x20,0x30,0x28,0x24},//K
{0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x7C},//L//21
{0x81,0xC3,0xA5,0x99,0x81,0x81,0x81,0x81},//M//22
{0x00,0x42,0x62,0x52,0x4A,0x46,0x42,0x00},//N
{0x3C,0x42,0x42,0x42,0x42,0x42,0x42,0x3C},//O//24
{0x3C,0x22,0x22,0x22,0x3C,0x20,0x20,0x20},//P//25
{0x1C,0x22,0x22,0x22,0x22,0x26,0x22,0x1D},//Q
{0x3C,0x22,0x22,0x22,0x3C,0x24,0x22,0x21},//R
{0x00,0x1E,0x20,0x20,0x3E,0x02,0x02,0x3C},//S
{0x00,0x3E,0x08,0x08,0x08,0x08,0x08,0x08},//T//29
{0x42,0x42,0x42,0x42,0x42,0x42,0x22,0x1C},//U
{0x42,0x42,0x42,0x42,0x42,0x42,0x24,0x18},//V
{0x00,0x49,0x49,0x49,0x49,0x2A,0x1C,0x00},//W
{0x81,0x42,0x24,0x18,0x18,0x24,0x42,0x81},//X
{0x41,0x22,0x14,0x08,0x08,0x08,0x08,0x08},//Y
{0x00,0x7F,0x02,0x04,0x08,0x10,0x20,0x7F},//Z
{0x00,0x00,0x01,0x00,0x01,0x00,0x00,0x00},//:
{0xC0,0xCE,0x10,0x10,0x10,0x10,0x0E,0x00},//*c//37
{0x3C,0x7E,0xFF,0xFF,0xFF,0xFF,0x7E,0x3C},//太阳
{0x00,0x00,0x1C,0x26,0x62,0x81,0xFF,0x00},//云
{0x1C,0x36,0x22,0xC1,0xFF,0x00,0x2A,0x2A},//雨

};

/************************************************/
u8 code_disp_cjh[38][8]={
{0x0E,0x11,0x11,0x11,0x11,0x11,0x11,0x0E},//0
{0x04,0x0C,0x04,0x04,0x04,0x04,0x04,0x0E},//1
{0x0E,0x11,0x01,0x02,0x04,0x08,0x10,0x1F},//2
{0x0E,0x11,0x01,0x0F,0x01,0x01,0x11,0x0E},//3
{0x02,0x06,0x0A,0x12,0x1F,0x02,0x02,0x00},//4
{0x0F,0x10,0x10,0x0E,0x01,0x01,0x11,0x0E},//5
{0x0E,0x10,0x10,0x1E,0x11,0x11,0x11,0x0E},//6
{0x1E,0x01,0x02,0x04,0x04,0x04,0x04,0x04},//7
{0x0E,0x11,0x11,0x1F,0x11,0x11,0x11,0x0E},//8
{0x0E,0x11,0x11,0x11,0x0F,0x01,0x11,0x0E},//9
{0x00,0x00,0x01,0x00,0x01,0x00,0x00,0x00},//:
{0x11,0x1B,0x0E,0x04,0x0E,0x0A,0x1B,0x11},//x


};
u8 code_disp_min[38][8]={
{0x00,0x00,0x00,0x07,0x05,0x05,0x05,0x07},/*"未命名文件",0*///0
{0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x01},/*"未命名文件",0*///1
{0x00,0x00,0x00,0x07,0x01,0x07,0x04,0x07},/*"未命名文件",0*///2
{0x00,0x00,0x00,0x07,0x01,0x07,0x01,0x07},/*"未命名文件",0*///3
{0x00,0x00,0x00,0x05,0x05,0x07,0x01,0x01},/*"未命名文件",0*///4
{0x00,0x00,0x00,0x07,0x04,0x07,0x01,0x07},/*"未命名文件",0*///5
{0x00,0x00,0x00,0x07,0x04,0x07,0x05,0x07},/*"未命名文件",0*///6
{0x00,0x00,0x00,0x07,0x01,0x01,0x01,0x01},/*"未命名文件",0*///7
{0x00,0x00,0x00,0x07,0x05,0x07,0x05,0x07},/*"未命名文件",0*///8
{0x00,0x00,0x00,0x07,0x05,0x07,0x01,0x07},/*"未命名文件",0*///9
{0x0E,0x09,0x09,0x0E,0x08,0x08,0x08,0x08},/*"未命名文件",0*///p
{0x00,0x00,0x00,0x00,0x0E,0x0A,0x0E,0x0F},/*"未命名文件",0*///a
{0x00,0x00,0x01,0x00,0x01,0x00,0x00,0x00},//:
{0x11,0x1B,0x0E,0x04,0x0E,0x0A,0x1B,0x11},//x
{0x00,0x06,0x09,0x08,0x08,0x08,0x09,0x06},//C//14
{0x00,0x04,0x04,0x04,0x04,0x04,0x04,0x06},//l
{0x00,0x00,0x00,0x06,0x09,0x09,0x09,0x06},//o
{0x00,0x00,0x00,0x00,0x09,0x09,0x09,0x06},//u
{0x02,0x02,0x02,0x02,0x0E,0x0A,0x0A,0x0F},//d
{0x00,0x05,0x05,0x07,0x01,0x01,0x05,0x02},//y
{0x06,0x08,0x08,0x0F,0x01,0x01,0x09,0x0E},//S//20
{0x00,0x00,0x00,0x00,0x07,0x05,0x05,0x05},//n
{0x00,0x0E,0x0A,0x0E,0x0C,0x0A,0x0A,0x09},//R
{0x00,0x00,0x04,0x00,0x04,0x04,0x04,0x04},//i






};







#if 0

//
SPI_IS_HARDWARE  //判断SPI方式

void Max7219_GPIO_Init(void)	//硬件SPI
{
	SPI_InitTypeDef  SPI1_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	 
	//配置SPI1管脚
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=Max7219_pinCS_GPIO;//CS/LOAD
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=Max7219_pinCLK_GPIO|SPI_MISO_GPIO|Max7219_pinDIN_GPIO;//CLK MISO MOSI 
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
	 
	SPI1_InitStructure.SPI_Direction=SPI_Direction_2Lines_FullDuplex;
	SPI1_InitStructure.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_64;
	SPI1_InitStructure.SPI_DataSize=SPI_DataSize_8b;
	SPI1_InitStructure.SPI_Mode=SPI_Mode_Master;
	SPI1_InitStructure.SPI_FirstBit=SPI_FirstBit_MSB; //高位在先
	SPI1_InitStructure.SPI_CPOL = SPI_CPOL_High;					
	SPI1_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI1_InitStructure.SPI_NSS = SPI_NSS_Soft; 
	SPI1_InitStructure.SPI_CRCPolynomial = 7;
	SPI_I2S_DeInit(SPI1);
	SPI_Init(SPI1, &SPI1_InitStructure); 
	SPI_Cmd(SPI1, ENABLE);   
}

//功能：向MAX7219内部寄存器写入数据
//参数：addr、dat
void Write_Max7219(u8 addr,u8 dat)
{
	Max7219_pinCS = 0;	
	SPI_I2S_SendData(SPI1, addr);  //写入地址
	delay_us(10);//必须，点阵不显示时可以延长此延时调试
	SPI_I2S_SendData(SPI1, dat);    	//写入数据
	delay_us(10);//必须，点阵不显示时可以延长此延时调试
	Max7219_pinCS = 1;
}

#endif










void Max7219_GPIO_Init(void)//软件模拟SPI
{
	 GPIO_InitTypeDef GPIO_InitStructure;
	 
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	 GPIO_InitStructure.GPIO_Pin  = Max7219_pinCS_GPIO | Max7219_pinDIN_GPIO | Max7219_pinCLK_GPIO;
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	 GPIO_Init(GPIOA,&GPIO_InitStructure);
}
 
//功能：向MAX7219写入字节
//入口参数：data
void Write_Max7219_byte(u8 data)         
{
	u8 i;    
	Max7219_pinCS = 0;	
	for(i = 0;i<8;i++)
	{			
		Max7219_pinCLK = 0;
		if((data & 0x80) == 0)//写成Max7219_pinDIN = data & 0x80;实测无效，Max7219_pinDIN一直是低电平
		{
			Max7219_pinDIN = 0;
		}
		else
		{
			Max7219_pinDIN = 1;
		}
		data = data<<1;
		Max7219_pinCLK = 1;
	 }
}

//功能：向MAX7219内部寄存器写入数据
//参数：addr、dat
void Write_Max7219(u8 addr,u8 dat)
{ 
   Max7219_pinCS = 0;
   Write_Max7219_byte(addr);           //写入地址
	 //delay_us(10);//模拟SPI时可以去除
   Write_Max7219_byte(dat);               //写入数据
	 //delay_us(10);
   Max7219_pinCS = 1; 
}


void Init_MAX7219(void)
{
		
	 Write_Max7219_1(0x09, 0x00,0x00);       //译码方式：BCD码
	 Write_Max7219_1(0x0a, 0x03,0x00);       //亮度 
	 Write_Max7219_1(0x0b, 0x07,0x00);       //扫描界限；8个数码管显示
	 Write_Max7219_1(0x0c, 0x01,0x00);       //掉电模式：0，普通模式：1
	 Write_Max7219_1(0x0f, 0x00,0x00);       //显示测试：1；测试结束，正常显示：0
	 
	 Write_Max7219_2(0x09, 0x00,0x00);       //译码方式：BCD码
	 Write_Max7219_2(0x0a, 0x03,0x00);       //亮度 
	 Write_Max7219_2(0x0b, 0x07,0x00);       //扫描界限；8个数码管显示
	 Write_Max7219_2(0x0c, 0x01,0x00);       //掉电模式：0，普通模式：1
	 Write_Max7219_2(0x0f, 0x00,0x00);       //显示测试：1；测试结束，正常显示：0
	 
	 Write_Max7219_3(0x09, 0x00,0x00);       //译码方式：BCD码
	 Write_Max7219_3(0x0a, 0x03,0x00);       //亮度 
	 Write_Max7219_3(0x0b, 0x07,0x00);       //扫描界限；8个数码管显示
	 Write_Max7219_3(0x0c, 0x01,0x00);       //掉电模式：0，普通模式：1
	 Write_Max7219_3(0x0f, 0x00,0x00);       //显示测试：1；测试结束，正常显示：0

	 Write_Max7219_4(0x09, 0x00,0x00);       //译码方式：无
	 Write_Max7219_4(0x0a, 0x03,0x00);       //亮度 
	 Write_Max7219_4(0x0b, 0x07,0x00);       //扫描界限；8个数码管显示
	 Write_Max7219_4(0x0c, 0x01,0x00);       //掉电模式：0，普通模式：1
	 Write_Max7219_4(0x0f, 0x00,0x00);       //显示测试：1；测试结束，正常显示：0
}
/************************************************/


//-------------------------------------------
/*向M第一片MAX7219写入数据*/
void Write_Max7219_1(uchar add1,uchar dat1,uchar dat2)
{ 
     Max7219_pinCS=0;
	 Write_Max7219_byte(add1);           		//写入地址，即数码管编号
     Write_Max7219_byte(dat1|dat2);             //写入数据，即数码管显示数字 
	 Max7219_pinCS=1;                        
}
/*第二片MAX7219的写入数据*/
void Write_Max7219_2(uchar add2,uchar dat1,uchar dat2)
{
	Max7219_pinCS=0;
	Write_Max7219_byte(add2);
	Write_Max7219_byte(dat1|dat2);
	Max7219_pinCLK=1;
	Write_Max7219_byte(0x00);;  //片1写入空
	Write_Max7219_byte(0x00);;
	Max7219_pinCS=1;
}
/*第三片MAX7219的写入数据*/
void Write_Max7219_3(uchar add3,uchar dat1,uchar dat2)
{
	Max7219_pinCS=0;
	Write_Max7219_byte(add3);
	Write_Max7219_byte(dat1|dat2);
	Max7219_pinCLK=1;
	Write_Max7219_byte(0x00);; //片1写入空
	Write_Max7219_byte(0x00);;
	Write_Max7219_byte(0x00);;//片2写入空
	Write_Max7219_byte(0x00);;
	Max7219_pinCS=1;
}
/*第四片MAX7219的写入数据*/
void Write_Max7219_4(uchar add4,uchar dat1,uchar dat2)
{
	Max7219_pinCS=0;
	Write_Max7219_byte(add4);
	Write_Max7219_byte(dat1|dat2);
	Max7219_pinCLK=1;
	Write_Max7219_byte(0x00);; //片1写入空
	Write_Max7219_byte(0x00);;
	Write_Max7219_byte(0x00);; //片2写入空
	Write_Max7219_byte(0x00);;
	Write_Max7219_byte(0x00);; //片3写入空
	Write_Max7219_byte(0x00);;
	Max7219_pinCS=1;
}


/**********************************************/
void Max7219_showtime()
{
	get_time();
	sec1=calendar.sec/10;
	sec2=calendar.sec%10;	
	min1=calendar.min/10;
	min2=calendar.min%10;
	hour1=calendar.hour/10;
	hour2=calendar.hour%10;			

   for(i=1;i<9;i++)
   {
	Write_Max7219_1(i,code_disp_min[sec2][i-1],code_disp_min[sec1][i-1]<<4);
	Write_Max7219_2(i,code_disp_cjh[min2][i-1],code_disp_cjh[min1][i-1]<<6);
	Write_Max7219_3(i,code_disp_cjh[10][i-1]<<4|code_disp_cjh[min1][i-1]>>2,code_disp_cjh[hour2][i-1]<<6);
	Write_Max7219_4(i,code_disp_cjh[hour2][i-1]>>2,code_disp_cjh[hour1][i-1]<<3);
	}
}
/*********************************************/


void Max7219_showclock()
{
	u8 k;
	get_time();
	
	time[0]=calendar.sec/10;	
	time[1]=calendar.sec%10;	
	time[2]=calendar.min/10;
	time[3]=calendar.min%10;
	time[4]=calendar.hour/10;
	time[5]=calendar.hour%10;	


	for (j=0;j<6;)
	{
		 if (Key_GetNum0()==1) //上键按下
		{
			time[j]++;
			if(time[j]>9)
			{	
				time[j]=0;
			}
			if(time[4]>2)
			{	
				time[4]=0;
			}
			if(time[0]>6)
			{	
				time[0]=0;
			}
			if(time[2]>6)
			{	
				time[2]=0;
			}
			
		}
		if (Key_GetNum1()==1) //下键按下
		{
			time[j]--;

			if(time[4]<0)
			{	
				time[4]=2;
			}
			if(time[0]<0)
			{	
				time[0]=5;
			}
			if(time[2]<0)
			{	
				time[2]=5;
			}
			 if(time[j]<0)
			{	
				time[j]=9;
			}
		}
		if (Key_GetNum2()==1) //确定键按下
		{
			j++;
		}
		if (Key_GetNum3()==1) //返回键按下
		{
		   j--;
		}
		
		hour1=time[4];
		hour2=time[5];
		min1=time[2];
		min2=time[3];
		sec1=time[0];
		sec2=time[1];
		
		for (k=0;k<6;k++)//记录闹钟时间
		{
		colck[k]=time[k];
		}
		

		for(i=1;i<9;i++)
	   {
		Write_Max7219_1(i,code_disp_min[sec2][i-1],code_disp_min[sec1][i-1]<<4);
		Write_Max7219_2(i,code_disp_cjh[min2][i-1],code_disp_cjh[min1][i-1]<<6);
		Write_Max7219_3(i,code_disp_cjh[10][i-1]<<4|code_disp_cjh[min1][i-1]>>2,code_disp_cjh[hour2][i-1]<<6);
		Write_Max7219_4(i,code_disp_cjh[hour2][i-1]>>2,code_disp_cjh[hour1][i-1]<<3);
		}

	}
	delay_ms(10);

}

void Bellmove()
{
	u8 isEqual = 1;
	u8 isEqual_2 = 1;
	u8 Bell=0;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA, GPIO_Pin_6);
	
	get_time();

	time[0]=calendar.sec/10;	
	time[1]=calendar.sec%10;	
	time[2]=calendar.min/10;
	time[3]=calendar.min%10;
	time[4]=calendar.hour/10;
	time[5]=calendar.hour%10;
    for (i = 0; i < 6; i++) 
	{
        if (time[i] != colck[i]) 
		{
            isEqual = 0;
            break;
        }
    }
	for (i = 0; i < 4; i++) 
	{
        if (time[i] != 0) 
		{
            isEqual_2 = 0;
            break;
        }
    }
    if (isEqual) 
	{
	GPIO_ResetBits(GPIOA, GPIO_Pin_6);
	delay_ms(300);
    }
	if (isEqual_2) 
	{
	GPIO_ResetBits(GPIOA, GPIO_Pin_6);
	delay_ms(300);
    }
	GPIO_SetBits(GPIOA, GPIO_Pin_6);
}


/***********************************************************/
void Max7219_showchar(u8 x1,u8 x2,u8 x3,u8 x4)
{

   for(i=1;i<9;i++)
   {
	Write_Max7219_1(i,code_disp1[x4][i-1],0);
	Write_Max7219_2(i,code_disp1[x3][i-1],0);
	Write_Max7219_3(i,code_disp1[x2][i-1],0);
	Write_Max7219_4(i,code_disp1[x1][i-1],0);
	}
}

void Max7219_1()
{
	Max7219_showchar(29,14,22,25);//temp
}

void Max7219_2()
{
	Max7219_showchar(11,14,21,21);//bell
}


/*******************************************************/
void max7219showtianqi(void)
{
	u8 n,tianqi_n;

	for (tianqi_n=0;tianqi_n<20;tianqi_n++)
	{
		if(tianqi[tianqi_n]==0x6E)
		{
			if(tianqi[tianqi_n+1]==0x6E)
			{
			  for(n=1;n<9;n++)
				{
				Write_Max7219_1(n,0,code_disp_min[19][n-1]<<4);//晴
				Write_Max7219_2(n,code_disp_min[21][n-1],code_disp_min[21][n-1]<<4);//晴
				Write_Max7219_3(n,code_disp_min[17][n-1],code_disp_min[20][n-1]<<4);//晴
				Write_Max7219_4(n,code_disp1[38][n-1],0);//晴
				}
			}
		}
		if(tianqi[tianqi_n]==0x65)
		{
			if(tianqi[tianqi_n+1]==0x72)
			{
			  for(n=1;n<9;n++)
				{
				Write_Max7219_1(n,0,0);
				Write_Max7219_2(n,code_disp_min[21][n-1],code_disp_min[23][n-1]<<4);
				Write_Max7219_3(n,code_disp_min[11][n-1],code_disp_min[22][n-1]<<4);
				Write_Max7219_4(n,code_disp1[40][n-1],0);//雨
				}
			}	
		}		
		if(tianqi[tianqi_n]==0x69)
		{
			if(tianqi[tianqi_n+1]==0x6E)
			{
			  for(n=1;n<9;n++)
				{
				Write_Max7219_1(n,0,0);
				Write_Max7219_2(n,code_disp_min[21][n-1],code_disp_min[23][n-1]<<4);
				Write_Max7219_3(n,code_disp_min[11][n-1],code_disp_min[22][n-1]<<4);
				Write_Max7219_4(n,code_disp1[40][n-1],0);//雨
				}
			}
		}
		else
			{
			  for(n=1;n<9;n++)
				{
				Write_Max7219_1(n,code_disp_min[19][n-1],code_disp_min[18][n-1]<<4);
				Write_Max7219_2(n,code_disp_min[17][n-1],code_disp_min[16][n-1]<<4);
				Write_Max7219_3(n,code_disp_min[15][n-1],code_disp_min[14][n-1]<<4);
				Write_Max7219_4(n,code_disp1[39][n-1],0);//云
				}
			}
	}
}















