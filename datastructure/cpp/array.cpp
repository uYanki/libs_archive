// 一维数组

#include <assert.h>
#include <cmath>
#include <iostream>

// std::cout << std::endl;

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

// 输出一维数组
template <typename T>
void array_print(T arr[], uint32_t len) {
    for (uint32_t i = 0; i < len; ++i) std::cout << arr[i] << "  ";
    std::cout << std::endl;
}

// 数组基本参数
template <typename T>
void param(T arr[], uint32_t len) {
    assert(arr), assert(len);
    T min, max, sum = arr[0];
    for (uint32_t i = 1; i < len; ++i) {
        if (min > arr[i]) min = arr[i];
        if (max < arr[i]) max = arr[i];
        sum += arr[i];
    }

    T     variance, std = 0;  // 方差 & 标准差(standard deviation,std)
    float mean = sum / (float)len;
    for (uint32_t i = 0; i < len; ++i) { variance += (mean - arr[i]) * (mean - arr[i]); }
    variance /= len;
    std = sqrtf(variance);

    std::cout << "min: " << min << std::endl;
    std::cout << "max: " << max << std::endl;
    std::cout << "sum: " << sum << std::endl;
    std::cout << "mean: " << mean << std::endl;          // 平均值
    std::cout << "std: " << std << std::endl;            // 标准差
    std::cout << "variance: " << variance << std::endl;  // 方差
}

// 加权求和
template <typename T, typename D>
T sum_w(T arr[], D weight[], uint32_t len) {
    assert(arr), assert(weight), assert(len);
    T sum = 0;
    for (uint32_t i = 0; i < len; ++i) sum += arr[i] * weight[i];
    return sum;
}

// 等差数列 arithmetic progression / arithmetic sequence
template <typename T, typename D>
T* ap(T seq[], T a0 /*首项*/, D d /*公差*/, uint32_t len /*项数*/) {
    assert(seq), assert(len);
    seq[0] = a0;
    for (uint32_t i = 1; i < len; ++i) seq[i] = seq[i - 1] + d;
    return seq;
}

// 等比数列 geometric  progression / geometric  sequence
template <typename T, typename D>
T* gp(T seq[], T a0 /*首项*/, D q /*公比*/, uint32_t len /*项数*/) {
    assert(seq), assert(len), assert(q);
    seq[0] = a0;
    for (uint32_t i = 1; i < len; ++i) seq[i] = seq[i - 1] * q;
    return seq;
}

using namespace std;
int main() {
    int arr[15] = {-2793, 12, 356, 7, 22, -1, 5, 2, 3456, 7, 89};
    // array_print(gp(arr, 1, 2, ARRAY_SIZE(arr)), ARRAY_SIZE(arr));
    param(arr, ARRAY_SIZE(arr));
    return 0;
}
