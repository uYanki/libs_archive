// 链栈 linkstack

#include <stdio.h>
#include <stdlib.h>
// #define NULL ((void *)0)

typedef unsigned int uint32_t;

typedef struct POINT {
    uint32_t x, y;
} POINT;

#define elem_stack_t POINT  // 指定数据类型

typedef struct Node {
    elem_stack_t data;
    struct Node* next;
} Node;

Node* stack_init();
Node* stack_push(Node* stack, elem_stack_t data);
Node* stack_pop(Node* stack);
Node* stack_free(Node* stack);

unsigned int stack_print(Node* stack);
void example_stack();
