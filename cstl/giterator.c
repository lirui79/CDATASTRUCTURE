#include  "giterator.h"
#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>



static GIterator       g_default_iterator_next(GIterator *thiz) {
    thiz->refer.data += thiz->refer.size;
    return thiz[0];
}

static GIterator       g_default_iterator_prev(GIterator *thiz) {
    thiz->refer.data -= thiz->refer.size;
    return thiz[0];
}

static GIterator       g_default_iterator_forward(GIterator *thiz, guint n) {
    thiz->refer.data += n * thiz->refer.size;
    return thiz[0];
}

static GIterator       g_default_iterator_backward(GIterator *thiz, guint n) {
    thiz->refer.data -= n * thiz->refer.size;
    return thiz[0];
}

static int              g_default_iterator_equal(GIterator *thiz, GIterator *that) {
    if (thiz->refer.data == that->refer.data)
        return 1;
    return 0;
}

static int              g_default_iterator_not_equal(GIterator *thiz, GIterator *that) {
    if (thiz->refer.data != that->refer.data)
        return 1;
    return 0;
}

static int              g_default_iterator_less(GIterator *thiz, GIterator *that) {
    if (thiz->refer.data < that->refer.data)
        return 1;
    return 0;
}

static int              g_default_iterator_less_equal(GIterator *thiz, GIterator *that) {
    if (thiz->refer.data <= that->refer.data)
        return 1;
    return 0;
}

static int              g_default_iterator_greater(GIterator *thiz, GIterator *that) {
    if (thiz->refer.data > that->refer.data)
        return 1;
    return 0;
}

static int              g_default_iterator_greater_equal(GIterator *thiz, GIterator *that) {
    if (thiz->refer.data >= that->refer.data)
        return 1;
    return 0;
}

static GReference         g_default_iterator_get(GIterator *thiz) {
    return thiz->refer;
}

static GIterator       g_default_iterator_set(GIterator *thiz, GReference val) {
    thiz->refer = val;
    return thiz[0];
}

GIterator g_default_iterator(gpointer data, guint size, int dir) {
    GIterator        thiz  = {0};
    thiz.refer.data  = data;
    thiz.refer.size  = size;
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
    thiz.not_equal  = g_default_iterator_not_equal;
    thiz.less  = g_default_iterator_less;
    thiz.less_equal  = g_default_iterator_less_equal;
    thiz.greater  = g_default_iterator_greater;
    thiz.greater_equal  = g_default_iterator_greater_equal;
    thiz.get  = g_default_iterator_get;
    thiz.set  = g_default_iterator_set;
    thiz.data = g_default_iterator_get;
    return thiz;
}
