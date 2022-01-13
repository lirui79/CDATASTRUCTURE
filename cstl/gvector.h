#ifndef GVECTOR_H_INCLUDED
#define GVECTOR_H_INCLUDED


#include "gtypes.h"
#include "giterator.h"


G_BEGIN_DECLS


typedef struct  _GVector     GVector;

struct _GVector {
    void      (*clear)(GVector *thiz);// but not free thiz

    void      (*free)(GVector *thiz);  // free thiz


    guint     (*typesize)(GVector *thiz);

    guint     (*empty)(GVector *thiz);

    guint     (*size)(GVector *thiz);

    guint     (*capacity)(GVector *thiz);

    void      (*resize)(GVector *thiz, guint n, gconstpointer val);

    void      (*reserve)(GVector *thiz, guint capacity);



    gpointer  (*back)(GVector *thiz);

    gpointer  (*front)(GVector *thiz);

    gpointer  (*at)(GVector *thiz, guint index);

    gpointer  (*data)(GVector *thiz);



    GIterator  (*begin)(GVector *thiz);

    GIterator  (*end)(GVector *thiz);

    GIterator  (*rbegin)(GVector *thiz);

    GIterator  (*rend)(GVector *thiz);



    void      (*push_back)(GVector *thiz, gconstpointer val);

    void      (*push_front)(GVector *thiz, gconstpointer val);

    void      (*pop_back)(GVector *thiz);//

    void      (*pop_front)(GVector *thiz);//

    GIterator  (*erase)(GVector *thiz, GIterator first, GIterator last);

    GIterator  (*remove)(GVector *thiz, GIterator position);//


    void      (*assign)(GVector *thiz, GIterator first, GIterator last);

    void      (*fill)(GVector *thiz, GIterator position, guint n, gconstpointer val);

    void      (*insert)(GVector *thiz, GIterator position, GIterator first, GIterator last);

    void      (*reverse)(GVector *thiz);

    void      (*swap)(GVector *thiz, GVector *that);
};


GVector* g_vector_alloc(guint cnt, guint typesize); //vector ElementSize



G_END_DECLS


#endif // GVECTOR_H_INCLUDED
