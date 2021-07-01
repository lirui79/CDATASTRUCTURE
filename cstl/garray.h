#ifndef GARRAY_H_INCLUDED
#define GARRAY_H_INCLUDED


#include "gtypes.h"
#include "giterator.h"


G_BEGIN_DECLS


typedef struct _GArray     GArray;

struct _GArray {
    void     (*free)(GArray *thiz);  // free thiz

    guint    (*size)(GArray *thiz);

    GReference (*back)(GArray *thiz);

    GReference (*front)(GArray *thiz);

    GReference (*at)(GArray *thiz, guint index);

    GReference (*data)(GArray *thiz);


    GIterator (*begin)(GArray *thiz);

    GIterator (*end)(GArray *thiz);

    GIterator (*rbegin)(GArray *thiz);

    GIterator (*rend)(GArray *thiz);


    void     (*reverse)(GArray *thiz);

    void     (*fill)(GArray *thiz, GReference val);

    void     (*assign)(GArray *thiz, GIterator first, GIterator last);

    void     (*swap)(GArray *thiz, GArray *that);
};

GArray* g_array_alloc(guint n, guint c); //n - count   c - ElementSize



G_END_DECLS


#endif // GARRAY_H_INCLUDED
