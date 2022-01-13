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

    gpointer val = NULL;

    for (int i = 1; i < 4; ++i) {
        stack->push(stack, item + i);
    }

    printf("GStack::size %d  empty %d \n", stack->size(stack), stack->empty(stack));
    stack->clear(stack);
    while(stack->size(stack)) {
        val = stack->top(stack);
        st_item *it = val;
        printf("GStack: %d   %d  %s\n", it->id, it->grade, it->name);
        stack->pop(stack);
    }
    printf("GStack::size %d  empty %d \n", stack->size(stack), stack->empty(stack));

    for (int i = 0; i < 5; ++i) {
        stack->push(stack, item + i);
    }

    while(stack->size(stack)) {
        val = stack->top(stack);
        st_item *it = val;
        printf("GStack: %d   %d  %s\n", it->id, it->grade, it->name);
        stack->pop(stack);
    }

    printf("GStack::size %d  empty %d \n", stack->size(stack), stack->empty(stack));
    stack->free(stack);
}
