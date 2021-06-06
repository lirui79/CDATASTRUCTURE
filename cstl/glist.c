
#include  "glist.h"
#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>

typedef struct _GDNode GDNode;

struct _GDNode {
    GNode              node;
    GDNode            *next;
    GDNode            *prev;
    gpointer           data;// data pointer
    unsigned int       size;//  data size
};

static GNode*          g_node_next(GNode  *node);

static GNode*          g_node_prev(GNode  *node);

static gpointer        g_node_data(GNode  *node);

static unsigned int  g_node_size(GNode  *node);


static GDNode*          g_node_init(GDNode *node, gpointer data, unsigned int size) {
    node->node.next = g_node_next;
    node->node.prev = g_node_prev;
    node->node.data = g_node_data;
    node->node.size = g_node_size;
    node->next  = NULL;
    node->prev  = NULL;
    node->data  = data;
    node->size  = size;
    return node;
}

static GDNode*          g_node_alloc(gpointer data, unsigned int size) {
    GDNode *node = malloc(sizeof(GDNode));
    return g_node_init(node, data, size);
}

GNode*          g_node_next(GNode  *node) {
    return (GNode*)((GDNode*) node)->next;
}

GNode*          g_node_prev(GNode  *node) {
    return (GNode*)((GDNode*) node)->prev;
}

gpointer        g_node_data(GNode  *node) {
    return ((GDNode*) node)->data;
}

unsigned int  g_node_size(GNode  *node) {
    return ((GDNode*) node)->size;
}

static void           g_node_free(GDNode  *node) {
    if (node->data != NULL)
        free(node->data);
    free(node);
}

static GDNode*          g_node_copy(GDNode  *node) {
    GDNode *newnode = g_node_alloc(NULL, 0);
    if (node->size > 0) {
        newnode->data = malloc(node->size);
        memcpy(newnode->data, node->data, node->size);
    }
    newnode->size    = node->size;
    return newnode;
}

static GDNode*          g_node_insert(GDNode  *node, GDNode  *newnode) {
    GDNode *next   = node->next;
    node->next    = newnode;
    next->prev    = newnode;
    newnode->next = next;
    newnode->prev = node;
    return node;
}

/////////////////////////////////////////////////////////////////////////////////
typedef struct _GDList GDList;

struct _GDList {
    GList                  list;
    GDNode                 head;// list  head  no data        last-> head ->first
    unsigned int           size;
};

static   void  g_list_alloc_clear(GList *glist) {
    GDList* list = (GDList*)glist;
    GDNode *node = NULL, *next = NULL;
    node = list->head.next;
    while (node != &(list->head)) {
        next = node->next;
        g_node_free(node);
        node = next;
    }
    list->head.next = &(list->head);
    list->head.prev = &(list->head);
    list->head.data = NULL;
    list->head.size = 0;
    list->size      = 0;
}

static   void  g_list_alloc_free(GList *glist) {
    GDList* list = (GDList*)glist;
    g_list_alloc_clear(glist);
    free(list);
}

static   GList* g_list_push_back(GList *glist, gpointer data, unsigned int size) {
    GDList* list = (GDList*)glist;
    GDNode *node = NULL, *prev = NULL;
    prev = list->head.prev;// last node
    node = g_node_alloc(data, size);
    g_node_insert(prev, node);
    list->size = list->size + 1;
    return glist;
}

static   GList* g_list_push_front(GList *glist, gpointer data, unsigned int size) {
    GDList* list = (GDList*)glist;
    GDNode *node = NULL, *prev = NULL;
    prev = &(list->head);// head node
    node = g_node_alloc(data, size);
    g_node_insert(prev, node);
    list->size = list->size + 1;
    return glist;
}

static   GList* g_list_remove(GList *glist, gpointer data, unsigned int size) {
    GDList* list = (GDList*)glist;
    GDNode *node = NULL, *next = NULL, *prev = NULL;
    if ((data == NULL) || (size <= 0)) {
        return glist;
    }

    node = list->head.next;
    while(node != &(list->head)) {
        if ((node->data != data) || (node->size != size)) {
            node = node->next;
            continue;
        }
        next = node->next;
        prev = node->prev;
        free(node);
        prev->next = next;
        next->prev = prev;
        list->size = list->size - 1;
        return glist;
    }

    return glist;
}

static   GList* g_list_pop_back(GList *glist) {
    GDList* list = (GDList*)glist;
    GDNode *node = NULL, *next = NULL, *prev = NULL;
    node = list->head.prev;// last node
    if (node == &(list->head))
        return glist;
    prev = node->prev;
    next = node->next;
    free(node);
    prev->next = next;
    next->prev = prev;
    list->size = list->size - 1;
    return glist;
}

static   GList* g_list_pop_front(GList *glist) {
    GDList* list = (GDList*)glist;
    GDNode *node = NULL, *next = NULL, *prev = NULL;
    node = list->head.next;// first node
    if (node == &(list->head))
        return glist;
    prev = node->prev;
    next = node->next;
    free(node);
    prev->next = next;
    next->prev = prev;
    list->size = list->size - 1;
    return glist;
}

static   GNode* g_list_back(GList *glist) {
    GDList* list = (GDList*)glist;
    GDNode *node = list->head.prev;// last node
    if (node == &(list->head))
        return NULL;
    return &(node->node);
}

static   GNode* g_list_front(GList *glist) {
    GDList* list = (GDList*)glist;
    GDNode *node = list->head.next;// first node
    if (node == &(list->head))
        return NULL;
    return &(node->node);
}

static   GNode* g_list_begin(GList *glist) {
    GDList* list = (GDList*)glist;
    return &(list->head.next->node);// first node
}

static   GNode* g_list_end(GList *glist) {
    GDList* list = (GDList*)glist;
    return &(list->head.node);// head node
}

static   GNode* g_list_rbegin(GList *glist) {
    GDList* list = (GDList*)glist;
    return &(list->head.prev->node);// last node
}

static   GNode* g_list_rend(GList *glist) {
    GDList* list = (GDList*)glist;
    return &(list->head.node);// head node
}

static   GList* g_list_reverse(GList *glist) {
    GDList* list = (GDList*)glist;
    GDNode *node = NULL, *next = NULL, *prev = NULL;
    node = list->head.next;// first node

    while (node != &(list->head)) {
        next = node->next;
        prev = node->prev;

        node->next = prev;
        node->prev = next;

        node = next;
    }

    next = list->head.next;
    prev = list->head.prev;

    list->head.next = prev;
    list->head.prev = next;
    return glist;
}

static   GList* g_list_copy(GList *glist, GList *gnewlist) {
    GDList* list = (GDList*)glist;
    GDList* newlist = NULL;
    GDNode *node = NULL, *prev = NULL, *newnode = NULL;
    if (gnewlist == NULL) {
        gnewlist = g_list_alloc();
    }

    newlist = (GDList*)gnewlist;
    newlist->size = list->size;
    node = list->head.next;
    while (node != &(list->head)) {
        newnode = g_node_copy(node);
        prev = newlist->head.prev;// last node
        g_node_insert(prev, newnode);
        node = node->next;
    }

    return gnewlist;
}

static   int    g_list_find(GList *glist, gpointer data, unsigned int size) {
    GDNode *node = NULL;
    GDList* list = (GDList*)glist;
    int index = 0;
    if ((data == NULL) || (size <= 0)) {
        return -1;
    }

    node = list->head.next;
    while(node != &(list->head)) {
        if ((node->data != data) || (node->size != size)) {
            node = node->next;
            ++index;
            continue;
        }

        return index;
    }

    return -2;
}

static   GNode*  g_list_at(GList *glist, int index) {
    GDList* list = (GDList*)glist;
    GDNode *node = NULL;
    if (index < 0)
        return NULL;

    node = list->head.next;
    while(node != &(list->head)) {
        if (index-- <= 0)
            return &(node->node);

        node = node->next;
    }

    return NULL;
}

static   GList* g_list_insert(GList *glist, int index, gpointer data, unsigned int size) {
    GDList* list = (GDList*)glist;
    GDNode *newnode = NULL, *node = (GDNode *)g_list_at(glist, index);
    if (node == NULL) {
        return g_list_push_back(glist, data, size);
    }
    newnode = g_node_alloc(data, size);
    node = node->prev;
    g_node_insert(node, newnode);
    list->size = list->size + 1;
    return glist;
}

static   int  g_list_empty(GList *glist) {
    GDList* list = (GDList*)glist;
    if (list->size <= 0)
        return 1;
    return 0;
}

static   int  g_list_size(GList *glist) {
    GDList* list = (GDList*)glist;
    return list->size;
}


GList* g_list_alloc() {
    GDList *dlist = (GDList *) malloc(sizeof(GDList));
    GList  *list = NULL;
    if (dlist == NULL)
        return NULL;
    g_node_init(&dlist->head, NULL, 0);
    dlist->head.next = &(dlist->head);
    dlist->head.prev = &(dlist->head);
    dlist->size      = 0;
    list = &dlist->list;
    /////////////////////////////////////
    list->clear        = g_list_alloc_clear;
    list->free         = g_list_alloc_free;

    list->push_back    = g_list_push_back;
    list->push_front   = g_list_push_front;
    list->remove       = g_list_remove;
    list->pop_back     = g_list_pop_back;
    list->pop_front    = g_list_pop_front;
    list->back         = g_list_back;
    list->front        = g_list_front;
    list->begin        = g_list_begin;
    list->end          = g_list_end;
    list->rbegin       = g_list_rbegin;
    list->rend         = g_list_rend;
    list->reverse      = g_list_reverse;
    list->copy         = g_list_copy;
    list->find         = g_list_find;
    list->at           = g_list_at;
    list->insert       = g_list_insert;
    list->empty        = g_list_empty;
    list->size         = g_list_size;
    return list;
}

