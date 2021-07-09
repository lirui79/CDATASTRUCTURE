
#include "gavltree.h"
#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>


typedef struct _GAvlDNode     GAvlDNode;

typedef struct _GAvlDTree     GAvlDTree;



struct _GAvlDNode {
    GAvlNode           thiz;
    GAvlDNode         *left;
    GAvlDNode         *right;
    GAvlDNode         *parent;
    gint               height;
    GType              data;
    GType              key;
};

static  GAvlNode* g_avlnode_left(GAvlNode *thiz) {
    GAvlDNode* gthiz = (GAvlDNode*) thiz;
    return gthiz->left;
}

static  GAvlNode* g_avlnode_right(GAvlNode *thiz) {
    GAvlDNode* gthiz = (GAvlDNode*) thiz;
    return gthiz->right;
}

static  GAvlNode* g_avlnode_parent(GAvlNode *thiz) {
    GAvlDNode* gthiz = (GAvlDNode*) thiz;
    return gthiz->parent;
}

static  GAvlNode* g_avlnode_rotate(GAvlNode *thiz, int dir) { // dir: 0 - left rotate   1 - right rotate
    GAvlDNode* gthiz = (GAvlDNode*) thiz;
    return gthiz->left;
}

static  gint      g_avlnode_height(GAvlNode *thiz) {
    GAvlDNode* gthiz = (GAvlDNode*) thiz;
    return gthiz->height;
}

static  GType     g_avlnode_data(GAvlNode *thiz) {
    GAvlDNode* gthiz = (GAvlDNode*) thiz;
    return gthiz->data;
}

static  GType     g_avlnode_key(GAvlNode *thiz) {
    GAvlDNode* gthiz = (GAvlDNode*) thiz;
    return gthiz->key;
}


static GAvlDNode* g_avlnode_alloc(GType data, GType key) {
    GAvlDNode *gthiz = (GAvlDNode*) malloc(sizeof(GAvlDNode));
    gthiz->thiz.left   = g_avlnode_left;
    gthiz->thiz.right  = g_avlnode_right;
    gthiz->thiz.parent = g_avlnode_parent;
    gthiz->thiz.rotate = g_avlnode_rotate;
    gthiz->thiz.height = g_avlnode_height;
    gthiz->thiz.data   = g_avlnode_data;
    gthiz->thiz.key    = g_avlnode_key;

    gthiz->left        = NULL;
    gthiz->right       = NULL;
    gthiz->parent      = NULL;
    gthiz->height      = 0;
    gthiz->data        = data;
    gthiz->key         = key;
    return gthiz;
}





struct _GAvlDTree {
    GAvlTree      thiz;
    GAvlDNode    *root;

};



static    void       g_avltree_free(GAvlTree    *thiz) {

}

static    void       g_avltree_clear(GAvlTree  *thiz) {

}



static    guint      g_avltree_empty(GAvlTree *thiz) {

}

static    guint      g_avltree_size(GAvlTree *thiz) {

}

static    guint      g_avltree_height(GAvlTree *thiz) {

}

static    guint      g_avltree_degree(GAvlTree *thiz) {

}



static    GIterator  g_avltree_begin(GAvlTree *thiz) {

}

static    GIterator  g_avltree_end(GAvlTree *thiz) {

}

static    GIterator  g_avltree_rbegin(GAvlTree *thiz) {

}

static    GIterator  g_avltree_rend(GAvlTree *thiz) {

}

static    GIterator  g_avltree_min(GAvlTree *thiz) {

}

static    GIterator  g_avltree_max(GAvlTree *thiz) {

}

static    GIterator  g_avltree_find(GAvlTree *thiz, GType key) {

}



static    GIterator  g_avltree_remove(GAvlTree *thiz, GType val) {

}
static    GIterator  g_avltree_insert(GAvlTree *thiz, GType val) {

}

static    void       g_avltree_traverse(GAvlTree *thiz) {

}



static    void       g_avltree_swap(GAvlTree *thiz, GAvlTree *that) {

}


GIterator g_avltree_iterator(gpointer data, guint size, int dir) {

}

GAvlTree* g_avltree_alloc() {
    GAvlDTree *gthiz = (GAvlDTree*) malloc(sizeof(GAvlDTree));
    GAvlTree  *thiz = &(gthiz->thiz);


    thiz->free   = g_avltree_free;

    thiz->clear  = g_avltree_clear;


    thiz->empty  = g_avltree_empty;

    thiz->size  = g_avltree_size;

    thiz->height  = g_avltree_height;

    thiz->degree  = g_avltree_degree;



    thiz->begin  = g_avltree_begin;

    thiz->end  = g_avltree_end;

    thiz->rbegin  = g_avltree_rbegin;

    thiz->rend  = g_avltree_rend;

    thiz->min  = g_avltree_min;

    thiz->max  = g_avltree_max;

    thiz->find  = g_avltree_find;



    thiz->remove  = g_avltree_remove;

    thiz->insert  = g_avltree_insert;

    thiz->traverse  = g_avltree_traverse;


    thiz->swap  = g_avltree_swap;

    return thiz;
}




