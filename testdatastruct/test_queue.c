#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>
#include "test_queue.h"


typedef struct st_item {
    int    id;
    int    grade;
    char   name[32];
} st_item;



void test_queue() {
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

    GQueue *queue = g_queue_alloc();

    printf("GQueue::size %d  empty %d \n", queue->size(queue), queue->empty(queue));


    GType val = {NULL, 0};

    for (int i = 1; i < 4; ++i) {
        val.data = item + i;
        val.size = sizeof(st_item);
        queue->push(queue, val);
    }

    printf("GQueue::size %d  empty %d \n", queue->size(queue), queue->empty(queue));
    queue->clear(queue);
    while(queue->size(queue)) {
        val = queue->front(queue);
        st_item *it = val.data;
        printf("GQueue::front %d   %d  %s\n", it->id, it->grade, it->name);
        val = queue->back(queue);
        it = val.data;
        if (it != NULL)
        printf("GQueue::back %d   %d  %s\n", it->id, it->grade, it->name);

        queue->pop(queue);
    }
    printf("GQueue::size %d  empty %d \n", queue->size(queue), queue->empty(queue));

    for (int i = 0; i < 5; ++i) {
        val.data = item + i;
        val.size = sizeof(st_item);
        queue->push(queue, val);
    }

    while(queue->size(queue)) {
        val = queue->front(queue);
        st_item *it = val.data;
        printf("GQueue::front %d   %d  %s\n", it->id, it->grade, it->name);
        val = queue->back(queue);
        it = val.data;
        if (it != NULL)
        printf("GQueue::back %d   %d  %s\n", it->id, it->grade, it->name);

        queue->pop(queue);
    }

    printf("GQueue::size %d  empty %d \n", queue->size(queue), queue->empty(queue));
    queue->free(queue);

}
