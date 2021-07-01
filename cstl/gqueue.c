#include "gqueue.h"
#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>


typedef struct _GDQNode   GDQNode;

struct _GDQNode {
    GDQNode           *next;
    GDQNode           *prev;
    GType              data;// data pointer //  data size
};


static GDQNode*          g_node_init(GDQNode *thiz, gpointer data, guint size) {
    GType     val = {data, size};
    thiz->next  = NULL;
    thiz->prev  = NULL;
    thiz->data  = val;
    return thiz;
}


static GDQNode*          g_node_alloc(GType val) {
    GDQNode *thiz = malloc(sizeof(GDQNode));
    if(val.data == NULL || val.size <= 0) {
        return g_node_init(thiz, NULL, 0);
    }
    gpointer newdata = malloc(val.size);
    memcpy(newdata, val.data, val.size);
    return g_node_init(thiz, newdata, val.size);
}

static GType      g_node_data(GDQNode  *thiz) {
    return thiz->data;
}


static void           g_node_free(GDQNode  *thiz) {
    if (thiz->data != NULL)
        free(thiz->data);
    free(thiz);
}

static GDQNode*          g_node_insert(GDQNode  *thiz, GDQNode  *newthis) {
    GDQNode *next  = thiz->next;
    thiz->next    = newthis;
    next->prev     = newthis;
    newthis->next  = next;
    newthis->prev  = thiz;
    return thiz;
}


///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
typedef struct _GDQueue  GDQueue;

struct _GDQueue {
    GQueue                 thiz;
    GDQNode                head;// list  head  no data      next -> first node
    guint                  size;
};


static    void g_queue_free(GQueue *thiz) { // free thiz
    GDQueue* gthis = (GDQueue*) thiz;
    GDQNode *node = NULL, *next = NULL;
    GType     val = {NULL, 0};
    node = gthis->head.next;
    while (node != &(gthis->head)) {
        next = node->next;
        g_node_free(node);
        node = next;
    }

    gthis->head.next = &(gthis->head);
    gthis->head.prev = &(gthis->head);
    thiz->head.data  = val;
    gthis->size      = 0;
}

static    void g_queue_clear(GQueue *thiz) {
    GDQueue* gthis = (GDQueue*) thiz;
    g_queue_free(thiz);
    free(gthis);
}

static    guint  g_queue_size(GQueue *thiz) {
    GDQueue* gthis = (GDQueue*) thiz;
    return gthis->size;
}

static    guint  g_queue_empty(GQueue *thiz) {
    GDQueue* gthis = (GDQueue*) thiz;
    if (gthis->size <= 0)
        return 1;
    return 0;
}

static    GType g_queue_front(GQueue *thiz) {
    GDQueue* gthis = (GDQueue*) thiz;
    GDQNode *node = gthis->head.next;
    return node->data;
}

static    GType g_queue_back(GQueue *thiz) {
    GDQueue* gthis = (GDQueue*) thiz;
    GDQNode *node = gthis->head.prev;
    return node->data;
}

static    void    g_queue_push(GQueue *thiz, GType val) {
    GDQueue* gthis = (GDQueue*) thiz;
    GDQNode *node  = g_node_alloc(val);
    GDQNode *prev  = gthis->head.prev;
    g_node_insert(prev, node);
    gthis->size    =  gthis->size + 1;
}

static    void    g_queue_pop(GQueue *thiz) {
    GDQueue* gthis = (GDQueue*) thiz;
    GDQNode *node = NULL, *next = NULL, *prev = NULL;
    node = gthis->head.next;// first node
    if (node == &(gthis->head))
        return;
    prev = node->prev;
    next = node->next;
    g_node_free(node);
    prev->next = next;
    next->prev = prev;
    thiz->size = thiz->size - 1;
}

static    void    g_queue_swap(GQueue *thiz, GQueue *that) {
    GDQNode            *next, *nxt;
    GDQNode            *prev, *prv;
    GDQueue *gthiz = (GDQueue*) thiz, *gthat = (GDQueue*)that;
    guint              size = 0;
    if (that == NULL) {
        return;
    }

    next = gthiz->head.next;
    prev = gthiz->head.prev;

    nxt  = gthat->head.next;
    prv  = gthat->head.prev;

    size = gthiz->size;
    gthiz->size = gthat->size;
    gthat->size = size;

    if (nxt != &(gthat->head)) {
        gthiz->head.next = nxt;
        gthiz->head.prev = prv;
        nxt->prev = &(gthiz->head);
        prv->next = &(gthiz->head);
    } else {
        gthiz->head.next = &(gthiz->head);
        gthiz->head.prev = &(gthiz->head);
    }

    if (next != &(gthiz->head)) {
        gthat->head.next = next;
        gthat->head.prev = prev;
        next->prev = &(gthat->head);
        prev->next = &(gthat->head);
    } else {
        gthat->head.next = &(gthat->head);
        gthat->head.prev = &(gthat->head);
    }
}


GQueue* g_queue_alloc() {
    GDQueue *gthis = (GDQueue *) malloc(sizeof(GDQueue));
    GQueue  *thiz = NULL;
    if (gthis == NULL) {
        return NULL;
    }

    g_node_init(&gthis->head, NULL, 0);
    gthis->head.next = &(gthis->head);
    gthis->head.prev = &(gthis->head);
    gthis->size      = 0;

    thiz = &gthis->thiz;
    /////////////////////////////////////
    thiz->clear        = g_queue_clear;
    thiz->free         = g_queue_free;

    thiz->size         = g_queue_size;
    thiz->empty        = g_queue_empty;
    thiz->front        = g_queue_front;
    thiz->back         = g_queue_back;
    thiz->push         = g_queue_push;
    thiz->pop          = g_queue_pop;
    thiz->swap         = g_queue_swap;
    return thiz;
}
