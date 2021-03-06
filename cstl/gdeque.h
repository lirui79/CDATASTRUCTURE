#ifndef GDEQUE_H_INCLUDED
#define GDEQUE_H_INCLUDED



#include "gtypes.h"
#include "giterator.h"


G_BEGIN_DECLS


typedef struct _GDeque     GDeque;

struct _GDeque {
    void      (*free)(GDeque *thiz);

    void      (*clear)(GDeque *thiz);

    guint      (*typesize)(GDeque *thiz);


    guint     (*size)(GDeque *thiz);

    guint     (*empty)(GDeque *thiz);

    gpointer     (*front)(GDeque *thiz);

    gpointer     (*back)(GDeque *thiz);

    gpointer     (*at)(GDeque *thiz, guint index);


    GIterator  (*begin)(GDeque *thiz);

    GIterator  (*end)(GDeque *thiz);

    GIterator  (*rbegin)(GDeque *thiz);

    GIterator  (*rend)(GDeque *thiz);


    void      (*push_back)(GDeque *thiz, gconstpointer val);

    void      (*push_front)(GDeque *thiz, gconstpointer val);

    void      (*pop_back)(GDeque *thiz);

    void      (*pop_front)(GDeque *thiz);

    GIterator  (*erase)(GDeque *thiz, GIterator first, GIterator last);

    GIterator  (*remove)(GDeque *thiz, GIterator position);


    void      (*assign)(GDeque *thiz, GIterator first, GIterator last);

    void      (*fill)(GDeque *thiz, GIterator position, guint n, gconstpointer val);

    void      (*insert)(GDeque *thiz, GIterator position, GIterator first, GIterator last);

    void      (*swap)(GDeque *thiz, GDeque *that);

    void      (*resize)(GDeque *thiz, guint n, gconstpointer val);
};

GIterator g_deque_iterator(gpointer data, guint size, int dir);

GDeque*   g_deque_alloc(guint typesize); //n - count   c - ElementSize


G_END_DECLS


#endif // GDEQUE_H_INCLUDED
