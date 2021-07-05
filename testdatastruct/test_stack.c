#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>
#include "test_stack.h"


typedef struct st_item {
    int    id;
    int    grade;
    char   name[32];
} st_item;


void test_stack() {
    GStack *stack = g_stack_alloc(8, sizeof(st_item));
    st_item item[5] = {
        {
            0,
            1,
            "lirui",
        },
        {
            1,
            2,
            "stone",
        },
        {
            2,
            3,
            "steel",
        },
        {
            3,
            4,
            "iron",
        },
        {
            4,
            5,
            "snow",
        },
    };

    GType val = {NULL, 0};

    for (int i = 1; i < 4; ++i) {
        val.data = item + i;
        val.size = sizeof(st_item);
        stack->push(stack, val);
    }

    printf("GStack::size %d  empty %d \n", stack->size(stack), stack->empty(stack));
    stack->clear(stack);
    while(stack->size(stack)) {
        val = stack->top(stack);
        st_item *it = val.data;
        printf("GStack: %d   %d  %s\n", it->id, it->grade, it->name);
        stack->pop(stack);
    }
    printf("GStack::size %d  empty %d \n", stack->size(stack), stack->empty(stack));

    for (int i = 0; i < 5; ++i) {
        val.data = item + i;
        val.size = sizeof(st_item);
        stack->push(stack, val);
    }

    while(stack->size(stack)) {
        val = stack->top(stack);
        st_item *it = val.data;
        printf("GStack: %d   %d  %s\n", it->id, it->grade, it->name);
        stack->pop(stack);
    }

    printf("GStack::size %d  empty %d \n", stack->size(stack), stack->empty(stack));
    stack->free(stack);
}
