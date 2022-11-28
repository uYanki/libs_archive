#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned int uint32_t;
typedef unsigned int uint8_t;

typedef struct block {
    uint8_t left, up, right, down;  // 四周是否有墙
    uint8_t visited;                // 是否已访问过该点
} block;

#define elem_array2d_t block  // 指定类型

typedef struct array2d {
    uint32_t w, h;      // width & height
    elem_array2d_t* p;  // pointer of table
} array2d;

array2d* array2d_init(uint32_t w, uint32_t h);
void array2d_set(array2d* arr, uint32_t x, uint32_t y, elem_array2d_t data);
elem_array2d_t array2d_get(array2d* arr, uint32_t x, uint32_t y);
elem_array2d_t* array2d_get_ex(array2d* arr, uint32_t x, uint32_t y);
array2d* array2d_free(array2d* arr);

uint32_t array2d_print(array2d* arr);
void example_array();
