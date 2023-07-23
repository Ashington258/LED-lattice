#ifndef __COMPENSATION_H
#define __COMPENSATION_H

#include "sys.h"
#include "BME280.h"

/*******************************下面是用来计算补偿值相关**********************************/
typedef			long signed int				BMP280_S32_t;	//有符号 64位！
typedef			long unsigned int			BMP280_U32_t;	//无符号 32位！
typedef			long long signed int		BMP280_S64_t;
//extern BMP280* bmp280;

double bmp280_compensate_T_double(BMP280_S32_t adc_T);
double bmp280_compensate_P_double(BMP280_S32_t adc_P);	

#define	dig_T1			bmp280->T1	
#define	dig_T2			bmp280->T2	
#define	dig_T3			bmp280->T3	

#define	dig_P1			bmp280->P1
#define	dig_P2			bmp280->P2
#define	dig_P3			bmp280->P3
#define	dig_P4			bmp280->P4
#define	dig_P5			bmp280->P5
#define	dig_P6			bmp280->P6
#define	dig_P7			bmp280->P7
#define	dig_P8			bmp280->P8
#define	dig_P9			bmp280->P9
/************************************************CUT****************************************/







#endif

