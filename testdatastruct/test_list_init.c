
#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>
#include "test_list_init.h"

typedef struct {
   unsigned int   type;
   int             idx;
   unsigned long  len;
   unsigned char  data[0];
}  Msg_t;

static void print_Msg_t(Msg_t *msg) {
   if(msg == NULL)
      return ;
   printf("Msg:type %X idx %X len %d data;", msg->type, msg->idx, msg->len);
   for (int i = 0; i < msg->len; ++i) {
      printf("%02X ", msg->data[i]);
   }
   printf("\n");
}

static void print_begintoend(GList *list) {
   GNode  *node = NULL;
   Msg_t *msg = NULL;
   printf("GList: empty %d  size %d\n", list->empty(list), list->size(list));
   for (node = list->begin(list); node != list->end(list); node = node->next(node)) {
        msg = node->data(node);
        print_Msg_t(msg);
   }
}

static void print_rbegintorend(GList *list) {
   GNode  *node = NULL;
   Msg_t *msg = NULL;
   printf("GList: empty %d  size %d\n", list->empty(list), list->size(list));
   for (node = list->rbegin(list); node != list->rend(list); node = node->prev(node)) {
        msg = node->data(node);
        print_Msg_t(msg);
   }
}

static void test_list_nodes(GList *list) {
    GNode *node = list->front(list);
    Msg_t *msg = node->data(node);
    print_Msg_t(msg);

    list->pop_front(list);

    node = list->back(list);
    msg  = node->data(node);

    print_Msg_t(msg);

    list->pop_back(list);

    print_begintoend(list);

    node = list->at(list, 8);

    msg  = node->data(node);

    print_Msg_t(msg);

    int index = list->find(list, node->data(node), node->size(node));

    printf("index = %d  : 8\n", index);

    list->remove(list, node->data(node), node->size(node));

    print_begintoend(list);
    int headSize = sizeof(unsigned int) +  sizeof(unsigned long) +  sizeof(int);
    int dataSize = 4 + 30;
    msg = malloc(headSize + dataSize);
    msg->type = 0x2000 + index;
    msg->idx  = 0x20000 + index;
    msg->len  = dataSize;
    for(int j = 0; j < msg->len; ++j) {
        msg->data[j] = 0x30 * index + j;
    }

    list->insert(list, index, msg, headSize + dataSize);

    print_begintoend(list);
}

/*
static void test_list_init_1() {
    GList  list, list1;
    g_list_init(&list);
    g_list_init(&list1);

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

        list.push_back(&list, msg, headSize + dataSize);
    }

    print_begintoend(&list);
    print_rbegintorend(&list);
    test_list_nodes(&list);

    list.reverse(&list);
    print_begintoend(&list);
    list.copy(&list, &list1);

    list.free(&list);
    print_begintoend(&list1);
    print_begintoend(&list);
    list.free(&list1);
    print_rbegintorend(&list1);


}
*/










void test_list_init() {
    //test_list_init_1() ;



}
