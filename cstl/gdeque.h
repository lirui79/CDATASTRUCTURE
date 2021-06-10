#ifndef GDEQUE_H_INCLUDED
#define GDEQUE_H_INCLUDED



#include "gtypes.h"


G_BEGIN_DECLS


typedef struct _GStack     GStack;

struct _GStack {
    void (*free)(GStack *_this);  // free _this

    gpointer (*top)(GStack *_this);

    int  (*size)(GStack *_this);

    gpointer  (*empty)(GStack *_this);

    GStack* (*push)(GStack *_this, gpointer data);

    GStack* (*pop)(GStack *_this);

    void (*swap)(GStack *_this, GStack *_that);
};


GStack* g_stack_alloc(int n, int c); //n - count   c - ElementSize

#endif // GDEQUE_H_INCLUDED
