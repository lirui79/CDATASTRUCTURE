#ifndef GITERATOR_H_INCLUDED
#define GITERATOR_H_INCLUDED

#include "gtypes.h"


G_BEGIN_DECLS



typedef struct  _GRef          GRef;

struct _GRef {
    union {
        struct {// vector list array
            gpointer     data;
            guint        size;
        };

        struct {// deque
            gpointer container;
            gint     rows;
            gint     cols;
        };


    };
};

GRef g_default_ref(gpointer data, guint size);


typedef struct  _GIterator     GIterator;

struct _GIterator {
    GIterator        (*next)(GIterator *thiz);
    GIterator        (*prev)(GIterator *thiz);
    GIterator        (*forward)(GIterator *thiz, guint n);// next n
    GIterator        (*backward)(GIterator *thiz, guint n);// prev n

    int              (*equal)(GIterator *thiz, GIterator *that);
    int              (*unequal)(GIterator *thiz, GIterator *that);

    int              (*less)(GIterator *thiz, GIterator *that);
    int              (*lequal)(GIterator *thiz, GIterator *that);// less and equal

    int              (*greater)(GIterator *thiz, GIterator *that);
    int              (*gequal)(GIterator *thiz, GIterator *that);// greater and equal


    GRef             (*get)(GIterator *thiz);
    GIterator        (*set)(GIterator *thiz, GRef val);

    gpointer         (*data)(GIterator *thiz);

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
    guint            dir;

    GRef             idata;

};


GIterator g_default_iterator(gpointer data, guint size, int dir);


G_END_DECLS




#endif // GITERATOR_H_INCLUDED
