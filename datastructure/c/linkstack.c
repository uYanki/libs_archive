#include "linkstack.h"

Node* stack_init() { return NULL; }

Node* stack_push(Node* stack, elem_stack_t data) {
    Node* p = (Node*)malloc(sizeof(Node));
    p->data = data;
    p->next = stack;
    return p;
}

Node* stack_pop(Node* stack) {
    if (stack) {
        Node* p = stack;
        stack = stack->next;
        // printf("%d\n", p->data);
        free(p);
    }
    return stack;
}

Node* stack_free(Node* stack) {
    Node* p;
    while (p = stack) {
        stack = stack->next;
        free(p);
    }
    return NULL;
}

unsigned int stack_print(Node* stack) {
    unsigned int cnt = 0;
    Node* p;
    while (p = stack) {
        // printf("%d\n", p->data);
        stack = stack->next;
        cnt++;
    }
    // printf("count:%d", cnt);
    return cnt;
}

// void example_stack() {
//     Node* stack = stack_init();
//     stack = stack_push(stack, 1);
//     stack = stack_push(stack, 2);
//     stack = stack_push(stack, 3);
//     stack = stack_pop(stack);
//     stack_print(stack);
//     stack = stack_free(stack);
// }
