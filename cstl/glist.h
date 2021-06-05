#ifndef GLIST_H_INCLUDED
#define GLIST_H_INCLUDED


#include "gtypes.h"


G_BEGIN_DECLS


typedef struct  _GNode     GNode;


struct _GNode {
    GNode             *next;// next node
    GNode             *prev;// prev node
    gpointer           data;// data pointer
    unsigned int     size;//  data size
};

GNode*          g_node_next(GNode  *node);

GNode*          g_node_prev(GNode  *node);

gpointer        g_node_data(GNode  *node);

unsigned int  g_node_size(GNode  *node);



/////////////////////////////////////////////////
/////////////////////////////////////////////////

typedef struct  _GList     GList;

struct _GList {
    GNode                  head;// list  head  no data        last-> head ->first
    unsigned int         length;

    void (*clear)(GList *list);// free node, but not free data

    void (*free)(GList *list);  // free node, and free data

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


GList* g_list_alloc();

GList* g_list_init(GList *list);




G_END_DECLS




#endif // GLIST_H_INCLUDED
