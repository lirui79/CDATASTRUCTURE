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

    GQueue *queue = g_queue_alloc(sizeof(st_item));

    printf("GQueue::size %d  empty %d \n", queue->size(queue), queue->empty(queue));

    for (int i = 1; i < 4; ++i) {
        queue->push(queue, item + i);
    }

    printf("GQueue::size %d  empty %d \n", queue->size(queue), queue->empty(queue));
    queue->clear(queue);
    while(queue->size(queue)) {
        st_item *it = queue->front(queue);
        printf("GQueue::front %d   %d  %s\n", it->id, it->grade, it->name);
        it = queue->back(queue);
        if (it != NULL)
        printf("GQueue::back %d   %d  %s\n", it->id, it->grade, it->name);

        queue->pop(queue);
    }
    printf("GQueue::size %d  empty %d \n", queue->size(queue), queue->empty(queue));

    for (int i = 0; i < 5; ++i) {
        queue->push(queue, item + i);
    }

    while(queue->size(queue)) {
        st_item *it = queue->front(queue);
        printf("GQueue::front %d   %d  %s\n", it->id, it->grade, it->name);
        it = queue->back(queue);
        if (it != NULL)
        printf("GQueue::back %d   %d  %s\n", it->id, it->grade, it->name);

        queue->pop(queue);
    }

    printf("GQueue::size %d  empty %d \n", queue->size(queue), queue->empty(queue));
    queue->free(queue);

}
