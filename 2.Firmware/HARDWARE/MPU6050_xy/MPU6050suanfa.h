#ifndef MPU6050_H
#define MPU6050_H

#include "stm32f10x.h"



// 假设有MPU6050的库文件，包含相关的函数和定义

// 函数声明
void MPU6050_Init();
void MPU6050_ReadGyro();
float MPU6050_GetGyroX();
float MPU6050_GetGyroY();
float MPU6050_GetGyroZ();
//void printFlipDirection(FlipType direction);
u8 getmpu6050_fleg(void);
u8 mpu6050_key(void);

#endif
