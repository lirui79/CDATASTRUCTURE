#ifndef GAVLTREE_H_INCLUDED
#define GAVLTREE_H_INCLUDED


#include "gtypes.h"


G_BEGIN_DECLS




typedef struct _GAvlNode     GAvlNode;

typedef struct _GAvlTree     _GAvlTree;

struct _GAvlNode {
    GAvlNode* (*left)(GAvlNode *thiz);
    GAvlNode* (*right)(GAvlNode *thiz);
    GAvlNode* (*parent)(GAvlNode *thiz);
    GAvlNode* (*rotate)(GAvlNode *thiz, int dir); // dir: 0 - left rotate   1 - right rotate
    gint      (*height)(GAvlNode *thiz);
    GType     (*data)(GAvlNode *thiz);
    GType     (*key)(GAvlNode *thiz);
};




struct _GAvlTree {
    void       (*free)(GAvlTree    *thiz);

    void       (*clear)(GAvlTree  *thiz);

    guint      (*typesize)(GAvlTree *thiz);

    guint      (*empty)(GAvlTree *thiz);

    guint      (*size)(GAvlTree *thiz);

    guint      (*height)(GAvlTree *thiz);

    guint      (*degree)(GAvlTree *thiz);



    GIterator  (*begin)(GAvlTree *thiz);

    GIterator  (*end)(GAvlTree *thiz);

    GIterator  (*rbegin)(GAvlTree *thiz);

    GIterator  (*rend)(GAvlTree *thiz);

    GIterator  (*min)(GAvlTree *thiz);

    GIterator  (*max)(GAvlTree *thiz);

    GIterator  (*find)(GAvlTree *thiz, gconstpointer key);



    GIterator  (*remove)(GAvlTree *thiz, gconstpointer val);

    GIterator  (*insert)(GAvlTree *thiz, gconstpointer val);

    void      (*traverse)(GAvlTree *thiz);

    void      (*swap)(GAvlTree *thiz, GAvlTree *that);
};


GIterator g_avltree_iterator(gpointer data, guint size, int dir);

GAvlTree* g_avltree_alloc(); //



G_END_DECLS


#endif // GAVLTREE_H_INCLUDED
