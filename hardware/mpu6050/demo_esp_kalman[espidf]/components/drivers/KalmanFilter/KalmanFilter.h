#ifndef _KALMANFILTER_H_
#define _KALMANFILTER_H_

// 卡尔曼结构体
typedef struct
{
    float Q_angle;   // 加速计的过程噪声协方差
    float Q_bias;    // 陀螺仪偏差的过程噪声协方差
    float R_measure; // 测量噪声协方差
    float angle;     // 由卡尔曼滤波器计算的角度-2x1状态向量的一部分
    float bias;      // 由卡尔曼滤波器计算的偏差-2x1状态向量的一部分
    float P[2][2];   // 误差协方差矩阵-这是一个2x2矩阵
} Kalman_t;

float Kalman_getAngle(Kalman_t *Kalman, float newAngle, float newRate, float dt);

#endif