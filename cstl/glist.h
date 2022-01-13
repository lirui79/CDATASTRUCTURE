#ifndef GLIST_H_INCLUDED
#define GLIST_H_INCLUDED


#include "gtypes.h"
#include "giterator.h"


G_BEGIN_DECLS


/////////////////////////////////////////////////

typedef struct  _GList     GList;

struct _GList {
    void   (*clear)(GList *thiz);// free node, and free data , but not free list

    void   (*free)(GList *thiz);  // free node, and free data, free list

    guint   (*typesize)(GList *thiz);


    guint  (*empty)(GList *thiz);

    guint  (*size)(GList *thiz);

    gpointer  (*back)(GList *thiz);

    gpointer  (*front)(GList *thiz);

    gpointer  (*at)(GList *thiz, guint index);

    GIterator (*find)(GList *thiz, gconstpointer val);


    GIterator (*begin)(GList *thiz);

    GIterator (*end)(GList *thiz);

    GIterator (*rbegin)(GList *thiz);

    GIterator (*rend)(GList *thiz);


    void   (*push_back)(GList *thiz, gconstpointer val);

    void   (*push_front)(GList *thiz, gconstpointer val);

    void   (*pop_back)(GList *thiz);// free node, but not free data

    void   (*pop_front)(GList *thiz);// free node, but not free data

    GIterator  (*erase)(GList *thiz, GIterator first, GIterator last);

    GIterator  (*remove)(GList *thiz, GIterator position);//


    void   (*assign)(GList *thiz, GIterator first, GIterator last);

    void   (*fill)(GList *thiz, GIterator position, guint n, gconstpointer val);

    void   (*insert)(GList *thiz, GIterator position, GIterator first, GIterator last);

    void   (*reverse)(GList *thiz);

    void   (*swap)(GList *thiz, GList *that);
};

GIterator g_list_iterator(gpointer data, guint typsize, int dir);

GList* g_list_alloc(guint typesize); //1 -





G_END_DECLS




#endif // GLIST_H_INCLUDED
