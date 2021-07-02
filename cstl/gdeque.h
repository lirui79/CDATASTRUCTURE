#ifndef GDEQUE_H_INCLUDED
#define GDEQUE_H_INCLUDED



#include "gtypes.h"
#include "giterator.h"


G_BEGIN_DECLS


typedef struct _GDeque     GDeque;

struct _GDeque {
    void      (*free)(GDeque *thiz);

    void      (*clear)(GDeque *thiz);


    guint     (*size)(GDeque *thiz);

    guint     (*empty)(GDeque *thiz);

    GType     (*front)(GDeque *thiz);

    GType     (*back)(GDeque *thiz);

    GType     (*at)(GDeque *thiz, guint index);


    GIterator  (*begin)(GDeque *thiz);

    GIterator  (*end)(GDeque *thiz);

    GIterator  (*rbegin)(GDeque *thiz);

    GIterator  (*rend)(GDeque *thiz);


    void      (*push_back)(GDeque *thiz, GType val);

    void      (*push_front)(GDeque *thiz, GType val);

    void      (*pop_back)(GDeque *thiz);

    void      (*pop_front)(GDeque *thiz);

    GIterator  (*erase)(GDeque *thiz, GIterator first, GIterator last);

    GIterator  (*remove)(GDeque *thiz, GIterator position);


    void      (*assign)(GDeque *thiz, GIterator first, GIterator last);

    void      (*fill)(GDeque *thiz, GIterator position, guint n, GType val);

    void      (*insert)(GDeque *thiz, GIterator position, GIterator first, GIterator last);

    void      (*swap)(GDeque *thiz, GDeque *that);

    void      (*resize)(GDeque *thiz, guint n, GType val);
};

GIterator g_deque_iterator(gpointer data, guint size, int dir);

GDeque*   g_deque_alloc(guint size); //n - count   c - ElementSize


G_END_DECLS


#endif // GDEQUE_H_INCLUDED
