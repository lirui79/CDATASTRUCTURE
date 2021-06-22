#include "gqueue.h"
#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>


typedef struct _GDQNode   GDQNode;

struct _GDQNode {
    GDQNode           *next;
    GDQNode           *prev;
    gpointer           data;// data pointer
    guint              size;//  data size
};


static GDQNode*          g_node_init(GDQNode *_this, gpointer data, guint size) {
    _this->next  = NULL;
    _this->prev  = NULL;
    _this->data  = data;
    _this->size  = size;
    return _this;
}

static GDQNode*          g_node_alloc(gpointer data, guint size) {
    GDQNode *_this = malloc(sizeof(GDQNode));
    if(data == NULL || size <= 0) {
        return g_node_init(_this, NULL, 0);
    }
    gpointer newdata = malloc(size);
    memcpy(newdata, data, size);
    return g_node_init(_this, newdata, size);
}

static  gpointer        g_node_data(GDQNode  *_this) {
    return  _this->data;
}

static void           g_node_free(GDQNode  *_this) {
    if (_this->data != NULL)
        free(_this->data);
    free(_this);
}

static GDQNode*          g_node_insert(GDQNode  *_this, GDQNode  *newthis) {
    GDQNode *next  = _this->next;
    _this->next    = newthis;
    next->prev     = newthis;
    newthis->next  = next;
    newthis->prev  = _this;
    return _this;
}


///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
typedef struct _GDQueue  GDQueue;

struct _GDQueue {
    GQueue                _this;
    GDQNode                head;// list  head  no data      next -> first node
    guint                  size;
};


static    void g_queue_free(GQueue *_this) { // free _this
    GDQueue* gthis = (GDQueue*) _this;
    GDQNode *node = NULL, *next = NULL;
    node = gthis->head.next;
    while (node != &(gthis->head)) {
        next = node->next;
        g_node_free(node);
        node = next;
    }

    gthis->head.next = &(gthis->head);
    gthis->head.prev = &(gthis->head);
    gthis->head.data = NULL;
    gthis->head.size = 0;
    gthis->size      = 0;
}

static    void g_queue_clear(GQueue *_this) {
    GDQueue* gthis = (GDQueue*) _this;
    g_queue_free(_this);
    free(gthis);
}

static    guint  g_queue_size(GQueue *_this) {
    GDQueue* gthis = (GDQueue*) _this;
    return gthis->size;
}

static    guint  g_queue_empty(GQueue *_this) {
    GDQueue* gthis = (GDQueue*) _this;
    if (gthis->size <= 0)
        return 1;
    return 0;
}

static    gpointer g_queue_front(GQueue *_this) {
    GDQueue* gthis = (GDQueue*) _this;
    GDQNode *node = gthis->head.next;
    if (node == &(gthis->head))
        return NULL;
    return g_node_data(node);
}

static    gpointer g_queue_back(GQueue *_this) {
    GDQueue* gthis = (GDQueue*) _this;
    GDQNode *node = gthis->head.prev;
    if (node == &(gthis->head))
        return NULL;
    return g_node_data(node);
}

static    void    g_queue_push(GQueue *_this, gpointer data, guint size) {
    GDQueue* gthis = (GDQueue*) _this;
    GDQNode *node  = g_node_alloc(data, size);
    GDQNode *prev  = gthis->head.prev;
    g_node_insert(prev, node);
    gthis->size    =  gthis->size + 1;
}

static    void    g_queue_pop(GQueue *_this) {
    GDQueue* gthis = (GDQueue*) _this;
    GDQNode *node = NULL, *next = NULL, *prev = NULL;
    node = gthis->head.next;// first node
    if (node == &(gthis->head))
        return;
    prev = node->prev;
    next = node->next;
    g_node_free(node);
    prev->next = next;
    next->prev = prev;
    _this->size = _this->size - 1;
}

static    void    g_queue_swap(GQueue *_this, GQueue *_that) {
    GDQNode            *next, *nxt;
    GDQNode            *prev, *prv;
    GDQueue *_gthis = (GDQueue*) _this, *_gthat = (GDQueue*)_that;
    guint              size = 0;
    if (_that == NULL) {
        return;
    }

    next = _gthis->head.next;
    prev = _gthis->head.prev;

    nxt  = _gthat->head.next;
    prv  = _gthat->head.prev;

    size = _gthis->size;
    _gthis->size = _gthat->size;
    _gthat->size = size;

    if (nxt != &(_gthat->head)) {
        _gthis->head.next = nxt;
        _gthis->head.prev = prv;
        nxt->prev = &(_gthis->head);
        prv->next = &(_gthis->head);
    } else {
        _gthis->head.next = &(_gthis->head);
        _gthis->head.prev = &(_gthis->head);
    }

    if (next != &(_gthis->head)) {
        _gthat->head.next = next;
        _gthat->head.prev = prev;
        next->prev = &(_gthat->head);
        prev->next = &(_gthat->head);
    } else {
        _gthat->head.next = &(_gthat->head);
        _gthat->head.prev = &(_gthat->head);
    }
}


GQueue* g_queue_alloc() {
    GDQueue *gthis = (GDQueue *) malloc(sizeof(GDQueue));
    GQueue  *_this = NULL;
    if (gthis == NULL) {
        return NULL;
    }

    g_node_init(&gthis->head, NULL, 0);
    gthis->head.next = &(gthis->head);
    gthis->head.prev = &(gthis->head);
    gthis->size      = 0;

    _this = &gthis->_this;
    /////////////////////////////////////
    _this->clear        = g_queue_clear;
    _this->free         = g_queue_free;

    _this->size         = g_queue_size;
    _this->empty        = g_queue_empty;
    _this->front        = g_queue_front;
    _this->back         = g_queue_back;
    _this->push         = g_queue_push;
    _this->pop          = g_queue_pop;
    _this->swap         = g_queue_swap;
    return _this;
}
