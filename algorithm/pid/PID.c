#include <stdio.h>

// PID: Proportional(比例)、Integral(积分)、Differential(微分)
// 百度百科: https://baike.baidu.com/item/PID算法/4660106

// bilibili:
// 基础教程
//  https://www.bilibili.com/video/BV1M341127Dm
//  https://www.bilibili.com/video/BV1wh411y78M
// 调参过程
//  https://www.bilibili.com/video/BV1zo4y1D7bx

/*

P: 减小目标值与测量值的差值
D: 阻尼作用, 抑制过冲
I: 消除稳态误差

*/

typedef struct {
    float kp, ki, kd;
    float expect; /* 目标值 */
    float error;  /* 误差值 */
    float sum;    /* 误差积分 */
} PID_t;

float _PID_CALC_(PID_t hPID, float collect /* 测量值 */) {
    float result, error = hPID.expect - collect;
    result = hPID.kp * error +
             hPID.ki * (hPID.sum += error) +  // 积分项
             hPID.kd * (error - hPID.error);  // 微分项
    hPID.error = error;
    return result;
}

int main() {}