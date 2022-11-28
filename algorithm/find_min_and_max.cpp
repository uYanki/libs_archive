// 用途: 滑动窗口寻找区间最小最大值

#include <iostream>
#include <queue>

using namespace std;

template <typename T>
int find_min_and_max(T arr[], size_t len, size_t wnd_size) {
    // wnd_size: size of sliding window 滑动窗口的长度

    deque<int> q_min /*单调递减队列*/, q_max /*单调递增队列*/;

    std::cout << "min  max" << std::endl;

    for (int i = 0; i < len; ++i) {
        if (i >= wnd_size) {
            // 当前滑动窗口的最小/最大值为队列的首元素
            std::cout << q_min.front() << "  " << q_max.front() << std::endl;
            // 当前元素为队列中最小/最大的元素时,则出队
            if (arr[i - wnd_size] == q_min.front()) q_min.pop_front();
            if (arr[i - wnd_size] == q_max.front()) q_max.pop_front();
        }

        // 队列非空,且最后元素大于/小于当前元素,则出队最后元素,因为在当前位置,其不可能成为最小/最大元素
        while (!q_min.empty() && q_min.back() > arr[i]) q_min.pop_back();
        while (!q_max.empty() && q_max.back() < arr[i]) q_max.pop_back();

        // 当前元素入队
        q_min.push_back(arr[i]);
        q_max.push_back(arr[i]);
    }
    std::cout << q_min.front() << "  " << q_max.front() << std::endl;
    return 0;
}

int _main() {
    int arr[] = {5, 2, 6, 8, 10, 7, 4};
    int len   = sizeof(arr) / sizeof(int);  // number of elements
    find_min_and_max(arr, len, 4);
    return 0;
}
