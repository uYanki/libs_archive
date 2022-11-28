#include "array2d.h"

array2d* array2d_init(uint32_t w, uint32_t h) {
    assert(w), assert(h);  // 宽高不为0
    array2d* arr = malloc(sizeof(array2d));
    int size = sizeof(elem_array2d_t) * w * h;
    arr->p = malloc(size);
    if (arr->p == NULL) {  //分配内存失败
        free(arr);
        assert(0);
        return NULL;
    }
    arr->w = w;
    arr->h = h;
    memset(arr->p, 0, size);
    return arr;
}

void array2d_set(array2d* arr, uint32_t x, uint32_t y, elem_array2d_t data) {
    assert(x < arr->w), assert(y < arr->h);
    arr->p[y * arr->w + x] = data;
}

elem_array2d_t array2d_get(array2d* arr, uint32_t x, uint32_t y) {
    assert(x < arr->w), assert(y < arr->h);
    return arr->p[y * arr->w + x];
}

elem_array2d_t* array2d_get_ex(array2d* arr, uint32_t x, uint32_t y) {
    assert(x < arr->w), assert(y < arr->h);
    return arr->p + y * arr->w + x;
}

array2d* array2d_free(array2d* arr) {
    assert(arr);
    free(arr->p);
    free(arr);
    arr = NULL;
    return NULL;
}

uint32_t array2d_print(array2d* arr) {
    uint32_t start_x = 0, start_y = 0;
    uint32_t end_x = arr->w, end_y = arr->h;
    printf("[");
    for (int x = start_x; x < end_x; ++x) {
        printf("[");
        for (int y = start_y; y < end_y; ++y) {
            // printf("%4d ", arr->p[y * arr->w + x]);
            block elem = arr->p[y * arr->w + x];
            printf("[%d,%d,%d,%d,1]", elem.left, elem.up, elem.right, elem.down);
            if (y != end_y - 1) printf(",");
        }
        printf("],\n");
    }
    printf("]\n");
}

// void example_array() {
//     array2d* mat = array2d_init(6, 6);
//     for (int i = 0; i < 6; i++) { array2d_set(mat, i, i, i + 1); }
//     array2d_print(mat);
//     mat = array2d_free(mat);
// }
