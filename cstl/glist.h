#ifndef GLIST_H_INCLUDED
#define GLIST_H_INCLUDED


#include "gtypes.h"


G_BEGIN_DECLS


typedef struct  _GNode     GNode;

struct _GNode {
    GNode*       (*next)(GNode *_this);
    GNode*       (*prev)(GNode *_this);
    gpointer     (*data)(GNode *_this);
    unsigned int (*size)(GNode *_this);
};

/////////////////////////////////////////////////

typedef struct  _GList     GList;

struct _GList {
    void (*clear)(GList *list);// free node, and free data , but not free list

    void (*free)(GList *list);  // free node, and free data, free list

    GList* (*push_back)(GList *list, gpointer data, unsigned int size);

    GList* (*push_front)(GList *list, gpointer data, unsigned int size);

    GList* (*remove)(GList *list, gpointer data, unsigned int size);// free node, but not free data

    GList* (*pop_back)(GList *list);// free node, but not free data

    GList* (*pop_front)(GList *list);// free node, but not free data

    GNode* (*back)(GList *list);

    GNode* (*front)(GList *list);

    GNode* (*begin)(GList *list);

    GNode* (*end)(GList *list);

    GNode* (*rbegin)(GList *list);

    GNode* (*rend)(GList *list);

    GList* (*reverse)(GList *list);

    GList* (*copy)(GList *list, GList *newlist);

    int    (*find)(GList *list, gpointer data, unsigned int size);

    GNode*  (*at)(GList *list, int index);

    GList* (*insert)(GList *list, int index, gpointer data, unsigned int size);

    int  (*empty)(GList *list);

    int  (*size)(GList *list);
};


GList* g_list_alloc(); //1 -





G_END_DECLS




#endif // GLIST_H_INCLUDED
