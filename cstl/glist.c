
#include  "glist.h"
#include  <stdio.h>
#include  <stdlib.h>



static GNode*          g_node_alloc(gpointer data, unsigned int size) {
    GNode *node = malloc(sizeof(GNode));
    node->next  = NULL;
    node->prev  = NULL;
    node->data  = data;
    node->size  = size;
    return node;
}


static GNode*          g_node_init(GNode *node, gpointer data, unsigned int size) {
    node->next  = NULL;
    node->prev  = NULL;
    node->data  = data;
    node->size  = size;
    return node;
}

GNode*          g_node_next(GNode  *node) {
    return node->next;
}

GNode*          g_node_prev(GNode  *node) {
    return node->prev;
}

gpointer        g_node_data(GNode  *node) {
    return node->data;
}

unsigned int  g_node_size(GNode  *node) {
    return node->size;
}

static void           g_node_free(GNode  *node) {
    if (node->data != NULL)
        free(node->data);
    free(node);
}

static GNode*          g_node_copy(GNode  *node) {
    GNode *newnode = malloc(sizeof(GNode));
    if (newnode == NULL)
        return newnode;

    newnode->data    = NULL;
    if (node->size > 0) {
        newnode->data = malloc(node->size);
        memcpy(newnode->data, node->data, node->size);
    }
    newnode->size    = node->size;
    newnode->next = NULL;
    newnode->prev = NULL;
    return newnode;
}

static GNode*          g_node_insert(GNode  *node, GNode  *newnode) {
    GNode *next   = node->next;
    node->next    = newnode;
    next->prev    = newnode;
    newnode->next = next;
    newnode->prev = node;
    return node;
}

/////////////////////////////////////////////////////////////////////////////////

static   void  g_list_init_clear(GList *list) {
    GNode *node = NULL, *next = NULL;
    node = list->head.next;
    while (node != &(list->head)) {
        next = node->next;
        free(node);
        node = next;
    }

    list->head.next = &(list->head);
    list->head.prev = &(list->head);
    list->head.data = NULL;
    list->head.size = 0;
    list->length    = 0;
}

static   void  g_list_init_free(GList *list) {
    GNode *node = NULL, *next = NULL;
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
    list->length    = 0;
}


static   void  g_list_alloc_clear(GList *list) {
    GNode *node = NULL, *next = NULL;
    node = list->head.next;
    while (node != &(list->head)) {
        next = node->next;
        free(node);
        node = next;
    }

    free(list);
}

static   void  g_list_alloc_free(GList *list) {
    GNode *node = NULL, *next = NULL;
    node = list->head.next;
    while (node != &(list->head)) {
        next = node->next;
        g_node_free(node);
        node = next;
    }

    free(list);
}


static   GList* g_list_push_back(GList *list, gpointer data, unsigned int size) {
    GNode *node = NULL, *prev = NULL;
    prev = list->head.prev;// last node
    node = g_node_alloc(data, size);
    g_node_insert(prev, node);
    list->length = list->length + 1;
    return list;
}

static   GList* g_list_push_front(GList *list, gpointer data, unsigned int size) {
    GNode *node = NULL, *prev = NULL;
    prev = &(list->head);// head node
    node = g_node_alloc(data, size);
    g_node_insert(prev, node);
    list->length = list->length + 1;
    return list;
}

static   GList* g_list_remove(GList *list, gpointer data, unsigned int size) {
    GNode *node = NULL, *next = NULL, *prev = NULL;
    if ((data == NULL) || (size <= 0)) {
        return list;
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
        list->length = list->length - 1;
        return list;
    }

    return list;
}

static   GList* g_list_pop_back(GList *list) {
    GNode *node = NULL, *next = NULL, *prev = NULL;
    node = list->head.prev;// last node
    if (node == &(list->head))
        return list;
    prev = node->prev;
    next = node->next;
    free(node);
    prev->next = next;
    next->prev = prev;
    list->length = list->length - 1;
    return list;
}

static   GList* g_list_pop_front(GList *list) {
    GNode *node = NULL, *next = NULL, *prev = NULL;
    node = list->head.next;// first node
    if (node == &(list->head))
        return list;
    prev = node->prev;
    next = node->next;
    free(node);
    prev->next = next;
    next->prev = prev;
    list->length = list->length - 1;
    return list;
}

static   GNode* g_list_back(GList *list) {
    GNode *node = list->head.prev;// last node
    if (node == &(list->head))
        return NULL;
    return node;
}

static   GNode* g_list_front(GList *list) {
    GNode *node = list->head.next;// first node
    if (node == &(list->head))
        return NULL;
    return node;
}

static   GNode* g_list_begin(GList *list) {
    return list->head.next;// first node
}

static   GNode* g_list_end(GList *list) {
    return &(list->head);// head node
}

static   GNode* g_list_rbegin(GList *list) {
    return list->head.prev;// last node
}

static   GNode* g_list_rend(GList *list) {
    return &(list->head);// head node
}

static   GList* g_list_reverse(GList *list) {
    GNode *node = NULL, *next = NULL, *prev = NULL;
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
    return list;
}

static   GList* g_list_copy(GList *list, GList *newlist) {
    GNode *node = NULL, *prev = NULL, *newnode = NULL;
    if (newlist == NULL) {
        newlist = g_list_alloc();
    }

    newlist->length = list->length;
    node = list->head.next;
    while (node != &(list->head)) {
        newnode = g_node_copy(node);
        prev = newlist->head.prev;// last node
        g_node_insert(prev, newnode);
        node = node->next;
    }

    return newlist;
}

static   int    g_list_find(GList *list, gpointer data, unsigned int size) {
    GNode *node = NULL;
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

static   GNode*  g_list_at(GList *list, int index) {
    GNode *node = NULL;
    if (index < 0)
        return NULL;

    node = list->head.next;
    while(node != &(list->head)) {
        if (index-- <= 0)
            return node;

        node = node->next;
    }

    return NULL;
}

static   GList* g_list_insert(GList *list, int index, gpointer data, unsigned int size) {
    GNode *newnode = NULL, *node = g_list_at(list, index);
    if (node == NULL) {
        return g_list_push_back(list, data, size);
    }
    newnode = g_node_alloc(data, size);
    node = node->prev;
    g_node_insert(node, newnode);
    list->length = list->length + 1;
    return list;
}

static   int  g_list_empty(GList *list) {
    if (list->length <= 0)
        return 1;
    return 0;
}

static   int  g_list_size(GList *list) {
    return list->length;
}


GList* g_list_alloc() {
    GList *list = (GList *) malloc(sizeof(GList));
    if (list == NULL)
        return NULL;
    list->head.next = &(list->head);
    list->head.prev = &(list->head);
    list->head.data = NULL;
    list->head.size = 0;
    list->length    = 0;

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

GList* g_list_init(GList *list) {
    if (list == NULL)
        return NULL;
    list->head.next = &(list->head);
    list->head.prev = &(list->head);
    list->head.data = NULL;
    list->head.size = 0;
    list->length    = 0;

    /////////////////////////////////////
    list->clear        = g_list_init_clear;
    list->free         = g_list_init_free;

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
