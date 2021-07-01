#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>
#include "test_list_alloc.h"

typedef struct {
   unsigned int   type;
   int             idx;
   unsigned long  len;
   unsigned char  data[0];
}  Msg_t;

static void print_Msg_t(Msg_t *msg) {
   if(msg == NULL)
      return ;
   printf("Msg:type %X idx %X len %ld data;", msg->type, msg->idx, msg->len);
   for (int i = 0; i < msg->len; ++i) {
      printf("%02X ", msg->data[i]);
   }
   printf("\n");
}

static void print_begintoend(GList *list) {
   GIterator  iterator = list->begin(list), end = list->end(list);
   Msg_t *msg = NULL;
   printf("GList: empty %d  size %d\n", list->empty(list), list->size(list));
   for (; iterator.not_equal(&iterator, &end);iterator.next(&iterator)) {
        msg = iterator.data(&iterator).data;
        print_Msg_t(msg);
   }
}

static void print_rbegintorend(GList *list) {
   GIterator  iterator = list->rbegin(list), end = list->rend(list);
   Msg_t *msg = NULL;
   printf("GList: empty %d  size %d\n", list->empty(list), list->size(list));
   for (iterator = list->rbegin(list); iterator.not_equal(&iterator, &end); iterator.next(&iterator)) {
        msg = iterator.data(&iterator).data;
        print_Msg_t(msg);
   }
}

static void test_list_nodes(GList *list) {
    GReference val = list->front(list);
    Msg_t *msg =  val.data, *msg1 = NULL;
    int size;
    print_Msg_t(msg);

    list->pop_front(list);

    val = list->back(list);
    msg = val.data;
    print_Msg_t(msg);

    list->pop_back(list);

    print_begintoend(list);

    val = list->at(list, 8);
    msg = val.data;

    size = val.size;

    msg1 = malloc(size);

    memcpy(msg1, msg, size);

    print_Msg_t(msg1);

    GIterator pos = list->find(list, val);

    val = pos.data(&pos);

    printf("msg %p  %p : 8\n", msg, val.data);

    list->remove(list,  pos);

    free(msg1);

    print_begintoend(list);
    int headSize = sizeof(unsigned int) +  sizeof(unsigned long) +  sizeof(int);
    int dataSize = 4 + 30;
    int index = 8;
    msg = malloc(headSize + dataSize);
    msg->type = 0x2000 + index;
    msg->idx  = 0x20000 + index;
    msg->len  = dataSize;
    for(int j = 0; j < msg->len; ++j) {
        msg->data[j] = 0x30 * index + j;
    }

    val.data = msg;
    val.size = headSize + dataSize;
    list->push_front(list, val);
    //it.set(&it, msg, headSize + dataSize);

    free(msg);

    print_begintoend(list);
}

static void test_list_create_data(GList *list, GList *list1) {
    GReference val;
    Msg_t *msg = NULL;
    int headSize = sizeof(unsigned int) +  sizeof(unsigned long) +  sizeof(int);
    for (int i = 0; i < 13; ++i) {
        int dataSize = 4 + i;
        msg = malloc(headSize + dataSize);
        msg->type = 0x1000 + i;
        msg->idx  = 0x10000 + i;
        msg->len  = dataSize;
        for(int j = 0; j < msg->len; ++j) {
            msg->data[j] = 0x10 * i + j;
        }
        val.data = msg;
        val.size = headSize + dataSize;
        list->push_back(list, val);
        free(msg);
    }

    print_begintoend(list);
    print_rbegintorend(list);
    test_list_nodes(list);

    list->reverse(list);
    print_begintoend(list);
    list1->assign(list1, list->begin(list), list->end(list));

    list->clear(list);
    print_begintoend(list1);
    print_begintoend(list);
    list1->clear(list1);
    print_rbegintorend(list1);

    list->free(list);
    list1->free(list1);
}

static void test_list_alloc_1() {
    GList  *list, *list1 = NULL;
    list  = g_list_alloc();
    list1 = g_list_alloc();
    test_list_create_data(list, list1);
}



void test_list_alloc() {
   test_list_alloc_1();
}
