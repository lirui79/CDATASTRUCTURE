#ifndef GSTACK_H_INCLUDED
#define GSTACK_H_INCLUDED


#include "gtypes.h"


G_BEGIN_DECLS


typedef struct _GStack     GStack;

struct _GStack {
    void      (*free)(GStack *thiz);  // free thiz

    void      (*clear)(GStack *thiz);

    gpointer  (*top)(GStack *thiz);

    guint     (*typesize)(GStack *thiz);

    guint     (*size)(GStack *thiz);

    guint     (*empty)(GStack *thiz);

    void     (*push)(GStack *thiz, gconstpointer val);

    void     (*pop)(GStack *thiz);

    void     (*swap)(GStack *thiz, GStack *that);
};


GStack* g_stack_alloc(guint cnt, guint size); //n - count   c - ElementSize


G_END_DECLS

#endif // GSTACK_H_INCLUDED
