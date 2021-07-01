#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>
#include "test_array.h"



static void print_array_int(GArray *iarray) {
    int value = 0;
    GIterator it, end = iarray->end(iarray);
    printf("GArray::size %d \n", iarray->size(iarray));
    for (it = iarray->begin(iarray); it.less(&it, &end); it.next(&it)) {
        value = *(int *) it.get(&it).data;
        printf("%x ", value);
    }
    printf("\n");
}

static void print_array_int_r(GArray *iarray) {
    int value = 0;
    GIterator it, rend = iarray->rend(iarray);
    printf("iarray::size %d \n", iarray->size(iarray));
    for (it = iarray->rbegin(iarray); it.greater(&it, &rend); it.next(&it)) {
        value = *(int *) it.get(&it).data;
        printf("%x ", value);
    }
    printf("\n");
}


static void test_array_int() {
    GArray *iarray = g_array_alloc(32,sizeof(int));
    int value = 0x33;
    GReference ref = {&value, sizeof(int)};
    iarray->fill(iarray, ref);
    print_array_int(iarray);
    print_array_int_r(iarray);
    ref = iarray->data(iarray);
    value = *(int*) ref.data;
    printf("GArray first %x\n", value);

    int buf[] = {0x45, 0x32, 0x65, 0x223232, 0x78797};
    GIterator first = iarray->begin(iarray), last = iarray->end(iarray);
    ref.data = buf;
    first.set(&first, ref);
    ref.data = buf + 5;
    last.set(&last, ref);
    iarray->assign(iarray, first, last);
    print_array_int(iarray);
    print_array_int_r(iarray);

    ref = iarray->data(iarray);
    value = *(int*) ref.data;
    printf("GArray first %x\n", value);

    printf("GArray::size %d \n", iarray->size(iarray));

    iarray->free(iarray);
}





void test_array() {
    test_array_int();
}
