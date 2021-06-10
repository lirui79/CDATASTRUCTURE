#ifndef GQUEUE_H_INCLUDED
#define GQUEUE_H_INCLUDED



#include "gtypes.h"


G_BEGIN_DECLS


typedef struct _GQueue    GQueue;

struct _GQueue {
    void (*free)(GQueue *_this);  // free _this

    guint  (*size)(GQueue *_this);

    guint  (*empty)(GQueue *_this);

    gpointer (*front)(GQueue *_this);

    gpointer (*back)(GQueue *_this);

    GQueue* (*push)(GQueue *_this, gpointer data);

    GQueue* (*pop)(GQueue *_this);

    void    (*swap)(GQueue *_this, GQueue *_that);
};


GQueue* g_queue_alloc(int n, int c); //n - count   c - ElementSize

#endif // GQUEUE_H_INCLUDED
