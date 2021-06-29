#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>
#include "test_vector.h"


static void print_vector_int(GVector *ivector) {
    int value = 0;
    GIterator it, end = ivector->end(ivector);
    printf("GVector::size %d  empty %d capacity %d \n", ivector->size(ivector), ivector->empty(ivector), ivector->capacity(ivector));
    for (it = ivector->begin(ivector); it.less(&it, &end); it.next(&it)) {
        value = *(int *) it.data(&it);
        printf("%x ", value);
    }
    printf("\n");
}

static void print_vector_int_r(GVector *ivector) {
    int value = 0;
    GIterator it, rend = ivector->rend(ivector);
    printf("GVector::size %d  empty %d capacity %d \n", ivector->size(ivector), ivector->empty(ivector), ivector->capacity(ivector));
    for (it = ivector->rbegin(ivector); it.greater(&it, &rend); it.next(&it)) {
        value = *(int *) it.data(&it);
        printf("%x ", value);
    }
    printf("\n");
}

static void test_vector_int() {
    GVector *ivector = g_vector_alloc(8,sizeof(int));
    int value = 0x11;
    ivector->resize(ivector, 20, &value);
    print_vector_int(ivector);
    print_vector_int_r(ivector);
    value = *(int*) ivector->data(ivector);
    printf("GVector first %x\n", value);

    int buf[] = {0x21, 0x22, 0x43, 0x101020, 0x032390};
    GIterator first = ivector->begin(ivector), last = ivector->end(ivector);
    first.set(&first, buf, sizeof(int));
    last.set(&last, &buf[5], sizeof(int));
    ivector->assign(ivector, first, last);
    print_vector_int(ivector);
    print_vector_int_r(ivector);

    value = *(int*) ivector->data(ivector);
    printf("GVector first %x\n", value);

    ivector->clear(ivector);
    printf("GVector::size %d  empty %d capacity %d \n", ivector->size(ivector), ivector->empty(ivector), ivector->capacity(ivector));

    ivector->free(ivector);
}






void test_vector() {
   test_vector_int();
}
