#include "gstack.h"
#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>

typedef struct _GDStack  GDStack;

struct _GDStack {
    GStack     _this;
    gpointer   first;
    gpointer   last;
    gpointer   end;
    guint      csize;
};

static    void g_stack_free(GStack *_this) {
    GDStack *_gthis = (GDStack*) _this;
    free(_gthis->first);
    _gthis->first = NULL;
    _gthis->last  = NULL;
    _gthis->end  = NULL;
    free(_gthis);
}
static    gpointer g_stack_top(GStack *_this) {
    GDStack *_gthis = (GDStack*) _this;
    gpointer gptr = NULL;
    if (_gthis->last > _gthis->first)
        gptr = _gthis->last - _gthis->csize;
    return gptr;
}

static    guint  g_stack_size(GStack *_this) {
    GDStack *_gthis = (GDStack*) _this;
    guint size = _gthis->last - _gthis->first;
    size = size / _gthis->csize;
    return size;
}

static    guint  g_stack_empty(GStack *_this) {
    GDStack *_gthis = (GDStack*) _this;
    if (_gthis->first == _gthis->last)
        return 1;
    return 0;
}

static    GStack* g_stack_push(GStack *_this, gpointer data) {
    GDStack *_gthis = (GDStack*) _this;
    if (_gthis->end > _gthis->last) {
        memcpy(_gthis->last, data, _gthis->csize);
        _gthis->last = _gthis->last + _gthis->csize;
    }
     return _this;
}
static    GStack* g_stack_pop(GStack *_this) {
    GDStack *_gthis = (GDStack*) _this;
    if (_gthis->last > _gthis->first)
        _gthis->last = _gthis->last - _gthis->csize;
     return _this;
}

static    void g_stack_swap(GStack *_this, GStack *_that) {
    gpointer gptr = NULL;
    GDStack *_gthis = (GDStack*) _this, *_gthat = (GDStack*)_that;
    if (_that == NULL) {
        return;
    }
    gptr = _gthis->first;
    _gthis->first = _gthat->first;
    _gthat->first = gptr;

    gptr = _gthis->last;
    _gthis->last = _gthat->last;
    _gthat->last = gptr;

    gptr = _gthis->end;
    _gthis->end = _gthat->end;
    _gthat->end = gptr;

    guint c = _gthis->csize;
    _gthis->csize = _gthat->csize;
    _gthat->csize = c;
    return;
}

GStack* g_stack_alloc(guint n, guint c) {
 //n - count   c - ElementSize
    GDStack *_gthis = malloc(sizeof(GDStack));
    GStack *_this = NULL;
     if (n <= 0 || c <= 0) {
        return _this;
     }
    _gthis->first = malloc(n * c);
    _gthis->last  = _gthis->first;
    _gthis->end   = _gthis->first + n * c;
    _gthis->csize = c;// unit size > 0

    _this        = &(_gthis->_this);
    _this->free  = g_stack_free;  // free _this
    _this->top   = g_stack_top;
    _this->size  = g_stack_size;
    _this->empty = g_stack_empty;
    _this->push  = g_stack_push;
    _this->pop   = g_stack_pop;
    _this->swap  = g_stack_swap;
     return _this;
}
