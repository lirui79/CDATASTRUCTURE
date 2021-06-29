
#include "garray.h"
#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>





static GIterator       g_darray_iterator_next(GIterator *thiz) {
    thiz->itdata += thiz->itsize;
    return thiz[0];
}

static GIterator       g_darray_iterator_prev(GIterator *thiz) {
    thiz->itdata -= thiz->itsize;
    return thiz[0];
}

static GIterator       g_darray_iterator_forward(GIterator *thiz, guint n) {
    thiz->itdata += n * thiz->itsize;
    return thiz[0];
}

static GIterator       g_darray_iterator_backward(GIterator *thiz, guint n) {
    thiz->itdata -= n * thiz->itsize;
    return thiz[0];
}

static int              g_darray_iterator_equal(GIterator *thiz, GIterator *that) {
    if (thiz->itdata == that->itdata)
        return 1;
    return 0;
}

static int              g_darray_iterator_not_equal(GIterator *thiz, GIterator *that) {
    if (thiz->itdata != that->itdata)
        return 1;
    return 0;
}

static int              g_darray_iterator_less(GIterator *thiz, GIterator *that) {
    if (thiz->itdata < that->itdata)
        return 1;
    return 0;
}

static int              g_darray_iterator_less_equal(GIterator *thiz, GIterator *that) {
    if (thiz->itdata <= that->itdata)
        return 1;
    return 0;
}

static int              g_darray_iterator_greater(GIterator *thiz, GIterator *that) {
    if (thiz->itdata > that->itdata)
        return 1;
    return 0;
}

static int              g_darray_iterator_greater_equal(GIterator *thiz, GIterator *that) {
    if (thiz->itdata >= that->itdata)
        return 1;
    return 0;
}

static gpointer         g_darray_iterator_data(GIterator *thiz) {
    return thiz->itdata;
}

static guint            g_darray_iterator_size(GIterator *thiz) {
    return thiz->itsize;
}

static GIterator       g_darray_iterator_set(GIterator *thiz, gpointer data, guint sz) {
    thiz->itdata = data;
    thiz->itsize = sz;
    return thiz[0];
}

static GIterator g_darray_iterator_init(gpointer data, guint size, guint dir) {
    GIterator        thiz  = {0};
    thiz.itdata  = data;
    thiz.itsize  = size;
    thiz.dir   = dir;
    if (dir > 0) {
        thiz.next  = g_darray_iterator_next;
        thiz.prev  = g_darray_iterator_prev;
        thiz.forward  = g_darray_iterator_forward;
        thiz.backward  = g_darray_iterator_backward;
    } else {
        thiz.next  = g_darray_iterator_prev;
        thiz.prev  = g_darray_iterator_next;
        thiz.forward  = g_darray_iterator_backward;
        thiz.backward  = g_darray_iterator_forward;
    }
    thiz.equal  = g_darray_iterator_equal;
    thiz.not_equal  = g_darray_iterator_not_equal;
    thiz.less  = g_darray_iterator_less;
    thiz.less_equal  = g_darray_iterator_less_equal;
    thiz.greater  = g_darray_iterator_greater;
    thiz.greater_equal  = g_darray_iterator_greater_equal;
    thiz.data  = g_darray_iterator_data;
    thiz.size  = g_darray_iterator_size;
    thiz.set  = g_darray_iterator_set;
    return thiz;
}

GIterator g_array_iterator() {
    return g_darray_iterator_init(NULL, 0, 1);
}



typedef struct _GDArray  GDArray;

struct _GDArray {
    GArray     thiz;
    gpointer   first;
    gpointer   last;
    gint       csize;
};


static void g_array_free(GArray *thiz) {  // free thiz
    GDArray *gthiz = (GDArray*)thiz;
    free(gthiz->first);
    gthiz->first = NULL;
    gthiz->last  = NULL;
    free(gthiz);
}

static guint  g_array_size(GArray *thiz) {
    GDArray *gthiz = (GDArray*) thiz;
    guint size = gthiz->last - gthiz->first;
    size = size / gthiz->csize;
    return size;
}


static gpointer g_array_back(GArray *thiz) {
    GDArray *gthiz = (GDArray*)thiz;
    gpointer gptr = gthiz->last - gthiz->csize;
    return gptr;
}

static gpointer g_array_front(GArray *thiz) {
    GDArray *gthiz = (GDArray*)thiz;
    return gthiz->first;
}

static gpointer g_array_at(GArray *thiz, guint index) {
    GDArray *gthiz = (GDArray*)thiz;
    if (index >= thiz->size(thiz))
        return gthiz->last;
    return (gthiz->first + index * gthiz->csize);
}

static gpointer g_array_data(GArray *thiz) {
    GDArray *gthiz = (GDArray*) thiz;
    return gthiz->first;
}



static GIterator g_array_begin(GArray *thiz) {
    GDArray *gthiz = (GDArray*)thiz;
    GIterator iterator = g_darray_iterator_init(gthiz->first, gthiz->csize, 1);
    return iterator;
}

static GIterator g_array_end(GArray *thiz) {
    GDArray *gthiz = (GDArray*)thiz;
    GIterator iterator = g_darray_iterator_init(gthiz->last, gthiz->csize, 1);
    return iterator;
}

static GIterator g_array_rbegin(GArray *thiz) {
    GDArray *gthiz = (GDArray*)thiz;
    GIterator iterator = g_darray_iterator_init(gthiz->last - gthiz->csize, gthiz->csize, 0);
    return iterator;
}

static GIterator g_array_rend(GArray *thiz) {
    GDArray *gthiz = (GDArray*)thiz;
    GIterator iterator = g_darray_iterator_init(gthiz->first - gthiz->csize, gthiz->csize, 0);
    return iterator;
}



static void g_array_reverse(GArray *thiz) {
    GDArray *gthiz = (GDArray*)thiz;
    gpointer first = gthiz->first, last = gthiz->last - gthiz->csize;
    gpointer gptr  = malloc(gthiz->csize);
    for (; first < last; first = first + gthiz->csize, last = last - gthiz->csize) {
        memcpy(gptr,  first, gthiz->csize);
        memcpy(first, last,  gthiz->csize);
        memcpy(last,  gptr,  gthiz->csize);
    }
    free(gptr);
}

static void g_array_fill(GArray *thiz, gpointer data) {
    GDArray *gthiz = (GDArray*) thiz;
    gpointer gptr = NULL;
    for(gptr = gthiz->first; gptr < gthiz->last; gptr += gthiz->csize) {
        memcpy(gptr, data, gthiz->csize);
    }
}

static void g_array_assign(GArray *thiz, GIterator itfirst, GIterator itlast) {
    GDArray *gthiz = (GDArray*) thiz;
    gpointer first = itfirst.data(&itfirst), last = itlast.data(&itlast);
    if (first == NULL || last == NULL || first >= last) {
        return;
    }

    guint size = gthiz->last - gthiz->first;
    guint newsize = last - first;
    if (newsize > size)
        newsize = size;
    memcpy(gthiz->first, first, newsize);
}

static void  g_array_swap(GArray *thiz, GArray *that) {
    gpointer gptr = NULL;
    GDArray *gthiz = (GDArray*) thiz, *gthat = (GDArray*)that;
    if (that == NULL) {
        return;
    }
    gptr = gthiz->first;
    gthiz->first = gthat->first;
    gthat->first = gptr;

    gptr = gthiz->last;
    gthiz->last = gthat->last;
    gthat->last = gptr;

    guint c = gthiz->csize;
    gthiz->csize = gthat->csize;
    gthat->csize = c;
}


GArray* g_array_alloc(guint n, guint c) { //n - count   c - ElementSize
    GDArray *gthiz = NULL;
    GArray  *thiz  = NULL;
    if (n <= 0 || c <= 0) {
        return thiz;
    }
    gthiz = malloc(sizeof(GDArray));
    gthiz->first = malloc(n * c);
    gthiz->last  = gthiz->first + n * c;
    gthiz->csize = c;// unit size > 0

    thiz     = &(gthiz->thiz);
    thiz->free  = g_array_free;  // free thiz
    thiz->back  = g_array_back;
    thiz->front  = g_array_front;
    thiz->begin  = g_array_begin;
    thiz->end    = g_array_end;
    thiz->rbegin = g_array_rbegin;
    thiz->rend   = g_array_rend;
    thiz->reverse  = g_array_reverse;
    thiz->at  = g_array_at;
    thiz->fill  = g_array_fill;
    thiz->size  = g_array_size;
    thiz->data  = g_array_data;
    thiz->assign  = g_array_assign;
    thiz->swap  = g_array_swap;
    return thiz;
}
