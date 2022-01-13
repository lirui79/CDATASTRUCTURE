#include  "giterator.h"
#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>

GType  g_default_type(gpointer data, guint size) {
     GType val = {0};
     val.data = data;
     val.size = size;
     return val;
}

GRef g_default_ref(gpointer data, guint size) {
     GRef val = {0};
     val.data = data;
     val.size = size;
     return val;
}

static GIterator       g_default_iterator_next(GIterator *thiz) {
    thiz->idata.data += thiz->idata.size;
    return thiz[0];
}

static GIterator       g_default_iterator_prev(GIterator *thiz) {
    thiz->idata.data -= thiz->idata.size;
    return thiz[0];
}

static GIterator       g_default_iterator_forward(GIterator *thiz, guint n) {
    thiz->idata.data += n * thiz->idata.size;
    return thiz[0];
}

static GIterator       g_default_iterator_backward(GIterator *thiz, guint n) {
    thiz->idata.data -= n * thiz->idata.size;
    return thiz[0];
}

static int              g_default_iterator_equal(GIterator *thiz, GIterator *that) {
    if (thiz->idata.data == that->idata.data)
        return 1;
    return 0;
}

static int              g_default_iterator_unequal(GIterator *thiz, GIterator *that) {
    if (thiz->idata.data != that->idata.data)
        return 1;
    return 0;
}

static int              g_default_iterator_less(GIterator *thiz, GIterator *that) {
    if (thiz->idata.data < that->idata.data)
        return 1;
    return 0;
}

static int              g_default_iterator_less_equal(GIterator *thiz, GIterator *that) {
    if (thiz->idata.data <= that->idata.data)
        return 1;
    return 0;
}

static int              g_default_iterator_greater(GIterator *thiz, GIterator *that) {
    if (thiz->idata.data > that->idata.data)
        return 1;
    return 0;
}

static int              g_default_iterator_greater_equal(GIterator *thiz, GIterator *that) {
    if (thiz->idata.data >= that->idata.data)
        return 1;
    return 0;
}

static GRef         g_default_iterator_get(GIterator *thiz) {
    return thiz->idata;
}

static GIterator       g_default_iterator_set(GIterator *thiz, GRef val) {
    thiz->idata = val;
    return thiz[0];
}

static gpointer        g_default_iterator_data(GIterator *thiz) {
    return thiz->idata.data;
}

GIterator g_default_iterator(gpointer data, guint size, int dir) {
    GIterator        thiz  = {0};
    thiz.idata.data  = data;
    thiz.idata.size  = size;
    thiz.dir         = dir;
    if (dir > 0) {
        thiz.next  = g_default_iterator_next;
        thiz.prev  = g_default_iterator_prev;
        thiz.forward  = g_default_iterator_forward;
        thiz.backward  = g_default_iterator_backward;
    } else {
        thiz.next  = g_default_iterator_prev;
        thiz.prev  = g_default_iterator_next;
        thiz.forward  = g_default_iterator_backward;
        thiz.backward  = g_default_iterator_forward;
    }
    thiz.equal  = g_default_iterator_equal;
    thiz.unequal  = g_default_iterator_unequal;
    thiz.less  = g_default_iterator_less;
    thiz.lequal  = g_default_iterator_less_equal;
    thiz.greater  = g_default_iterator_greater;
    thiz.gequal  = g_default_iterator_greater_equal;
    thiz.get  = g_default_iterator_get;
    thiz.set  = g_default_iterator_set;
    thiz.data = g_default_iterator_data;
    return thiz;
}
