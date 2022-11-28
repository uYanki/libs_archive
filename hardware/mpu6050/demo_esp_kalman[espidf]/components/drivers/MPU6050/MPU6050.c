#include <math.h>

#include "esp_log.h"

#include "MPU6050.h"

// 设备地址
static uint8_t devAddr;
// 存储零偏值
static float axZero;
static float ayZero;
static float azZero;
static float gxZero;
static float gyZero;
static float gzZero;

// 初始化MPU
bool mpu_Init()
{
    i2c_Init();
    vTaskDelay(100 / portTICK_RATE_MS);
    devAddr = MPU6050_DEFAULT_ADDRESS;
    ESP_LOGI("MPU6050", "配置电源管理寄存器...");
    i2c_writeByte(devAddr, PWR_MGMT_1, 0x80);
    vTaskDelay(100 / portTICK_RATE_MS);
    i2c_writeByte(devAddr, PWR_MGMT_1, 0x01);
    ESP_LOGI("MPU6050", "配置陀螺仪采样率分频寄存器...");
    i2c_writeByte(devAddr, SMPLRT_DIV, 0x04);
    ESP_LOGI("MPU6050", "配置数字低通滤波器...");
    i2c_writeByte(devAddr, CONFIG, 0x02);
    ESP_LOGI("MPU6050", "设置陀螺仪满量程范围...");
    i2c_writeByte(devAddr, GYRO_CONFIG, 0x18);
    ESP_LOGI("MPU6050", "设置加速计满量程范围...");
    i2c_writeByte(devAddr, ACCEL_CONFIG, 0x08);
    ESP_LOGI("MPU6050", "校准MPU...");
    mpu_selfTest();
    ESP_LOGI("MPU6050", "初始化成功!");
    vTaskDelay(100 / portTICK_RATE_MS);
    return true;
}

// 获取I2C地址
uint8_t mpu_getWAI()
{
    uint8_t r[0];
    i2c_readByte(devAddr, WHO_AM_I, r, 1);
    return r[0];
}

// 零偏校准
void mpu_selfTest()
{
    axZero = 0;
    ayZero = 0;
    azZero = 0;
    gxZero = 0;
    gyZero = 0;
    gzZero = 0;
    float temp[6] = {0, 0, 0, 0, 0, 0};
    for (int i = 0; i < 1000; i++)
    {
        temp[0] += mpu_getAccX();
        temp[1] += mpu_getAccY();
        temp[2] += mpu_getAccY();
        temp[3] += mpu_getAccY();
        temp[4] += mpu_getAccY();
        temp[5] += mpu_getAccY();
    }
    axZero = temp[0] / 1000;
    ayZero = temp[1] / 1000;
    azZero = temp[2] / 1000;
    gxZero = temp[3] / 1000;
    gyZero = temp[4] / 1000;
    gzZero = temp[5] / 1000;
}

// 获取原始数据
float mpu_getAccX()
{
    uint8_t r[0];
    i2c_readByte(devAddr, ACCEL_XOUT_H, r, 2);
    short accx = (r[0] << 8) | r[1];
    return ((float)accx / 8192.00 * -1) - axZero;
}
float mpu_getAccY()
{
    uint8_t r[0];
    i2c_readByte(devAddr, ACCEL_YOUT_H, r, 2);
    short accy = (r[0] << 8) | r[1];
    return ((float)accy / 8192.00) - ayZero;
}
float mpu_getAccZ()
{
    uint8_t r[0];
    i2c_readByte(devAddr, ACCEL_ZOUT_H, r, 2);
    short accz = (r[0] << 8) | r[1];
    return ((float)accz / 8192.00) - azZero;
}
float mpu_getGyroX()
{
    uint8_t r[0];
    i2c_readByte(devAddr, GYRO_XOUT_H, r, 2);
    short gyrox = (r[0] << 8) | r[1];
    return ((float)gyrox / 16.38) - gxZero;
}
float mpu_getGyroY()
{
    uint8_t r[0];
    i2c_readByte(devAddr, GYRO_YOUT_H, r, 2);
    short gyroy = (r[0] << 8) | r[1];
    return ((float)gyroy / 16.38) - gyZero;
}
float mpu_getGyroZ()
{
    uint8_t r[0];
    i2c_readByte(devAddr, GYRO_ZOUT_H, r, 2);
    short gyroz = (r[0] << 8) | r[1];
    return ((float)gyroz / 16.38) - gzZero;
}

// 获取姿态角
float mpu_getPitch()
{
    float ax, ay, az;
    ax = mpu_getAccX();
    ay = mpu_getAccY();
    az = mpu_getAccZ();
    float pitch = atan(ay / sqrt(ax * ax + az * az)) * RAD_TO_DEG;
    return pitch;
}
float mpu_getRoll()
{
    float ax, az;
    ax = mpu_getAccX();
    az = mpu_getAccZ();
    float roll = atan2(ax, az) * RAD_TO_DEG;
    return roll;
}