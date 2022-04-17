#ifndef _MPU6050_H_
#define _MPU6050_H_

#include "I2Cdev.h"

#define MPU6050_DEFAULT_ADDRESS 0x68 // 设备地址
#define WHO_AM_I 0x75                // I2C地址寄存器

#define PWR_MGMT_1 0x6B   // 电源管理寄存器1
#define PWR_MGMT_2 0x6C   // 电源管理寄存器2
#define SMPLRT_DIV 0x19   // 陀螺仪采样率分频寄存器 1khz/(1+4)=200Hz
#define CONFIG 0x1A       // 配置寄存器
#define GYRO_CONFIG 0x1B  // 陀螺仪配置寄存器
#define ACCEL_CONFIG 0x1C // 加速计配置寄存器

// 加速计数据输出寄存器
#define ACCEL_XOUT_H 0x3B
#define ACCEL_XOUT_L 0x3C
#define ACCEL_YOUT_H 0x3D
#define ACCEL_YOUT_L 0x3E
#define ACCEL_ZOUT_H 0x3F
#define ACCEL_ZOUT_L 0x40

// 温度计数据输出寄存器
#define TEMP_OUT_H 0x41
#define TEMP_OUT_L 0x42

// 陀螺仪数据输出寄存器
#define GYRO_XOUT_H 0x43
#define GYRO_XOUT_L 0x44
#define GYRO_YOUT_H 0x45
#define GYRO_YOUT_L 0x46
#define GYRO_ZOUT_H 0x47
#define GYRO_ZOUT_L 0x48

// 弧度转角度转换率(360/PI/2)
#define RAD_TO_DEG 57.2958
// 角度转弧度转换率(2*PI/360)
#define DEG_TO_RAD 0.0175

// 初始化MPU
bool mpu_Init();

// 获取I2C地址
uint8_t mpu_getWAI();

// 零偏校准
void mpu_selfTest();

// 获取原始数据
float mpu_getAccX();
float mpu_getAccY();
float mpu_getAccZ();
float mpu_getGyroX();
float mpu_getGyroY();
float mpu_getGyroZ();

// 获取姿态角
float mpu_getPitch();
float mpu_getRoll();

#endif