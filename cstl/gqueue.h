#ifndef GQUEUE_H_INCLUDED
#define GQUEUE_H_INCLUDED



#include "gtypes.h"


G_BEGIN_DECLS


typedef struct _GQueue    GQueue;

struct _GQueue {
    void      (*free)(GQueue *_this);  // free _this

    guint     (*size)(GQueue *_this);

    guint     (*empty)(GQueue *_this);

    gpointer  (*front)(GQueue *_this);

    gpointer  (*back)(GQueue *_this);

    void      (*push)(GQueue *_this, gpointer data);

    void      (*pop)(GQueue *_this);

    void      (*swap)(GQueue *_this, GQueue *_that);
};


GQueue* g_queue_alloc(guint n, guint c); //n - count   c - ElementSize


G_END_DECLS

#endif // GQUEUE_H_INCLUDED
