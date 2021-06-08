#ifndef GVECTOR_H_INCLUDED
#define GVECTOR_H_INCLUDED


#include "gtypes.h"


G_BEGIN_DECLS


typedef struct  _GVector     GVector;

struct _GVector {
    void (*clear)(GVector *_this);// but not free _this

    void (*free)(GVector *_this);  // free _this

    GVector* (*push_back)(GVector *_this, gpointer data);

    GVector* (*push_front)(GVector *_this, gpointer data);

    GVector* (*remove)(GVector *_this, gpointer data);//

    GVector* (*pop_back)(GVector *_this);//

    GVector* (*pop_front)(GVector *_this);//

    gpointer (*back)(GVector *_this);

    gpointer (*front)(GVector *_this);

    gpointer (*begin)(GVector *_this);

    gpointer (*end)(GVector *_this);

    gpointer  (*move)(GVector *_this, gpointer position, int n);

    gpointer (*rbegin)(GVector *_this);

    gpointer (*rend)(GVector *_this);

    gpointer  (*forward)(GVector *_this, gpointer position, int n);

    GVector* (*reverse)(GVector *_this);

    gpointer  (*at)(GVector *_this, int index);

    GVector* (*fill)(GVector *_this, gpointer position, int n, gpointer data);

    int  (*empty)(GVector *_this);

    int  (*size)(GVector *_this);

    GVector* (*erase)(GVector *_this, gpointer first, gpointer last);

    gpointer  (*data)(GVector *_this);

    GVector* (*resize)(GVector *_this, int n, gpointer data);

    GVector* (*reserve)(GVector *_this, int capacity);

    GVector* (*assign)(GVector *_this, gpointer first, gpointer last);

    GVector* (*insert)(GVector *_this, gpointer position, gpointer first, gpointer last);

    int (*capacity)(GVector *_this);
};


GVector* g_vector_alloc(int cellsize); //vector ElementSize


G_END_DECLS


#endif // GVECTOR_H_INCLUDED
