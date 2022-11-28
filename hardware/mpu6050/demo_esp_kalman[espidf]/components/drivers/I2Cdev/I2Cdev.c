#include "I2Cdev.h"

// 初始化IIC
bool i2c_Init() {
    i2c_config_t conf;
    conf.mode             = I2C_MODE_MASTER;
    conf.sda_io_num       = SDA_PIN;  // SDA映射
    conf.scl_io_num       = SCL_PIN;  // SCL映射
    conf.sda_pullup_en    = GPIO_PULLUP_ENABLE;
    conf.scl_pullup_en    = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = 400000;  // 时钟频率
    i2c_param_config(I2C_NUM, &conf);
    i2c_driver_install(I2C_NUM, I2C_MODE_MASTER, 0, 0, 0);
    return true;
}

bool i2c_readByte(uint8_t devAddr, uint8_t regAddr, uint8_t* data, uint16_t length) {
    i2c_cmd_handle_t cmd;

    cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (devAddr << 1) | I2C_MASTER_WRITE, 1);
    i2c_master_write_byte(cmd, regAddr, 1);
    i2c_master_stop(cmd);
    i2c_master_cmd_begin(I2C_NUM, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);

    cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (devAddr << 1) | I2C_MASTER_READ, 1);
    if (length > 1) { i2c_master_read(cmd, data, length - 1, I2C_MASTER_ACK); }
    i2c_master_read_byte(cmd, data + length - 1, I2C_MASTER_NACK);
    i2c_master_stop(cmd);
    i2c_master_cmd_begin(I2C_NUM, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);
    return true;
}

bool i2c_writeByte(uint8_t devAddr, uint8_t regAddr, uint8_t data) {
    i2c_cmd_handle_t cmd;                                              // 命令链接句柄
    cmd = i2c_cmd_link_create();                                       // 创建命令链接
    i2c_master_start(cmd);                                             // 传输起始信号
    i2c_master_write_byte(cmd, (devAddr << 1) | I2C_MASTER_WRITE, 1);  // 广播从机地址(7Bit)|选择传输方向读或写(1Bit),检查从机应答或非应答信号
    i2c_master_write_byte(cmd, regAddr, 1);                            // 填充数据(8Bit),检查从机应答或非应答信号
    i2c_master_write_byte(cmd, data, 1);                               // 填充数据(8Bit),检查从机应答或非应答信号
    i2c_master_stop(cmd);                                              // 停止传输信号
    i2c_master_cmd_begin(I2C_NUM, cmd, 1000 / portTICK_PERIOD_MS);     // 执行链接
    i2c_cmd_link_delete(cmd);                                          // 释放链接
    return true;
}
