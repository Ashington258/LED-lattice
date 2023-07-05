#include "stm32f10x.h"
#include "mpu6050.h" // 假设有MPU6050的库文件，包含相关的函数和定义
#include "delay.h"
#include "ds3231.h"


// 定义滤波和校准的参数
#define FILTER_ALPHA 0.8f
#define CALIBRATION_SAMPLES 1000


/**

    在这个特定的例子中，FILTER_ALPHA 可以被用于控制滤波算法中的滤波系数。
    它表示滤波中新数据的权重比例，即滤波后的结果是原始数据和上一次滤波结果的加权平均值。
    在这里，FILTER_ALPHA 的值为 0.8f，表示新数据的权重为 80%，上一次滤波结果的权重为 20%。
    这个值可以根据需要进行调整，以获得适合的滤波效果。
**/
// 定义翻转方向的枚举类型
typedef enum
{
    FlipNone,
    FlipUp,
    FlipDown,
    FlipLeft,
    FlipRight
} FlipType;

/**
  typedef 关键字给该枚举类型起了一个别名 FlipType。使用 typedef 后，可以直接使用别名 FlipType 来代表该枚举类型。这样做可以简化代码并提高可读性，因为不再需要使用完整的枚举类型名称。

  正常写法：
  enum FlipType {
    FlipNone,
    FlipUp,
    FlipDown,
    FlipLeft,
    FlipRight
};
这种写法定义了一个枚举类型 enum FlipType，其中包含了五个枚举常量 FlipNone、FlipUp、FlipDown、FlipLeft 和 FlipRight。这些常量可以在程序中直接使用，表示不同的翻转方向。使用这种写法时，需要使用完整的枚举类型名称 enum FlipType 来声明变量或使用枚举常量。

enum FlipType flipDirection;
flipDirection = FlipLeft;


FlipType flipDirection;：声明了一个名为 flipDirection 的变量，其类型为 FlipType，即之前定义的枚举类型。这个变量将用于存储翻转方向的值。
flipDirection = FlipLeft;：将变量 flipDirection 的值设置为 FlipLeft，即将其赋值为枚举类型中定义的翻转方向 FlipLeft。这样，flipDirection 变量就代表了翻转方向为向左。

**/

// 全局变量
short gx,gy,gz,ax,ay,az;

// 全局变量
float gyroX, gyroY, gyroZ;
float gyroXoffset, gyroYoffset, gyroZoffset;



// 初始化MPU6050并进行滤波和校准
void MPU6050_Init()
{
    int i;
    float gyroXsum = 0, gyroYsum = 0, gyroZsum = 0;
    // 初始化MPU6050，包括I2C初始化等步骤
	MPU_Init(); 		
    // 设置陀螺仪滤波和校准

	MPU_Get_Gyroscope(&gx,&gy,&gz);	 // 读取陀螺仪数据   //G是角速度
	gyroXsum += gx;
	gyroYsum += gy;
	gyroZsum += gz;
	delay_ms(1); // 等待1毫秒
    gyroXoffset = gyroXsum / CALIBRATION_SAMPLES;
    gyroYoffset = gyroYsum / CALIBRATION_SAMPLES;
    gyroZoffset = gyroZsum / CALIBRATION_SAMPLES;
}

// 读取陀螺仪数据并进行滤波
void MPU6050_ReadGyro()
{    
	static float filteredGyroX = 0, filteredGyroY = 0, filteredGyroZ = 0;
    // 读取原始的陀螺仪数据
	
	MPU_Get_Gyroscope(&gx,&gy,&gz);
    gyroX = gx;
    gyroY = gy;
    gyroZ = gz;

    // 滤波处理

    filteredGyroX = (FILTER_ALPHA * filteredGyroX) + ((1 - FILTER_ALPHA) * (gyroX - gyroXoffset));
    filteredGyroY = (FILTER_ALPHA * filteredGyroY) + ((1 - FILTER_ALPHA) * (gyroY - gyroYoffset));
    filteredGyroZ = (FILTER_ALPHA * filteredGyroZ) + ((1 - FILTER_ALPHA) * (gyroZ - gyroZoffset));

    // 更新滤波后的陀螺仪数据
    gyroX = filteredGyroX;
    gyroY = filteredGyroY;
    gyroZ = filteredGyroZ;
}





// 检测翻转方向
u8 getmpu6050_fleg(void)	
{
	u8 mpu6050_fleg;
    // 读取陀螺仪数据
    MPU6050_ReadGyro();
    // 检测翻转方向
    if (gyroX > 2000)
    {
		mpu6050_fleg=1;//        return FlipRight;
		
    }
    else if (gyroX < -2000)
    {
		mpu6050_fleg=2;//        return FlipLeft;
		
    }
    else if (gyroY > 2000)
    {
		mpu6050_fleg=3;//        return FlipDown;
    }
    else if (gyroY < -2000)
    {
		mpu6050_fleg=4;//        return FlipUp;
    }
    else
    {
		mpu6050_fleg=5;//        return FlipNone;
    }
	return mpu6050_fleg;
	
}



u8 mpu6050_key(void)
{
	u8 s_move,s_now,time,fleged,fleg;
	get_time();
//	s_now=calendar.sec;
	fleg=getmpu6050_fleg();
	fleged=fleg;
	if(fleg!=5)
	{
	s_move=calendar.sec;
	if(calendar.sec-s_move<=2)
		get_time();
//		fleg=fleged;
	}
	


}



























//// 检测翻转方向
//FlipType DetectFlipDirection()
//{
//    // 读取陀螺仪数据
//    MPU6050_ReadGyro();
//    // 检测翻转方向
//    if (gyroX > 50)
//    {
////        return FlipRight;
//		return x1=1;
//		
//    }
//    else if (gyroX < -50)
//    {
////        return FlipLeft;
//		x2=1;
//		
//    }
//    else if (gyroY > 50)
//    {
////        return FlipDown;
//		x3=1;
//    }
//    else if (gyroY < -50)
//    {
////        return FlipUp;
//		x4=1;
//    }
//    else
//    {
////        return FlipNone;
//		x5
//    }
//}
/**
在上述代码中，我们通过判断陀螺仪的角速度来检测翻转方向。如果陀螺仪在 X 轴上的角速度超过 50（可根据实际情况进行调整），则认为发生了向右翻转，返回 `FlipRight`。如果角速度小于 -50，则认为发生了向左翻转，返回 `FlipLeft`。类似地，我们可以根据 Y 轴的角速度判断向上翻转（`FlipUp`）和向下翻转（`FlipDown`）。如果都不满足以上条件，则返回 `FlipNone`，表示没有发生翻转操作。

请注意，这只是一个简单的示例，用于演示翻转检测的思路。实际应用中，你可能需要根据具体需求和实际情况进行参数调整和算法优化，以提高识别的准确性和稳定性。

另外，请确保在 `DetectFlipDirection()` 函数中调用了 `MPU6050_ReadGyro()` 函数，以获取最新的陀螺仪数据进行处理。

希望这可以帮助你实现基于STM32F103标准库的MPU6050翻转检测功能。**/
//void printFlipDirection(FlipType direction)
//{
//	

//    if (direction == FlipNone)
//    {
////        printf("No flip");
//		x1=1;
//    }
//    else if (direction == FlipUp)
//    {
////       printf("Flip Up");
//		x2=2;
//		
//    }
//    else if (direction == FlipDown)
//    {
////        printf("Flip Down");
//		x3=3;
//    }
//    else if (direction == FlipLeft)
//    {
////        printf("Flip Left");
//		x4=1;
//    }
//    else if (direction == FlipRight)
//    {
////        printf("Flip Right");
//		x5=1;
//    }
//    else
//    {
////        printf("Unknown flip direction");
//		x6=1;
//    }
//}
