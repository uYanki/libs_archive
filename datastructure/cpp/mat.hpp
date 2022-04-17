#pragma once

#include <assert.h>
#include <iostream>

void* fillzero(void* dest, uint32_t len) {
    assert(dest);
    uint8_t* p = (uint8_t*)dest;
    while (len--) *p++ = 0;
    return dest;
}

template <typename T>
class mat {
private:
    T* p;           // pointer of table
    uint32_t w, h;  // width(col) & height(row)
public:
    mat(uint32_t width, uint32_t height);
    ~mat();

    void set(uint32_t row, uint32_t col, T data);
    T get(uint32_t row, uint32_t col);

    uint32_t width() { return w; }
    uint32_t height() { return h; }

    T* operator[](int col) { return p + w * col; }

    void print();
};

template <typename T>
mat<T>::mat(uint32_t width, uint32_t height) {
    w = width, h = height;
    assert(w), assert(h);  // not zero
    p = new T[w * h];
    assert(p);  // alloc fail
    fillzero(p, sizeof(T) * w * h);
    // sizeof -> count of bytes
}
template <typename T>
mat<T>::~mat() {
    if (p) {
        delete p;
        p = NULL;
    };
};

template <typename T>
void mat<T>::set(uint32_t row, uint32_t col, T data) {
    assert(col < w), assert(row < h);
    p[row * w + col] = data;
}
template <typename T>
T mat<T>::get(uint32_t row, uint32_t col) {
    assert(col < w), assert(row < h);
    return p[row * w + col];
}

template <typename T>
void mat<T>::print() {
    uint32_t start_col = 0, start_row = 0;
    uint32_t end_col = w, end_row = h;
    for (int col = start_col; col < end_col; ++col) {
        for (int row = start_row; row < end_row; ++row)  //
            std::cout << p[row * w + col] << "  ";
        std::cout << std::endl;
    }
}
