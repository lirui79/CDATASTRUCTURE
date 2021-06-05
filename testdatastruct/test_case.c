#include <stdio.h>
#include <stdlib.h>

#include "../cstl/glist.h"

typedef struct Msg_s {
    int  type;
    int  id;
    int  len;
    struct {
        unsigned char data[16];
    };
}Msg_t;


extern void print_msg(Msg_t* msg);


void print_msg(Msg_t* msg) {
    printf("msg:%d %d %d ( ", msg->type, msg->id, msg->len);
    for (int i = 0 ; i < msg->len; ++i)
        printf("%02x ", msg->data[i]);
    printf(")\n");
}


void test_list_init_1() {
    GList list, *listptr = NULL;
    Msg_t  *msg = NULL;
    g_list_init(&list);
    listptr = g_list_alloc();

    printf("list: %d   %d\n", list.empty(&list), list.size(&list));

    for (int i = 0 ; i < 10; ++i) {
        msg = malloc(sizeof(Msg_t));
        msg->type = i + 0x10;
        msg->id   = i + 1;
        msg->len  = 0x10;
        for (int j = 0; j < msg->len; ++j)
            msg->data[j] = j + i * 0x10;
        list.push_back(&list, msg, sizeof(Msg_t));
    }

    printf("list: %d   %d\n", list.empty(&list), list.size(&list));
    GNode* node=NULL;
    for (node = list.begin(&list); node != list.end(&list); node = g_node_next(node)) {
        print_msg((Msg_t*)g_node_data(node));
    }

    list.free(&list);
    printf("list: %d   %d\n", list.empty(&list), list.size(&list));
}


void test_list_init() {
    test_list_init_1();
}
