#include "gqueue.h"
#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>


typedef struct _GDQNode   GDQNode;

struct _GDQNode {
    GDQNode           *next;
    GDQNode           *prev;
    gpointer           data;// data pointer //  data size
};


static GDQNode*          g_node_init(GDQNode *thiz, gpointer data) {
    thiz->next  = NULL;
    thiz->prev  = NULL;
    thiz->data  = data;
    return thiz;
}


static GDQNode*          g_node_alloc(gconstpointer val, guint typesize) {
    GDQNode *thiz = malloc(sizeof(GDQNode));
    if(val == NULL || typesize <= 0) {
        return g_node_init(thiz, NULL);
    }
    gpointer newdata = malloc(typesize);
    memcpy(newdata, val, typesize);
    return g_node_init(thiz, newdata);
}

static gpointer      g_node_data(GDQNode  *thiz) {
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
    guint                  typesize;
};

static    void g_queue_clear(GQueue *thiz) {
    GDQueue* gthis = (GDQueue*) thiz;
    GDQNode *node = NULL, *next = NULL;
    node = gthis->head.next;
    while (node != &(gthis->head)) {
        next = node->next;
        g_node_free(node);
        node = next;
    }

    gthis->head.next = &(gthis->head);
    gthis->head.prev = &(gthis->head);
    gthis->head.data  = NULL;
    gthis->size      = 0;
}


static    void g_queue_free(GQueue *thiz) { // free thiz
    GDQueue* gthis = (GDQueue*) thiz;
    g_queue_clear(thiz);
    free(gthis);
}

static    guint  g_queue_typesize(GQueue *thiz) {
    GDQueue* gthis = (GDQueue*) thiz;
    return gthis->typesize;
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

static    gpointer g_queue_front(GQueue *thiz) {
    GDQueue* gthis = (GDQueue*) thiz;
    GDQNode *node = gthis->head.next;
    return node->data;
}

static    gpointer g_queue_back(GQueue *thiz) {
    GDQueue* gthis = (GDQueue*) thiz;
    GDQNode *node = gthis->head.prev;
    return node->data;
}

static    void    g_queue_push(GQueue *thiz, gconstpointer val) {
    GDQueue* gthis = (GDQueue*) thiz;
    GDQNode *node  = g_node_alloc(val, gthis->typesize);
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
    gthis->size = gthis->size - 1;
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


GQueue* g_queue_alloc(guint typesize) {
    GDQueue *gthis = (GDQueue *) malloc(sizeof(GDQueue));
    GQueue  *thiz = NULL;
    if (gthis == NULL) {
        return NULL;
    }

    g_node_init(&gthis->head, NULL);
    gthis->head.next = &(gthis->head);
    gthis->head.prev = &(gthis->head);
    gthis->size      = 0;
    gthis->typesize      = typesize;

    thiz = &gthis->thiz;
    /////////////////////////////////////
    thiz->clear        = g_queue_clear;
    thiz->free         = g_queue_free;
    thiz->typesize     = g_queue_typesize;

    thiz->size         = g_queue_size;
    thiz->empty        = g_queue_empty;
    thiz->front        = g_queue_front;
    thiz->back         = g_queue_back;
    thiz->push         = g_queue_push;
    thiz->pop          = g_queue_pop;
    thiz->swap         = g_queue_swap;
    return thiz;
}
