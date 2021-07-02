#ifndef GVECTOR_H_INCLUDED
#define GVECTOR_H_INCLUDED


#include "gtypes.h"
#include "giterator.h"


G_BEGIN_DECLS


typedef struct  _GVector     GVector;

struct _GVector {
    void      (*clear)(GVector *thiz);// but not free thiz

    void      (*free)(GVector *thiz);  // free thiz



    guint     (*empty)(GVector *thiz);

    guint     (*size)(GVector *thiz);

    guint     (*capacity)(GVector *thiz);

    void      (*resize)(GVector *thiz, guint n, GType val);

    void      (*reserve)(GVector *thiz, guint capacity);



    GType     (*back)(GVector *thiz);

    GType     (*front)(GVector *thiz);

    GType     (*at)(GVector *thiz, guint index);

    GType     (*data)(GVector *thiz);



    GIterator  (*begin)(GVector *thiz);

    GIterator  (*end)(GVector *thiz);

    GIterator  (*rbegin)(GVector *thiz);

    GIterator  (*rend)(GVector *thiz);



    void      (*push_back)(GVector *thiz, GType val);

    void      (*push_front)(GVector *thiz, GType val);

    void      (*pop_back)(GVector *thiz);//

    void      (*pop_front)(GVector *thiz);//

    GIterator  (*erase)(GVector *thiz, GIterator first, GIterator last);

    GIterator  (*remove)(GVector *thiz, GIterator position);//


    void      (*assign)(GVector *thiz, GIterator first, GIterator last);

    void      (*fill)(GVector *thiz, GIterator position, guint n, GType val);

    void      (*insert)(GVector *thiz, GIterator position, GIterator first, GIterator last);

    void      (*reverse)(GVector *thiz);

    void      (*swap)(GVector *thiz, GVector *that);
};


GVector* g_vector_alloc(guint cnt, guint size); //vector ElementSize



G_END_DECLS


#endif // GVECTOR_H_INCLUDED
