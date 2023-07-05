#ifndef MPU6050_H
#define MPU6050_H

#include "stm32f10x.h"



// ������MPU6050�Ŀ��ļ���������صĺ����Ͷ���

// ��������
void MPU6050_Init();
void MPU6050_ReadGyro();
float MPU6050_GetGyroX();
float MPU6050_GetGyroY();
float MPU6050_GetGyroZ();
//void printFlipDirection(FlipType direction);
u8 getmpu6050_fleg(void);
u8 mpu6050_key(void);

#endif
