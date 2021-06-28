#ifndef GVECTOR_H_INCLUDED
#define GVECTOR_H_INCLUDED


#include "gtypes.h"
#include "giterator.h"


G_BEGIN_DECLS


typedef struct  _GVector     GVector;

struct _GVector {
    void      (*clear)(GVector *_this);// but not free _this

    void      (*free)(GVector *_this);  // free _this

    guint     (*empty)(GVector *_this);

    guint     (*size)(GVector *_this);

    guint     (*capacity)(GVector *_this);

    gpointer  (*back)(GVector *_this);

    gpointer  (*front)(GVector *_this);

    GIterator  (*begin)(GVector *_this);

    GIterator  (*end)(GVector *_this);

    gpointer  (*backward)(GVector *_this, gpointer position, gint n);

    GIterator  (*rbegin)(GVector *_this);

    GIterator  (*rend)(GVector *_this);

    gpointer  (*forward)(GVector *_this, gpointer position, gint n);

    gpointer  (*at)(GVector *_this, guint index);

    gpointer  (*data)(GVector *_this);

    void      (*push_back)(GVector *_this, gpointer data);

    void      (*push_front)(GVector *_this, gpointer data);

    void      (*pop_back)(GVector *_this);//

    void      (*pop_front)(GVector *_this);//

    GIterator  (*erase)(GVector *_this, GIterator first, GIterator last);

    GIterator  (*remove)(GVector *_this, GIterator position);//

    void      (*resize)(GVector *_this, guint n, gpointer data);

    void      (*reserve)(GVector *_this, guint capacity);

    void      (*assign)(GVector *_this, GIterator first, GIterator last);

    void      (*fill)(GVector *_this, GIterator position, guint n, gpointer data);

    void      (*insert)(GVector *_this, GIterator position, GIterator first, GIterator last);

    void      (*reverse)(GVector *_this);

    void      (*swap)(GVector *_this, GVector *_that);
};


GVector* g_vector_alloc(guint n, guint c); //vector ElementSize


G_END_DECLS


#endif // GVECTOR_H_INCLUDED
