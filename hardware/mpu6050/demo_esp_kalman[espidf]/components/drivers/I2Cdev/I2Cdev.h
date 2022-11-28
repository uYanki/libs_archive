#ifndef _I2CDEV_H_
#define _I2CDEV_H_

#include "driver/i2c.h"

#define SDA_PIN 21         // SDA引脚
#define SCL_PIN 22         // SCL引脚
#define I2C_NUM I2C_NUM_0  // I2C端口

bool i2c_Init();
bool i2c_readByte(uint8_t devAddr, uint8_t regAddr, uint8_t* data, uint16_t length);
bool i2c_writeByte(uint8_t devAddr, uint8_t regAddr, uint8_t data);

#endif
