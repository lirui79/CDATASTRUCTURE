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
    void (*clear)(GList *_this);// free node, and free data , but not free list

    void (*free)(GList *_this);  // free node, and free data, free list

    GList* (*push_back)(GList *_this, gpointer data, unsigned int size);

    GList* (*push_front)(GList *_this, gpointer data, unsigned int size);

    GList* (*remove)(GList *_this, gpointer data, unsigned int size);// free node, but not free data

    GList* (*pop_back)(GList *_this);// free node, but not free data

    GList* (*pop_front)(GList *_this);// free node, but not free data

    GNode* (*back)(GList *_this);

    GNode* (*front)(GList *_this);

    GNode* (*begin)(GList *_this);

    GNode* (*end)(GList *_this);

    GNode* (*rbegin)(GList *_this);

    GNode* (*rend)(GList *_this);

    GList* (*reverse)(GList *_this);

    GList* (*assign)(GList *_this, const GList *_othis);

    int    (*find)(GList *_this, gpointer data, unsigned int size);

    GNode*  (*at)(GList *_this, int index);

    GList* (*insert)(GList *_this, int index, gpointer data, unsigned int size);

    int  (*empty)(GList *_this);

    int  (*size)(GList *_this);
};


GList* g_list_alloc(); //1 -





G_END_DECLS




#endif // GLIST_H_INCLUDED
