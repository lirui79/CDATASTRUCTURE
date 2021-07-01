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


    GType            (*get)(GIterator *thiz);
    GIterator        (*set)(GIterator *thiz, GType val);

    GType            (*data)(GIterator *thiz);

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
    guint            dir;
    union {
       GType         idata;// vector list

   };
};


GIterator g_default_iterator(gpointer data, guint size, int dir);


G_END_DECLS




#endif // GITERATOR_H_INCLUDED
