
#include "gdeque.h"
#include "gvector.h"
#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>

typedef struct _GDDeque  GDDeque;

/*************************************/
/*
map   |  |  |
*/


struct _GDDeque {
    GDeque     _this;
    gpointer  *map;
    guint      rows;//
    guint      cols;
    guint      csize;
    guint      nsize;
    struct {
        gpointer   first;
        gpointer   last ;
        gpointer   cur;
        gpointer  *node;
    } start, finish;
};

static    void      g_deque_clear(GDeque *_this) {
    GDDeque *gthis = (GDDeque*) _this;
    gthis->start.node  = gthis->map;
    gthis->start.first = gthis->map[0];
    gthis->start.last  = gthis->map[0] + gthis->cols * gthis->csize;
    gthis->start.cur   = gthis->map[0];
    gthis->finish      = gthis->start;
}

static    void      g_deque_free(GDeque *_this) {
    GDDeque *gthis = (GDDeque*) _this;
    for (guint i = 0; i < gthis->rows; ++i) {
        gpointer gptr = gthis->map[i];
        free(gptr);
    }
    free(gthis->map);
    gthis->rows = 0;
    gthis->map = NULL;
    free(gthis);
}

static    guint     g_deque_size(GDeque *_this) {
    GDDeque *gthis = (GDDeque*) _this;
    gpointer  *node = gthis->start.node;
    guint  size = (gthis->start.last -gthis->start.cur) / gthis->csize;
    for (node += gthis->nsize; node < gthis->finish.node; node += gthis->nsize) {
        size += gthis->cols;
    }

    size += (gthis->finish.cur - gthis->finish.first) / gthis->csize;
    return size;
}

static    guint     g_deque_empty(GDeque *_this) {
    if (g_deque_size(_this) <= 0)
        return 1;
    return 0;
}

static    gpointer  g_deque_begin(GDeque *_this) {
    GDDeque *gthis = (GDDeque*) _this;
    return gthis->start.cur;
}

static    gpointer  g_deque_end(GDeque *_this) {
    GDDeque *gthis = (GDDeque*) _this;
    return gthis->finish.cur;
}

static    gpointer  g_deque_backward(GDeque *_this, gpointer position, gint n) {

}

static    gpointer  g_deque_front(GDeque *_this) {
    GDDeque *gthis = (GDDeque*) _this;
    return gthis->start.cur;
}

static    gpointer  g_deque_back(GDeque *_this) {
    GDDeque *gthis = (GDDeque*) _this;
    if (gthis->finish.cur == gthis->finish.first) {
        if (gthis->start.node == gthis->finish.node) {
            return NULL;
        }
        gpointer *node = gthis->finish.node;
        node -= sizeof(gpointer*);
        gpointer gptr = (*node);
        gptr += (gthis->cols - 1) * gthis->csize;
        return gptr;
    }
    return (gthis->finish.cur - gthis->csize);
}

static    gpointer  g_deque_at(GDeque *_this, guint index) {
}

static    gpointer  g_deque_rbegin(GDeque *_this){

}

static    gpointer  g_deque_rend(GDeque *_this){

}

static    gpointer  g_deque_forward(GDeque *_this, gpointer position, gint n){

}

static    void      g_deque_assign(GDeque *_this, gpointer first, gpointer last){

}

static    void      g_deque_fill(GDeque *_this, gpointer position, guint n, gpointer data){

}

static    void      g_deque_push_back(GDeque *_this, gpointer data){

}

static    void      g_deque_push_front(GDeque *_this, gpointer data){

}

static    void      g_deque_pop_back(GDeque *_this){

}

static    void      g_deque_pop_front(GDeque *_this){

}

static    void      g_deque_insert(GDeque *_this, gpointer position, gpointer first, gpointer last){

}

static    gpointer  g_deque_erase(GDeque *_this, gpointer first, gpointer last){

}

static    gpointer  g_deque_remove(GDeque *_this, gpointer data){

}

static    void      g_deque_swap(GDeque *_this, GDeque *_that){

}

static    void      g_deque_resize(GDeque *_this, guint n){

}


GDeque* g_deque_alloc(guint n, guint c) { //n - count   c - ElementSize
    GDDeque *gthis = NULL;
    GDeque   *_this = NULL;
    if (n <= 0 || c <= 0) {
        return _this;
    }

    gthis = malloc(sizeof(GDDeque));
    gthis->nsize = sizeof(gpointer);
    gthis->map   = malloc(n * gthis->nsize);
    gthis->rows  = n;//
    gthis->cols  = 8;
    for (guint i = 0; i < gthis->rows; ++i) {
        gthis->map[i] = malloc(gthis->cols * c);
    }
    gthis->csize = c;

    gthis->start.node  = gthis->map;
    gthis->start.first = gthis->map[0];
    gthis->start.last  = gthis->map[0] + gthis->cols * c;
    gthis->start.cur   = gthis->map[0];
    gthis->finish      = gthis->start;

    _this         = &(gthis->_this);
    _this->free      = g_deque_free;
    _this->clear     = g_deque_clear;
    _this->size      = g_deque_size;
    _this->empty     = g_deque_empty;
    _this->begin     = g_deque_begin;
    _this->end       = g_deque_end;
    _this->backward  = g_deque_backward;
    _this->front     = g_deque_front;
    _this->back      = g_deque_back;
    _this->at        = g_deque_at;
    _this->rbegin    = g_deque_rbegin;
    _this->rend      = g_deque_rend;
    _this->forward   = g_deque_forward;
    _this->assign    = g_deque_assign;
    _this->fill      = g_deque_fill;
    _this->push_back    = g_deque_push_back;
    _this->push_front   = g_deque_push_front;
    _this->pop_back     = g_deque_pop_back;
    _this->pop_front    = g_deque_pop_front;
    _this->insert       = g_deque_insert;
    _this->erase        = g_deque_erase;
    _this->remove       = g_deque_remove;
    _this->swap         = g_deque_swap;
    _this->resize       = g_deque_resize;
    return _this;
}
