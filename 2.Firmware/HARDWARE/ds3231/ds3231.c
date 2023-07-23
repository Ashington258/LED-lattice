#include "ds3231.h"
#include "iic.h" 
#include "oled.h"
#include "delay.h"



void DS3231_Init(void)	//��ʼ��
{
	IIC_Init();
}


void Alarmlock_set(u8 x,u8 y,u8 z)							//��������ʱ��
{
_Alarmclock_part Alarmclock;//�������ӽṹ��
	
	Alarmclock.hour =x;//���ӳ�ʼֵ
	Alarmclock.min = y;
	Alarmclock.sec = z;
	Alarmclock1_close();//��ʼ�Ƚ����ӹرղ�������ӱ�־λ
	Alarmclock1_cmd(0);
	
}



void DS3231_IIC_Write(u8 addr,u8 data)	//д����
{
	IIC_Start();
	IIC_Send_Byte(DS3231_Write_Address);
	IIC_Wait_Ack();
	delay_us(5);
	IIC_Send_Byte(addr);
	IIC_Wait_Ack();
	delay_us(5);
	IIC_Send_Byte(data);
	IIC_Wait_Ack();
	delay_us(5);
	IIC_Stop();
}

u8 DS3231_IIC_Read(u8 addr)	//������
{
	u8 data_s;
	IIC_Start();
	IIC_Send_Byte(DS3231_Write_Address);
	IIC_Wait_Ack();
	delay_us(5);
	IIC_Send_Byte(addr);
	IIC_Wait_Ack();
	delay_us(5);
	IIC_Start();
	IIC_Send_Byte(DS3231_Read_Address);
	IIC_Wait_Ack();
	delay_us(5);
	data_s = IIC_Read_Byte(1);
	IIC_Stop();
	return data_s;
}

void SetAlarmclock(u8 ahour,u8 amin,u8 asec)//��������
{
	u8 d;
	d = BIN_BCD(ahour);
	DS3231_IIC_Write(DS3231_AL1HOUR_REG,d);
	d = BIN_BCD(amin);
	DS3231_IIC_Write(DS3231_AL1MIN_REG,d);
	d = BIN_BCD(asec);
	DS3231_IIC_Write(DS3231_AL1SEC_REG,d);
	DS3231_IIC_Write(DS3231_AL1WDAY_REG,0x80);
}

void Alarmclock1_cmd(u8 mode)//mode = 1 / 0  ����ʹ��
{	
	DS3231_IIC_Write(DS3231_CONTROL_REG,0x1C|mode);
}

u8 Alarmclock1state(void)	//��������Ӧ��־
{
	u8 A1F;
	A1F=DS3231_IIC_Read(DS3231_STATUS_REG)&0x01;
	return A1F;
}

void Alarmclock1_close(void)	//�ر����Ӳ������־λ
{
	Alarmclock1_cmd(0);
	DS3231_IIC_Write(DS3231_STATUS_REG,0x00);
}

void ModifyTime(u8 year,u8 month,u8 day,u8 hour,u8 min,u8 sec)//�޸�ʱ��
{
	u8 temp;
	temp = BIN_BCD(year);
	DS3231_IIC_Write(DS3231_YEAR_REG,temp);
	temp = BIN_BCD(month);
	DS3231_IIC_Write(DS3231_MONTH_REG,temp);
	temp = BIN_BCD(day);
	DS3231_IIC_Write(DS3231_MDAY_REG,temp);
	temp = BIN_BCD(hour);
	DS3231_IIC_Write(DS3231_HOUR_REG,temp);
	temp = BIN_BCD(min);
	DS3231_IIC_Write(DS3231_MIN_REG,temp);
	temp = BIN_BCD(sec);
	DS3231_IIC_Write(DS3231_SEC_REG,temp);
}

void get_time(void)//��ȡʱ��
{
	calendar.year = DS3231_IIC_Read(DS3231_YEAR_REG);
	calendar.year = BCD_BIN(calendar.year);
	
	calendar.month = DS3231_IIC_Read(DS3231_MONTH_REG);
	calendar.month = BCD_BIN(calendar.month);
	
	calendar.day = DS3231_IIC_Read(DS3231_MDAY_REG);
	calendar.day = BCD_BIN(calendar.day);
	
	calendar.hour = DS3231_IIC_Read(DS3231_HOUR_REG);
	calendar.hour &= 0x3f;
	calendar.hour = BCD_BIN(calendar.hour);
	
	calendar.min = DS3231_IIC_Read(DS3231_MIN_REG);
	calendar.min = BCD_BIN(calendar.min);
	
	calendar.sec = DS3231_IIC_Read(DS3231_SEC_REG);
	calendar.sec = BCD_BIN(calendar.sec);
}

u8 BCD_BIN(u8 val)//BCD��ת��Ϊ������
{
	u8 temp;
	temp=val&0x0f;
	val>>=4;
	val&=0x0f;
	val*=10;
	temp+=val;
	return temp;
}

u8 BIN_BCD(u8 val)//������ת��ΪBCD��
{
	u8 i,j,k;
	i=val/10;
	j=val%10;
	k=j+(i<<4);
	return k;
}


void OLED_Show_Time()
{
	OLED_Refresh();
	get_time();
		
	OLED_ShowNum(0,0,2000+calendar.year,4,16,1);
	OLED_ShowString(32,0,":",16,1);	
	OLED_ShowNum(40,0,calendar.month,2,16,1);
	OLED_ShowString(56,0,":",16,1);
	OLED_ShowNum(64,0,calendar.day,2,16,1);

	OLED_ShowNum(0,16,calendar.hour,2,16,1);
	OLED_ShowString(16,16,":",16,1);		
	OLED_ShowNum(24,16,calendar.min,2,16,1);
	OLED_ShowString(40,16,":",16,1); 
	OLED_ShowNum(48,16,calendar.sec,2,16,1);
	OLED_ShowString(64,16,".",16,1);


}


