
#include "garray.h"
#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>


typedef struct _GDArray  GDArray;

struct _GDArray {
    GArray     thiz;
    gpointer   first;
    gpointer   last;
    gint       size;
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
    size = size / gthiz->size;
    return size;
}


static GType g_array_back(GArray *thiz) {
    GDArray *gthiz = (GDArray*)thiz;
    GType val = {gthiz->last - gthiz->size, gthiz->size};
    return val;
}

static GType g_array_front(GArray *thiz) {
    GDArray *gthiz = (GDArray*)thiz;
    GType val = {gthiz->first, gthiz->size};
    return val;
}

static GType g_array_at(GArray *thiz, guint index) {
    GDArray *gthiz = (GDArray*)thiz;
    GType val = {gthiz->last, gthiz->size};
    if (index >= thiz->size(thiz))
        return val;
    val.data =(gthiz->first + index * gthiz->size);
    return val;
}

static GType g_array_data(GArray *thiz) {
    GDArray *gthiz = (GDArray*) thiz;
    GType val = {gthiz->first, gthiz->size};
    return val;
}



static GIterator g_array_begin(GArray *thiz) {
    GDArray *gthiz = (GDArray*)thiz;
    GIterator iterator = g_default_iterator(gthiz->first, gthiz->size, 1);
    return iterator;
}

static GIterator g_array_end(GArray *thiz) {
    GDArray *gthiz = (GDArray*)thiz;
    GIterator iterator = g_default_iterator(gthiz->last, gthiz->size, 1);
    return iterator;
}

static GIterator g_array_rbegin(GArray *thiz) {
    GDArray *gthiz = (GDArray*)thiz;
    GIterator iterator = g_default_iterator(gthiz->last - gthiz->size, gthiz->size, 0);
    return iterator;
}

static GIterator g_array_rend(GArray *thiz) {
    GDArray *gthiz = (GDArray*)thiz;
    GIterator iterator = g_default_iterator(gthiz->first - gthiz->size, gthiz->size, 0);
    return iterator;
}



static void g_array_reverse(GArray *thiz) {
    GDArray *gthiz = (GDArray*)thiz;
    gpointer first = gthiz->first, last = gthiz->last - gthiz->size;
    gpointer gptr  = malloc(gthiz->size);
    for (; first < last; first = first + gthiz->size, last = last - gthiz->size) {
        memcpy(gptr,  first, gthiz->size);
        memcpy(first, last,  gthiz->size);
        memcpy(last,  gptr,  gthiz->size);
    }
    free(gptr);
}

static void g_array_fill(GArray *thiz, GType val) {
    GDArray *gthiz = (GDArray*) thiz;
    gpointer gptr = NULL;
    for(gptr = gthiz->first; gptr < gthiz->last; gptr += gthiz->size) {
        memcpy(gptr, val.data, val.size);
    }
}

static void g_array_assign(GArray *thiz, GIterator itfirst, GIterator itlast) {
    GDArray *gthiz = (GDArray*) thiz;
    gpointer first = itfirst.get(&itfirst).data, last = itlast.get(&itlast).data;
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

    guint c = gthiz->size;
    gthiz->size = gthat->size;
    gthat->size = c;
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
    gthiz->size = c;// unit size > 0

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
