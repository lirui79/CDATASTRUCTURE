#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>
#include "test_deque.h"



typedef struct st_item {
    int    id;
    int    grade;
    char   name[32];
} st_item;



static void test_deque_one() {
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

    GDeque *queue = g_deque_alloc(sizeof(st_item));

    printf("GDeque::size %d  empty %d \n", queue->size(queue), queue->empty(queue));


    GType val = {NULL, 0};

    for (int i = 1; i < 4; ++i) {
        val.data = item + i;
        val.size = sizeof(st_item);
        queue->push_back(queue, val);
        queue->push_front(queue, val);
    }

    printf("GDeque::size %d  empty %d \n", queue->size(queue), queue->empty(queue));
    queue->clear(queue);
    while(queue->size(queue)) {
        val = queue->front(queue);
        st_item *it = val.data;
        printf("GDeque::front %d   %d  %s\n", it->id, it->grade, it->name);
        val = queue->back(queue);
        it = val.data;
        if (it != NULL)
        printf("GDeque::back %d   %d  %s\n", it->id, it->grade, it->name);

        queue->pop_front(queue);
        queue->pop_back(queue);
    }
    printf("GDeque::size %d  empty %d \n", queue->size(queue), queue->empty(queue));

    for (int i = 0; i < 5; ++i) {
        val.data = item + i;
        val.size = sizeof(st_item);
        queue->push_back(queue, val);
        queue->push_front(queue, val);
    }

    for (int i = 0 ; i < queue->size(queue); ++i) {
        val = queue->at(queue, i);
        st_item *it = val.data;
        printf("GDeque::item %d   %d  %s\n", it->id, it->grade, it->name);
    }

    while(queue->size(queue)) {
        val = queue->front(queue);
        st_item *it = val.data;
        printf("GDeque::front %d   %d  %s\n", it->id, it->grade, it->name);
        val = queue->back(queue);
        it = val.data;
        if (it != NULL)
        printf("GDeque::back %d   %d  %s\n", it->id, it->grade, it->name);
        queue->pop_front(queue);
        queue->pop_back(queue);
    }

    printf("GDeque::size %d  empty %d \n", queue->size(queue), queue->empty(queue));
    queue->free(queue);

}

static void test_deque_two() {
    GDeque *queue = g_deque_alloc(sizeof(int));
    GDeque *deque = g_deque_alloc(sizeof(int));
    GType   val = {NULL, sizeof(int)};


    printf("GDeque::size %d  empty %d \n", queue->size(queue), queue->empty(queue));
    int i = 0;
    for (i = 0; i < 13; ++i) {
        val.data = &i;
        queue->push_back(queue, val);
    }

    printf("GDeque::size %d  empty %d \n", queue->size(queue), queue->empty(queue));
    GIterator it = queue->begin(queue), end = queue->end(queue);
    for (it = queue->begin(queue); it.not_equal(&it, &end); it.next(&it)) {
        int value = *(int*) it.data(&it).data;
        printf("GDeque::item %d \n", value);
    }

    deque->assign(deque, queue->begin(queue), queue->end(queue));
    printf("GDeque::size %d  empty %d \n", deque->size(deque), deque->empty(deque));

    queue->erase(queue, queue->begin(queue), queue->end(queue));
    printf("GDeque::size %d  empty %d \n", queue->size(queue), queue->empty(queue));
    queue->clear(queue);

    printf("GDeque::size %d  empty %d \n", deque->size(deque), deque->empty(deque));
    for (i = 0 ; i < deque->size(deque); ++i) {
        val = deque->at(deque, i);
        int value = *(int*) val.data;
        printf("GDeque::item %d \n", value);
    }

    end = deque->end(deque);
    it = deque->begin(deque);
    it.next(&it);
    deque->remove(deque, it);
    it = deque->begin(deque);
    for (; it.not_equal(&it, &end); it.next(&it)) {
        int value = *(int*) it.data(&it).data;
        printf("GDeque::item %d \n", value);
    }

    queue->insert(queue, queue->begin(queue), deque->begin(deque), deque->end(deque));
    printf("GDeque::size %d  empty %d \n", queue->size(queue), queue->empty(queue));
    it = queue->begin(queue), end = queue->end(queue);
    for (it = queue->begin(queue); it.not_equal(&it, &end); it.next(&it)) {
        int value = *(int*) it.data(&it).data;
        printf("GDeque::item %d \n", value);
    }

    int v1 = 100;
    val.data = &v1;

    queue->fill(queue, queue->begin(queue), 10, val);
    queue->resize(queue, 24, val);
    printf("GDeque::size %d  empty %d \n", queue->size(queue), queue->empty(queue));
    it = queue->begin(queue), end = queue->end(queue);
    for (it = queue->begin(queue); it.not_equal(&it, &end); it.next(&it)) {
        int value = *(int*) it.data(&it).data;
        printf("GDeque::item %d \n", value);
    }



    queue->free(queue);
    deque->free(deque);
}


void test_deque() {
    test_deque_one();
    test_deque_two();
}
