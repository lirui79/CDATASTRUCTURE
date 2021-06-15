#ifndef GARRAY_H_INCLUDED
#define GARRAY_H_INCLUDED


#include "gtypes.h"


G_BEGIN_DECLS


typedef struct _GArray     GArray;

struct _GArray {
    void     (*free)(GArray *_this);  // free _this

    guint    (*size)(GArray *_this);

    gpointer (*back)(GArray *_this);

    gpointer (*front)(GArray *_this);

    gpointer (*begin)(GArray *_this);

    gpointer (*end)(GArray *_this);

    gpointer (*backward)(GArray *_this, gpointer position, gint n);

    gpointer (*rbegin)(GArray *_this);

    gpointer (*rend)(GArray *_this);

    gpointer (*forward)(GArray *_this, gpointer position, gint n);

    void     (*reverse)(GArray *_this);

    gpointer (*at)(GArray *_this, guint index);

    void     (*fill)(GArray *_this, gpointer data);

    gpointer (*data)(GArray *_this);

    void     (*assign)(GArray *_this, gpointer first, gpointer last);

    void     (*swap)(GArray *_this, GArray *_that);
};


GArray* g_array_alloc(guint n, guint c); //n - count   c - ElementSize



G_END_DECLS


#endif // GARRAY_H_INCLUDED
