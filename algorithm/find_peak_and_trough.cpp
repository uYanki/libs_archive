// 用途: 二阶差分寻找波峰波谷

// 说明: 二阶导, f''>0 函数是凹的, 对应 trough; f''<0 函数是凸的, 对应 peak

#include <assert.h>
#include <stdio.h>
#include <iostream>
using namespace std;

// 输出数组
template <typename T>
void print_array(T arr[], size_t len, bool align = false, const char* str = "") {
    if (align) printf("     ");
    for (size_t i = 0; i < len; i++) { printf(" %3d ", arr[i]); }
    printf("   %s", str);
    std::cout << std::endl;
}

// 符号函数
template <typename T>
inline int sign(T val) {
    if (val > 0) return 1;
    if (val < 0) return -1;
    return 0;
}

// peak 波峰, trough 波谷
template <typename T>
void find_peak_and_trough(T arr[], size_t len) {
    assert(len > 1);  // 至少两项
    print_array(arr, len, false, "origin");

    len -= 1;
    T* diff = new T[len];

    // 一阶差分
    for (size_t i = 0; i < len; ++i)  // 前向差分
        diff[i] = sign(arr[i + 1] - arr[i]);
    print_array(diff, len, true, "diff 1");

    for (size_t i = 0; i < len - 1; ++i)  // 处理平坡
        if (diff[i] == 0) diff[i] = (diff[i + 1] >= 0) ? 1 : -1;
    if (diff[len - 1] == 0)  //处理尾项
        diff[len - 1] = (diff[len - 2] >= 0) ? 1 : -1;
    print_array(diff, len, true, "remove zero");

    // 二阶差分
    for (size_t i = 0; i < len - 1; ++i)  // 前向差分
        diff[i] = diff[i + 1] - diff[i];
    print_array(diff, len, true, "diff 2");

    // 输出结果
    for (size_t i = 0; i < len; ++i) {
        if (diff[i] == -2) printf("peak:%d,%d\n", arr[i + 1], i + 1);
        if (diff[i] == 2) printf("trough:%d,%d\n", arr[i + 1], i + 1);
    }

    delete diff;
}

int main() {
    int nums[] = {1, 2, 3, 4, 4, 4, 5, 2, 1, 0, 0, 5, 1, 0, 0, 1, 2, 3, 4, 0};
    int cnt = sizeof(nums) / sizeof(int);  // number of elements
    find_peak_and_trough(nums, cnt);
    return 0;
}
