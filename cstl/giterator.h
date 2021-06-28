#ifndef GITERATOR_H_INCLUDED
#define GITERATOR_H_INCLUDED

#include "gtypes.h"


G_BEGIN_DECLS

typedef struct  _GIterator     GIterator;


struct _GIterator {
    GIterator        (*next)(GIterator *thiz);
    GIterator        (*prev)(GIterator *thiz);
    GIterator        (*next_n)(GIterator *thiz, guint n);
    GIterator        (*prev_n)(GIterator *thiz, guint n);

    int              (*equal)(GIterator *thiz, GIterator that);
    int              (*not_equal)(GIterator *thiz, GIterator that);

    int              (*less)(GIterator *thiz, GIterator *that);
    int              (*less_equal)(GIterator *thiz, GIterator *that);

    int              (*greater)(GIterator *thiz, GIterator *that);
    int              (*greater_equal)(GIterator *thiz, GIterator *that);


    gpointer         (*get)(GIterator *thiz);
    guint            (*size)(GIterator *thiz);
    GIterator        (*set)(GIterator *thiz, gpointer data, guint sz);

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
    union {
       struct {// vector list
           gpointer    *data;
           guint        dsize;
       };


    };
};



G_END_DECLS




#endif // GITERATOR_H_INCLUDED
