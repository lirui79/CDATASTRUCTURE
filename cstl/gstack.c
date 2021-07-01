#include "gstack.h"
#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>

typedef struct _GDStack  GDStack;

struct _GDStack {
    GStack     thiz;
    gpointer   first;
    gpointer   last;
    gpointer   end;
    guint      size;
};

static    void g_stack_free(GStack *thiz) {
    GDStack *gthiz = (GDStack*) thiz;
    free(gthiz->first);
    gthiz->first = NULL;
    gthiz->last  = NULL;
    gthiz->end  = NULL;
    free(gthiz);
}

static    void g_stack_clear(GStack *thiz) {
    GDStack *gthiz = (GDStack*) thiz;
    gthiz->last  = gthiz->first;
}

static    GReference g_stack_top(GStack *thiz) {
    GDStack *gthiz = (GDStack*) thiz;
    GReference ref = {gthiz->first, gthiz->size};
    if (gthiz->last > gthiz->first)
        ref.data = gthiz->last - gthiz->size;
    return ref;
}

static    guint  g_stack_size(GStack *thiz) {
    GDStack *gthiz = (GDStack*) thiz;
    guint size = gthiz->last - gthiz->first;
    size = size / gthiz->size;
    return size;
}

static    guint  g_stack_empty(GStack *thiz) {
    GDStack *gthiz = (GDStack*) thiz;
    if (gthiz->first == gthiz->last)
        return 1;
    return 0;
}

static    void g_stack_push(GStack *thiz, GReference val) {
    GDStack *gthiz = (GDStack*) thiz;
    if (gthiz->end > gthiz->last) {
        memcpy(gthiz->last, val.data, gthiz->size);
        gthiz->last = gthiz->last + gthiz->size;
    }
}
static    void g_stack_pop(GStack *thiz) {
    GDStack *gthiz = (GDStack*) thiz;
    if (gthiz->last > gthiz->first)
        gthiz->last = gthiz->last - gthiz->size;
}

static    void g_stack_swap(GStack *thiz, GStack *that) {
    gpointer gptr = NULL;
    GDStack *gthiz = (GDStack*) thiz, *gthat = (GDStack*)that;
    if (that == NULL) {
        return;
    }
    gptr = gthiz->first;
    gthiz->first = gthat->first;
    gthat->first = gptr;

    gptr = gthiz->last;
    gthiz->last = gthat->last;
    gthat->last = gptr;

    gptr = gthiz->end;
    gthiz->end = gthat->end;
    gthat->end = gptr;

    guint c = gthiz->size;
    gthiz->size = gthat->size;
    gthat->size = c;
    return;
}

GStack* g_stack_alloc(guint n, guint c) { //n - count   c - ElementSize
    GDStack *gthiz = NULL;
    GStack  *thiz  = NULL;
    if (n <= 0 || c <= 0) {
        return thiz;
    }

    gthiz = malloc(sizeof(GDStack));
    gthiz->first = malloc(n * c);
    gthiz->last  = gthiz->first;
    gthiz->end   = gthiz->first + n * c;
    gthiz->size = c;// unit size > 0

    thiz        = &(gthiz->thiz);
    thiz->free  = g_stack_free;  // free thiz
    thiz->clear = g_stack_clear;
    thiz->top   = g_stack_top;
    thiz->size  = g_stack_size;
    thiz->empty = g_stack_empty;
    thiz->push  = g_stack_push;
    thiz->pop   = g_stack_pop;
    thiz->swap  = g_stack_swap;
    return thiz;
}
