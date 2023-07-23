#include "BME280.h"
#include "sys.h"
#include "Delay.h"
#include "compensation.h"
#include "iic.h"
#include "oled.h"
#include "max7219.h"


extern u8 code_disp1[38][8];
extern u8 code_disp_cjh[38][8];
extern u8 code_disp_min[38][8];

void OLED_Show_Temperature(u8 x,u8 y)
{
	OLED_ShowNum(x,y,BMP280_Get_Temperature(),7,16,1);
}


void OLED_Show_Pressure(u8 x,u8 y)
{
	OLED_ShowNum(x,y,BMP280_Get_Pressure(),7,16,1);
}

void Max7219_showtemp()
{
	u8 i;
	u32 x,x1,x2;
	x=BMP280_Get_Temperature();
	x1=x/10;
	x2=x%10;
	for(i=1;i<9;i++)
   {
	Write_Max7219_1(i,0,0);   
	Write_Max7219_2(i,code_disp1[37][i-1],0);
	Write_Max7219_3(i,code_disp1[x2][i-1],0);
	Write_Max7219_4(i,code_disp1[x1][i-1],0);
	}

}

void Max7219_showPressure()
{
	u8 i;
	u32 x,a[6];
	x=BMP280_Get_Pressure();
	for (i=0;i<6;i++)
	{
	a[i]=x%10;
	x=x/10;
	}

	for(i=1;i<9;i++)
   {
	Write_Max7219_1(i,code_disp_min[11][i-1],code_disp_min[10][i-1]<<4);   
	Write_Max7219_2(i,code_disp_min[a[0]][i-1],code_disp_min[a[1]][i-1]<<4);
	Write_Max7219_3(i,code_disp_min[a[2]][i-1],code_disp_min[a[3]][i-1]<<4);
	Write_Max7219_4(i,code_disp_min[a[4]][i-1],code_disp_min[a[5]][i-1]<<4);
	}

}



static uint8_t BMP280_Read_Byte(u8 reg)
{
	uint8_t rec_data;
	IIC_Start();
	IIC_Send_Byte(BMP280_ADDRESS<<1|0);
	IIC_Wait_Ack();
	IIC_Send_Byte(reg);
	IIC_Wait_Ack();
	
	IIC_Start();
	IIC_Send_Byte(BMP280_ADDRESS<<1|1);
	IIC_Wait_Ack();
	rec_data = IIC_Read_Byte(0);	//不应答
	IIC_Stop();
	return rec_data;
}

static void BMP280_Write_Byte(u8 reg,u8 data)
{
	IIC_Start();
	IIC_Send_Byte(BMP280_ADDRESS<<1);
	IIC_Wait_Ack();
	IIC_Send_Byte(reg);
	IIC_Wait_Ack();

	IIC_Send_Byte(data);
	IIC_Wait_Ack();
	IIC_Stop();
}





BMP280 bmp280_inst;
BMP280* bmp280 = &bmp280_inst;		//这个全局结构体变量用来保存存在芯片内ROM补偿参数
//设置BMP过采样因子 MODE 
//BMP280_SLEEP_MODE||BMP280_FORCED_MODE||BMP280_NORMAL_MODE
void BMP280_Set_TemOversamp(BMP_OVERSAMPLE_MODE * Oversample_Mode)
{
	u8 Regtmp;
	Regtmp = ((Oversample_Mode->T_Osample)<<5)|
			 ((Oversample_Mode->P_Osample)<<2)|
			 ((Oversample_Mode)->WORKMODE);
	
	BMP280_Write_Byte(BMP280_CTRLMEAS_REG,Regtmp);
}


//设置保持时间和滤波器分频因子
void BMP280_Set_Standby_FILTER(BMP_CONFIG * BMP_Config)
{
	u8 Regtmp;
	Regtmp = ((BMP_Config->T_SB)<<5)|
			 ((BMP_Config->FILTER_COEFFICIENT)<<2)|
			 ((BMP_Config->SPI_EN));
	
	BMP280_Write_Byte(BMP280_CONFIG_REG,Regtmp);
}



void Bmp_Init(void)
{
	u8 Lsb,Msb;
	BMP_OVERSAMPLE_MODE			BMP_OVERSAMPLE_MODEStructure;
	BMP_CONFIG					BMP_CONFIGStructure;	
	/********************接下来读出矫正参数*********************/
	//温度传感器的矫正值
	Lsb = BMP280_Read_Byte(BMP280_DIG_T1_LSB_REG);
	Msb = BMP280_Read_Byte(BMP280_DIG_T1_MSB_REG);
	bmp280->T1 = (((u16)Msb)<<8) + Lsb;			//高位加低位
	Lsb = BMP280_Read_Byte(BMP280_DIG_T2_LSB_REG);
	Msb = BMP280_Read_Byte(BMP280_DIG_T2_MSB_REG);
	bmp280->T2 = (((u16)Msb)<<8) + Lsb;		
	Lsb = BMP280_Read_Byte(BMP280_DIG_T3_LSB_REG);
	Msb = BMP280_Read_Byte(BMP280_DIG_T3_MSB_REG);
	bmp280->T3 = (((u16)Msb)<<8) + Lsb;		
	
	//大气压传感器的矫正值
	Lsb = BMP280_Read_Byte(BMP280_DIG_P1_LSB_REG);
	Msb = BMP280_Read_Byte(BMP280_DIG_P1_MSB_REG);
	bmp280->P1 = (((u16)Msb)<<8) + Lsb;		
	Lsb = BMP280_Read_Byte(BMP280_DIG_P2_LSB_REG);
	Msb = BMP280_Read_Byte(BMP280_DIG_P2_MSB_REG);
	bmp280->P2 = (((u16)Msb)<<8) + Lsb;	
	Lsb = BMP280_Read_Byte(BMP280_DIG_P3_LSB_REG);
	Msb = BMP280_Read_Byte(BMP280_DIG_P3_MSB_REG);
	bmp280->P3 = (((u16)Msb)<<8) + Lsb;	
	Lsb = BMP280_Read_Byte(BMP280_DIG_P4_LSB_REG);
	Msb = BMP280_Read_Byte(BMP280_DIG_P4_MSB_REG);
	bmp280->P4 = (((u16)Msb)<<8) + Lsb;	
	Lsb = BMP280_Read_Byte(BMP280_DIG_P5_LSB_REG);
	Msb = BMP280_Read_Byte(BMP280_DIG_P5_MSB_REG);
	bmp280->P5 = (((u16)Msb)<<8) + Lsb;	
	Lsb = BMP280_Read_Byte(BMP280_DIG_P6_LSB_REG);
	Msb = BMP280_Read_Byte(BMP280_DIG_P6_MSB_REG);
	bmp280->P6 = (((u16)Msb)<<8) + Lsb;	
	Lsb = BMP280_Read_Byte(BMP280_DIG_P7_LSB_REG);
	Msb = BMP280_Read_Byte(BMP280_DIG_P7_MSB_REG);
	bmp280->P7 = (((u16)Msb)<<8) + Lsb;	
	Lsb = BMP280_Read_Byte(BMP280_DIG_P8_LSB_REG);
	Msb = BMP280_Read_Byte(BMP280_DIG_P8_MSB_REG);
	bmp280->P8 = (((u16)Msb)<<8) + Lsb;	
	Lsb = BMP280_Read_Byte(BMP280_DIG_P9_LSB_REG);
	Msb = BMP280_Read_Byte(BMP280_DIG_P9_MSB_REG);
	bmp280->P9 = (((u16)Msb)<<8) + Lsb;	
	/******************************************************/
	

	BMP280_Write_Byte(BMP280_RESET_REG,BMP280_RESET_VALUE);	//往复位寄存器写入给定值
	

	BMP_OVERSAMPLE_MODEStructure.P_Osample = BMP280_P_MODE_3;
	BMP_OVERSAMPLE_MODEStructure.T_Osample = BMP280_T_MODE_1;
	BMP_OVERSAMPLE_MODEStructure.WORKMODE  = BMP280_NORMAL_MODE;
	BMP280_Set_TemOversamp(&BMP_OVERSAMPLE_MODEStructure);
	

	BMP_CONFIGStructure.T_SB = BMP280_T_SB1;
	BMP_CONFIGStructure.FILTER_COEFFICIENT = BMP280_FILTER_MODE_4;
	BMP_CONFIGStructure.SPI_EN = DISABLE;
	
	BMP280_Set_Standby_FILTER(&BMP_CONFIGStructure);

}


uint8_t BMP280_ReadID(void)
{
	return BMP280_Read_Byte(BMP280_CHIPID_REG);
}




//获取BMP当前状态
//status_flag = BMP280_MEASURING ||
//			 	BMP280_IM_UPDATE
u8  BMP280_GetStatus(u8 status_flag)
{
	u8 flag;
	flag = BMP280_Read_Byte(BMP280_STATUS_REG);
	if(flag&status_flag)	return SET;
	else return RESET;
}
////大气压值-Pa
//long BMP280_Get_Pressure(void)
//{
//	uint8_t XLsb,Lsb, Msb;
//	long signed Bit32;
//	//double pressure;
//	XLsb = BMP280_Read_Byte(BMP280_PRESSURE_XLSB_REG);
//	Lsb	 = BMP280_Read_Byte(BMP280_PRESSURE_LSB_REG);
//	Msb	 = BMP280_Read_Byte(BMP280_PRESSURE_MSB_REG);
//	Bit32 = ((long)(Msb << 12))|((long)(Lsb << 4))|(XLsb>>4);	//寄存器的值组合起来
//	return Bit32 ;
//}

/*******************主要部分*********************/
/****************获取传感器精确值****************/
//大气压值-Pa
double BMP280_Get_Pressure(void)
{
	uint8_t XLsb,Lsb, Msb;
	long signed Bit32;
	double pressure;
	XLsb = BMP280_Read_Byte(BMP280_PRESSURE_XLSB_REG);
	Lsb	 = BMP280_Read_Byte(BMP280_PRESSURE_LSB_REG);
	Msb	 = BMP280_Read_Byte(BMP280_PRESSURE_MSB_REG);
	Bit32 = ((long)(Msb << 12))|((long)(Lsb << 4))|(XLsb>>4);	//寄存器的值,组成一个浮点数
	pressure = bmp280_compensate_P_double(Bit32);
	return pressure;
}

//温度值-℃
double BMP280_Get_Temperature(void)
{
	uint8_t XLsb,Lsb, Msb;
	long signed Bit32;
	double temperature;
	XLsb = BMP280_Read_Byte(BMP280_TEMPERATURE_XLSB_REG);
	Lsb	 = BMP280_Read_Byte(BMP280_TEMPERATURE_LSB_REG);
	Msb	 = BMP280_Read_Byte(BMP280_TEMPERATURE_MSB_REG);
	Bit32 = ((long)(Msb << 12))|((long)(Lsb << 4))|(XLsb>>4);	//寄存器的值,组成一个浮点数
	temperature = bmp280_compensate_T_double(Bit32);
	return temperature;
}
/***************************************END OF LINE*********************************************/
