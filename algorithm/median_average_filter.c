#include <stdio.h>

typedef unsigned char uint8_t;

// @ 递推中值平均滤波 - 原理:
// 构建长度为N的队列, 新值加入队尾并移出队首的旧值.
// 对队列的数值进行排序, 去掉最大值和最小值, 取中间剩下的值的平均数作为最终结果.

#define filter_size 3  // filter_size > 2
#define filter_type float
filter_type filter(filter_type new_value) {
    static uint8_t     idx;
    static filter_type ring_arr[filter_size];
    static filter_type min, max, sum;

    // flag of ring array's initialization (1:doing,0:finished)
    static uint8_t bInit = 1;

    if (bInit && idx == 0) {
        // initialize maximum and minimum values
        min = max = new_value;
    } else {
        // update maximum and minimum values
        if (min > new_value)
            min = new_value;
        else if (max < new_value)
            max = new_value;
    }

    // store new value into ring array
    sum += (ring_arr[idx++] = new_value);

    if (bInit) {
        // before completely initializing the ring array, return 0
        if (idx < filter_size)
            return 0;
        // initialization complete
        bInit = 0;
        // reset idx to 0
        idx = 0;
    } else {
        // limit the range of idx from 0 to size-1
        if (idx == filter_size) idx = 0;
        // remove old value
        sum -= ring_arr[idx];
    }

    return (sum - min - max) / (filter_size - 2);
}

int main() {
    int i;

    for (i = 1; i < 15; i += 2)
        printf("(%d)%.2f ", i, filter(i));

    return 0;
}