#ifndef GARRAY_H_INCLUDED
#define GARRAY_H_INCLUDED


#include "gtypes.h"


G_BEGIN_DECLS


typedef struct _GArray     GArray;

struct _GArray {
    void (*free)(GArray *_this);  // free _this

    gpointer (*back)(GArray *_this);

    gpointer (*front)(GArray *_this);

    gpointer (*begin)(GArray *_this);

    gpointer (*end)(GArray *_this);

    gpointer  (*move)(GArray *_this, gpointer position, int n);

    gpointer (*rbegin)(GArray *_this);

    gpointer (*rend)(GArray *_this);

    gpointer  (*forward)(GArray *_this, gpointer position, int n);

    GArray* (*reverse)(GArray *_this);

    gpointer  (*at)(GArray *_this, int index);

    GArray* (*fill)(GArray *_this, gpointer data);

    int  (*size)(GArray *_this);

    gpointer  (*data)(GArray *_this);

    GArray* (*assign)(GArray *_this, gpointer first, gpointer last);

    int (*swap)(GArray *_this, GArray *_that);
};


GArray* g_array_alloc(int n, int c); //n - count   c - ElementSize



G_END_DECLS


#endif // GARRAY_H_INCLUDED
