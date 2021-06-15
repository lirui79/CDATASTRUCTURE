#ifndef GSTACK_H_INCLUDED
#define GSTACK_H_INCLUDED


#include "gtypes.h"


G_BEGIN_DECLS


typedef struct _GStack     GStack;

struct _GStack {
    void     (*free)(GStack *_this);  // free _this

    gpointer (*top)(GStack *_this);

    guint    (*size)(GStack *_this);

    guint    (*empty)(GStack *_this);

    void     (*push)(GStack *_this, gpointer data);

    void     (*pop)(GStack *_this);

    void     (*swap)(GStack *_this, GStack *_that);
};


GStack* g_stack_alloc(guint n, guint c); //n - count   c - ElementSize


G_END_DECLS

#endif // GSTACK_H_INCLUDED
