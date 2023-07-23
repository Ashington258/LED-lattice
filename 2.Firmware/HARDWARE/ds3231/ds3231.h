#ifndef _DS3231_H_
#define _DS3231_H_
#include "sys.h"

typedef struct
{
	u8 sec;
	u8 min;
	u8 hour;
	u8 week;
	u8 day;
	u8 month;
	u32 year;
}_calendar_part;	//ʱ��ṹ��

extern _calendar_part calendar;

typedef struct
{
	u8 hour;
	u8 min;
	u8 sec;
}_Alarmclock_part;//���ӽṹ��

extern _Alarmclock_part Alarmclock;

typedef struct
{
	u8 sure_flag;
	u8 stop_flag;
}_Alarm_flag;	//��־�ṹ��




extern _Alarm_flag flag;

/*  IIC_SDAʱ�Ӷ˿ڡ����Ŷ��� */
#define DS3231_SDA_PORT 			GPIOB  
#define DS3231_SDA_PIN 			(GPIO_Pin_7)
#define DS3231_SDA_PORT_RCC		RCC_APB2Periph_GPIOB

#define DS3231_Write_Address  0xD0  //д��ַ
#define DS3231_Read_Address   0xD1	//����ַ

#define EEPROM_Write_Address  0xAE	//дee��ַ
#define EEPROM_Read_Address   0xAF	//��ee��ַ

#define DS3231_SEC_REG        0x00  //��
#define DS3231_MIN_REG        0x01  //��
#define DS3231_HOUR_REG       0x02	//ʱ
#define DS3231_WDAY_REG       0x03	//����
#define DS3231_MDAY_REG       0x04	//��
#define DS3231_MONTH_REG      0x05	//��
#define DS3231_YEAR_REG       0x06	//��
/***����1***/
#define DS3231_AL1SEC_REG     0x07	//��
#define DS3231_AL1MIN_REG     0x08	//��
#define DS3231_AL1HOUR_REG    0x09 	//ʱ
#define DS3231_AL1WDAY_REG    0x0A	//��/����
/***����2***/
#define DS3231_AL2MIN_REG     0x0B	//��
#define DS3231_AL2HOUR_REG    0x0C	//ʱ
#define DS3231_AL2WDAY_REG    0x0D	//��/����

#define DS3231_CONTROL_REG          0x0E//����
#define DS3231_STATUS_REG           0x0F//״̬
#define DS3231_AGING_OFFSET_REG     0x10//�ϻ������Ĵ���
#define DS3231_TMP_UP_REG           0x11//�¶ȼĴ�����8�ֽ�
#define DS3231_TMP_LOW_REG          0x12//�¶ȵ�2�ֽ�

void DS3231_Init(void);//��ʼ��
void DS3231_IIC_Write(u8 addr,u8 data);	//д�Ĵ���
u8 DS3231_IIC_Read(u8 addr);	//���Ĵ���
void ModifyTime(u8 year,u8 month,u8 day,u8 hour,u8 min,u8 sec);//�޸�ʱ������
void get_time(void);//��ȡʱ��
void SetAlarmclock(u8 ahour,u8 amin,u8 asec);//��������1
void Alarmclock1_cmd(u8 mode);//ʹ������1 mode = 1 / 0
u8 Alarmclock1state(void);//��ȡ����1��־λ
void Alarmclock1_close(void);//�ر�����1
void Alarmlock_set(u8 x,u8 y,u8 z);//��������ʱ��

void OLED_Show_Time();

u8 BCD_BIN(u8 val);//BCD��ת��Ϊ������
u8 BIN_BCD(u8 val);//������ת��ΪBCD��

#endif


