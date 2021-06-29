#ifndef GLIST_H_INCLUDED
#define GLIST_H_INCLUDED


#include "gtypes.h"


G_BEGIN_DECLS


typedef struct  _GNode     GNode;

struct _GNode {
    GNode*       (*next)(GNode *thiz);
    GNode*       (*prev)(GNode *thiz);
    gpointer     (*data)(GNode *thiz);
    guint        (*size)(GNode *thiz);
};

/////////////////////////////////////////////////

typedef struct  _GList     GList;

struct _GList {
    void   (*clear)(GList *thiz);// free node, and free data , but not free list

    void   (*free)(GList *thiz);  // free node, and free data, free list

    guint  (*empty)(GList *thiz);

    guint  (*size)(GList *thiz);

    GNode* (*back)(GList *thiz);

    GNode* (*front)(GList *thiz);

    GNode* (*begin)(GList *thiz);

    GNode* (*end)(GList *thiz);

    GNode* (*rbegin)(GList *thiz);

    GNode* (*rend)(GList *thiz);

    GNode* (*at)(GList *thiz, guint index);

    gint   (*find)(GList *thiz, gpointer data, guint size);

    void   (*push_back)(GList *thiz, gpointer data, guint size);

    void   (*push_front)(GList *thiz, gpointer data, guint size);

    GNode* (*remove)(GList *thiz, gpointer data, guint size);// free node, but not free data

    void   (*pop_back)(GList *thiz);// free node, but not free data

    void   (*pop_front)(GList *thiz);// free node, but not free data

    void   (*reverse)(GList *thiz);

    void   (*assign)(GList *thiz, const GList *that);

    void   (*insert)(GList *thiz, guint index, gpointer data, guint size);

    void   (*swap)(GList *thiz, GList *that);
};


GList* g_list_alloc(); //1 -





G_END_DECLS




#endif // GLIST_H_INCLUDED
