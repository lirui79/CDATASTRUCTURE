
#include "garray.h"
#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>


typedef struct _GDArray  GDArray;

struct _GDArray {
    GArray     thiz;
    gpointer   first;
    gpointer   last;
    gint       typesize;
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
    size = size / gthiz->typesize;
    return size;
}

static guint  g_array_typesize(GArray *thiz) {
    GDArray *gthiz = (GDArray*)thiz;
    return gthiz->typesize;
}

static gpointer g_array_back(GArray *thiz) {
    GDArray *gthiz = (GDArray*)thiz;
    gpointer val = gthiz->last - gthiz->typesize;
    return val;
}

static gpointer g_array_front(GArray *thiz) {
    GDArray *gthiz = (GDArray*)thiz;
    return gthiz->first;
}

static gpointer g_array_at(GArray *thiz, guint index) {
    GDArray *gthiz = (GDArray*)thiz;
    gpointer val = gthiz->last;
    if (index >= thiz->size(thiz))
        return val;
    val =(gthiz->first + index * gthiz->typesize);
    return val;
}

static gpointer g_array_data(GArray *thiz) {
    GDArray *gthiz = (GDArray*) thiz;
    return gthiz->first;
}



static GIterator g_array_begin(GArray *thiz) {
    GDArray *gthiz = (GDArray*)thiz;
    GIterator iterator = g_default_iterator(gthiz->first, gthiz->typesize, 1);
    return iterator;
}

static GIterator g_array_end(GArray *thiz) {
    GDArray *gthiz = (GDArray*)thiz;
    GIterator iterator = g_default_iterator(gthiz->last, gthiz->typesize, 1);
    return iterator;
}

static GIterator g_array_rbegin(GArray *thiz) {
    GDArray *gthiz = (GDArray*)thiz;
    GIterator iterator = g_default_iterator(gthiz->last - gthiz->typesize, gthiz->typesize, 0);
    return iterator;
}

static GIterator g_array_rend(GArray *thiz) {
    GDArray *gthiz = (GDArray*)thiz;
    GIterator iterator = g_default_iterator(gthiz->first - gthiz->typesize, gthiz->typesize, 0);
    return iterator;
}



static void g_array_reverse(GArray *thiz) {
    GDArray *gthiz = (GDArray*)thiz;
    gpointer first = gthiz->first, last = gthiz->last - gthiz->typesize;
    gpointer gptr  = malloc(gthiz->typesize);
    for (; first < last; first = first + gthiz->typesize, last = last - gthiz->typesize) {
        memcpy(gptr,  first, gthiz->typesize);
        memcpy(first, last,  gthiz->typesize);
        memcpy(last,  gptr,  gthiz->typesize);
    }
    free(gptr);
}

static void g_array_fill(GArray *thiz, gconstpointer val) {
    GDArray *gthiz = (GDArray*) thiz;
    gpointer gptr = NULL;
    for(gptr = gthiz->first; gptr < gthiz->last; gptr += gthiz->typesize) {
        memcpy(gptr, val, gthiz->typesize);
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

    guint typesize = gthiz->typesize;
    gthiz->typesize = gthat->typesize;
    gthat->typesize = typesize;
}


GArray* g_array_alloc(guint cnt, guint typesize) { //n - count   c - ElementSize
    GDArray *gthiz = NULL;
    GArray  *thiz  = NULL;
    if (cnt <= 0 || typesize <= 0) {
        return thiz;
    }
    gthiz = malloc(sizeof(GDArray));
    gthiz->first = malloc(cnt * typesize);
    gthiz->last  = gthiz->first + cnt * typesize;
    gthiz->typesize = typesize;// unit typesize > 0

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
    thiz->typesize  = g_array_typesize;
    thiz->data  = g_array_data;
    thiz->assign  = g_array_assign;
    thiz->swap  = g_array_swap;
    return thiz;
}
