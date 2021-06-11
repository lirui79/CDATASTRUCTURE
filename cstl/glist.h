#ifndef GLIST_H_INCLUDED
#define GLIST_H_INCLUDED


#include "gtypes.h"


G_BEGIN_DECLS


typedef struct  _GNode     GNode;

struct _GNode {
    GNode*       (*next)(GNode *_this);
    GNode*       (*prev)(GNode *_this);
    gpointer     (*data)(GNode *_this);
    guint        (*size)(GNode *_this);
};

/////////////////////////////////////////////////

typedef struct  _GList     GList;

struct _GList {
    void   (*clear)(GList *_this);// free node, and free data , but not free list

    void   (*free)(GList *_this);  // free node, and free data, free list

    void   (*push_back)(GList *_this, gpointer data, guint size);

    void   (*push_front)(GList *_this, gpointer data, guint size);

    GNode* (*remove)(GList *_this, gpointer data, guint size);// free node, but not free data

    void   (*pop_back)(GList *_this);// free node, but not free data

    void   (*pop_front)(GList *_this);// free node, but not free data

    GNode* (*back)(GList *_this);

    GNode* (*front)(GList *_this);

    GNode* (*begin)(GList *_this);

    GNode* (*end)(GList *_this);

    GNode* (*rbegin)(GList *_this);

    GNode* (*rend)(GList *_this);

    void   (*reverse)(GList *_this);

    void   (*assign)(GList *_this, const GList *_othis);

    gint   (*find)(GList *_this, gpointer data, guint size);

    GNode* (*at)(GList *_this, guint index);

    void   (*insert)(GList *_this, guint index, gpointer data, guint size);

    guint  (*empty)(GList *_this);

    guint  (*size)(GList *_this);

    void   (*swap)(GList *_this, GList *_that);
};


GList* g_list_alloc(); //1 -





G_END_DECLS




#endif // GLIST_H_INCLUDED
