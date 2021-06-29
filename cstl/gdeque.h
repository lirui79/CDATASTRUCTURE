#ifndef GDEQUE_H_INCLUDED
#define GDEQUE_H_INCLUDED



#include "gtypes.h"


G_BEGIN_DECLS


typedef struct _GDeque     GDeque;

struct _GDeque {
    void      (*free)(GDeque *thiz);

    void      (*clear)(GDeque *thiz);

    guint     (*size)(GDeque *thiz);

    guint     (*empty)(GDeque *thiz);

    gpointer  (*begin)(GDeque *thiz);

    gpointer  (*end)(GDeque *thiz);

    gpointer  (*backward)(GDeque *thiz, gpointer position, gint n);

    gpointer  (*front)(GDeque *thiz);

    gpointer  (*back)(GDeque *thiz);

    gpointer  (*at)(GDeque *thiz, guint index);

    gpointer  (*rbegin)(GDeque *thiz);

    gpointer  (*rend)(GDeque *thiz);

    gpointer  (*forward)(GDeque *thiz, gpointer position, gint n);

    void      (*assign)(GDeque *thiz, gpointer first, gpointer last);

    void      (*fill)(GDeque *thiz, gpointer position, guint n, gpointer data);

    void      (*push_back)(GDeque *thiz, gpointer data);

    void      (*push_front)(GDeque *thiz, gpointer data);

    void      (*pop_back)(GDeque *thiz);

    void      (*pop_front)(GDeque *thiz);

    void      (*insert)(GDeque *thiz, gpointer position, gpointer first, gpointer last);

    gpointer  (*erase)(GDeque *thiz, gpointer first, gpointer last);

    gpointer  (*remove)(GDeque *thiz, gpointer data);

    void      (*swap)(GDeque *thiz, GDeque *that);

    void      (*resize)(GDeque *thiz, guint n, gpointer data);
};


GDeque* g_deque_alloc(guint n, guint c); //n - count   c - ElementSize


G_END_DECLS


#endif // GDEQUE_H_INCLUDED
