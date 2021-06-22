#ifndef GDEQUE_H_INCLUDED
#define GDEQUE_H_INCLUDED



#include "gtypes.h"


G_BEGIN_DECLS


typedef struct _GDeque     GDeque;

struct _GDeque {
    void      (*free)(GDeque *_this);

    void      (*clear)(GDeque *_this);

    guint     (*size)(GDeque *_this);

    guint     (*empty)(GDeque *_this);

    gpointer  (*begin)(GDeque *_this);

    gpointer  (*end)(GDeque *_this);

    gpointer  (*backward)(GDeque *_this, gpointer position, gint n);

    gpointer  (*front)(GDeque *_this);

    gpointer  (*back)(GDeque *_this);

    gpointer  (*at)(GDeque *_this, guint index);

    gpointer  (*rbegin)(GDeque *_this);

    gpointer  (*rend)(GDeque *_this);

    gpointer  (*forward)(GDeque *_this, gpointer position, gint n);

    void      (*assign)(GDeque *_this, gpointer first, gpointer last);

    void      (*fill)(GDeque *_this, gpointer position, guint n, gpointer data);

    void      (*push_back)(GDeque *_this, gpointer data);

    void      (*push_front)(GDeque *_this, gpointer data);

    void      (*pop_back)(GDeque *_this);

    void      (*pop_front)(GDeque *_this);

    void      (*insert)(GDeque *_this, gpointer position, gpointer first, gpointer last);

    gpointer  (*erase)(GDeque *_this, gpointer first, gpointer last);

    gpointer  (*remove)(GDeque *_this, gpointer data);

    void      (*swap)(GDeque *_this, GDeque *_that);

    void      (*resize)(GDeque *_this, guint n, gpointer data);
};


GDeque* g_deque_alloc(guint n, guint c); //n - count   c - ElementSize


G_END_DECLS


#endif // GDEQUE_H_INCLUDED
