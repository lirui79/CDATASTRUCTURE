#ifndef GITERATOR_H_INCLUDED
#define GITERATOR_H_INCLUDED

#include "gtypes.h"


G_BEGIN_DECLS

typedef struct  _GIterator     GIterator;


struct _GIterator {
    GIterator        (*next)(GIterator *thiz);
    GIterator        (*prev)(GIterator *thiz);
    GIterator        (*forward)(GIterator *thiz, guint n);// next n
    GIterator        (*backward)(GIterator *thiz, guint n);// prev n

    int              (*equal)(GIterator *thiz, GIterator *that);
    int              (*not_equal)(GIterator *thiz, GIterator *that);

    int              (*less)(GIterator *thiz, GIterator *that);
    int              (*less_equal)(GIterator *thiz, GIterator *that);

    int              (*greater)(GIterator *thiz, GIterator *that);
    int              (*greater_equal)(GIterator *thiz, GIterator *that);


    gpointer         (*data)(GIterator *thiz);
    guint            (*size)(GIterator *thiz);
    GIterator        (*set)(GIterator *thiz, gpointer data, guint sz);

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
    guint            dir;
    union {
       struct {// vector list
           gpointer     itdata;
           guint        itsize;
       };


   };
};



G_END_DECLS




#endif // GITERATOR_H_INCLUDED
