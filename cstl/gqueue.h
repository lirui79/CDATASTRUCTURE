#ifndef GQUEUE_H_INCLUDED
#define GQUEUE_H_INCLUDED



#include "gtypes.h"


G_BEGIN_DECLS


typedef struct _GQueue    GQueue;

struct _GQueue {
    void       (*free)(GQueue *thiz);  // free thiz

    void       (*clear)(GQueue *thiz);

    guint      (*size)(GQueue *thiz);

    guint      (*empty)(GQueue *thiz);

    GType      (*front)(GQueue *thiz);

    GType      (*back)(GQueue *thiz);

    void      (*push)(GQueue *thiz, GType val);

    void      (*pop)(GQueue *thiz);

    void      (*swap)(GQueue *thiz, GQueue *that);
};


GQueue* g_queue_alloc();


G_END_DECLS

#endif // GQUEUE_H_INCLUDED
