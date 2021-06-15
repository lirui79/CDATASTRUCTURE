#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>
#include "test_vector.h"


static void print_vector_int(GVector *ivector) {
    int value = 0;
    gpointer it = NULL;
    printf("GVector::size %d  empty %d capacity %d \n", ivector->size(ivector), ivector->empty(ivector), ivector->capacity(ivector));
    for (it = ivector->begin(ivector); it < ivector->end(ivector); it = ivector->backward(ivector, it, 1)) {
        value = *(int *) it;
        printf("%d ", value);
    }
    printf("\n");
}

static void print_vector_int_r(GVector *ivector) {
    int value = 0;
    gpointer it = NULL;
    printf("GVector::size %d  empty %d capacity %d \n", ivector->size(ivector), ivector->empty(ivector), ivector->capacity(ivector));
    for (it = ivector->rbegin(ivector); it > ivector->rend(ivector); it = ivector->forward(ivector, it, 1)) {
        value = *(int *) it;
        printf("%d ", value);
    }
    printf("\n");
}

void test_vector_int() {
    GVector *ivector = g_vector_alloc(8,sizeof(int));
    int value = 11;
    ivector->resize(ivector, 20, &value);
    print_vector_int(ivector);
    print_vector_int_r(ivector);
    value = *(int*) ivector->data(ivector);
    printf("GVector first %d\n", value);

    int buf[] = {0x21, 0x22, 0x43, 0x101020, 0x032390};
    ivector->assign(ivector, buf, buf + 5);
    print_vector_int(ivector);
    print_vector_int_r(ivector);

    value = *(int*) ivector->data(ivector);
    printf("GVector first %d\n", value);

    ivector->clear(ivector);
    printf("GVector::size %d  empty %d capacity %d \n", ivector->size(ivector), ivector->empty(ivector), ivector->capacity(ivector));

    ivector->free(ivector);
}






void test_vector() {
   test_vector_int();
}
