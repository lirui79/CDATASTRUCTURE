
#include  "glist.h"
#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>

typedef struct _GDNode GDNode;

struct _GDNode {
    GNode             thiz;
    GDNode            *next;
    GDNode            *prev;
    gpointer           data;// data pointer
    guint              size;//  data size
};

static GNode*          g_node_next(GNode  *thiz);

static GNode*          g_node_prev(GNode  *thiz);

static gpointer        g_node_data(GNode  *thiz);

static guint           g_node_size(GNode  *thiz);


static GDNode*          g_node_init(GDNode *thiz, gpointer data, guint size) {
    thiz->thiz.next = g_node_next;
    thiz->thiz.prev = g_node_prev;
    thiz->thiz.data = g_node_data;
    thiz->thiz.size = g_node_size;
    thiz->next  = NULL;
    thiz->prev  = NULL;
    thiz->data  = data;
    thiz->size  = size;
    return thiz;
}

static GDNode*          g_node_alloc(gpointer data, guint size) {
    GDNode *thiz = malloc(sizeof(GDNode));
    if(data == NULL || size <= 0) {
        return g_node_init(thiz, NULL, 0);
    }
    gpointer newdata = malloc(size);
    memcpy(newdata, data, size);
    return g_node_init(thiz, newdata, size);
}

GNode*          g_node_next(GNode  *thiz) {
    return (GNode*)((GDNode*) thiz)->next;
}

GNode*          g_node_prev(GNode  *thiz) {
    return (GNode*)((GDNode*) thiz)->prev;
}

gpointer        g_node_data(GNode  *thiz) {
    return ((GDNode*) thiz)->data;
}

guint  g_node_size(GNode  *thiz) {
    return ((GDNode*) thiz)->size;
}

static void           g_node_free(GDNode  *thiz) {
    if (thiz->data != NULL)
        free(thiz->data);
    free(thiz);
}

static GDNode*          g_node_insert(GDNode  *thiz, GDNode  *newthis) {
    GDNode *next  = thiz->next;
    thiz->next   = newthis;
    next->prev    = newthis;
    newthis->next = next;
    newthis->prev = thiz;
    return thiz;
}

/////////////////////////////////////////////////////////////////////////////////
typedef struct _GDList GDList;

struct _GDList {
    GList                  thiz;
    GDNode                 head;// list  head  no data        last-> head ->first
    guint                  size;
};

static   void  g_list_alloc_clear(GList *gthis) {
    GDList* thiz = (GDList*)gthis;
    GDNode *node = NULL, *next = NULL;
    node = thiz->head.next;
    while (node != &(thiz->head)) {
        next = node->next;
        g_node_free(node);
        node = next;
    }
    thiz->head.next = &(thiz->head);
    thiz->head.prev = &(thiz->head);
    thiz->head.data = NULL;
    thiz->head.size = 0;
    thiz->size      = 0;
}

static   void  g_list_alloc_free(GList *gthis) {
    GDList* thiz = (GDList*)gthis;
    g_list_alloc_clear(gthis);
    free(thiz);
}

static   void g_list_push_back(GList *gthis, gpointer data, guint size) {
    GDList* thiz = (GDList*)gthis;
    GDNode *node = NULL, *prev = NULL;
    prev = thiz->head.prev;// last node
    node = g_node_alloc(data, size);
    g_node_insert(prev, node);
    thiz->size = thiz->size + 1;
}

static   void g_list_push_front(GList *gthis, gpointer data, guint size) {
    GDList* thiz = (GDList*)gthis;
    GDNode *node = NULL, *prev = NULL;
    prev = &(thiz->head);// head node
    node = g_node_alloc(data, size);
    g_node_insert(prev, node);
    thiz->size = thiz->size + 1;
}

static   GNode* g_list_remove(GList *gthis, gpointer data, guint size) {
    GDList* thiz = (GDList*)gthis;
    GDNode *node = NULL, *next = NULL, *prev = NULL;
    if ((data == NULL) || (size <= 0)) {
        return NULL;
    }

    node = thiz->head.next;
    while(node != &(thiz->head)) {
        //if ((node->data != data) || (node->size != size)) {
        if (memcmp(data, node->data, size) != 0) {
            node = node->next;
            continue;
        }
        next = node->next;
        prev = node->prev;
        g_node_free(node);
        prev->next = next;
        next->prev = prev;
        thiz->size = thiz->size - 1;
        return (GNode*)(&next->thiz);
    }

    return NULL;
}

static   void g_list_pop_back(GList *gthis) {
    GDList* thiz = (GDList*)gthis;
    GDNode *node = NULL, *next = NULL, *prev = NULL;
    node = thiz->head.prev;// last node
    if (node == &(thiz->head))
        return;
    prev = node->prev;
    next = node->next;
    g_node_free(node);
    prev->next = next;
    next->prev = prev;
    thiz->size = thiz->size - 1;
}

static   void g_list_pop_front(GList *gthis) {
    GDList* thiz = (GDList*)gthis;
    GDNode *node = NULL, *next = NULL, *prev = NULL;
    node = thiz->head.next;// first node
    if (node == &(thiz->head))
        return;
    prev = node->prev;
    next = node->next;
    g_node_free(node);
    prev->next = next;
    next->prev = prev;
    thiz->size = thiz->size - 1;
}

static   GNode* g_list_back(GList *gthis) {
    GDList* thiz = (GDList*)gthis;
    GDNode *node = thiz->head.prev;// last node
    if (node == &(thiz->head))
        return NULL;
    return &(node->thiz);
}

static   GNode* g_list_front(GList *gthis) {
    GDList* thiz = (GDList*)gthis;
    GDNode *node = thiz->head.next;// first node
    if (node == &(thiz->head))
        return NULL;
    return &(node->thiz);
}

static   GNode* g_list_begin(GList *gthis) {
    GDList* thiz = (GDList*)gthis;
    return &(thiz->head.next->thiz);// first node
}

static   GNode* g_list_end(GList *gthis) {
    GDList* thiz = (GDList*)gthis;
    return &(thiz->head.thiz);// head node
}

static   GNode* g_list_rbegin(GList *gthis) {
    GDList* thiz = (GDList*)gthis;
    return &(thiz->head.prev->thiz);// last node
}

static   GNode* g_list_rend(GList *gthis) {
    GDList* thiz = (GDList*)gthis;
    return &(thiz->head.thiz);// head node
}

static   void g_list_reverse(GList *gthis) {
    GDList* thiz = (GDList*)gthis;
    GDNode *node = NULL, *next = NULL, *prev = NULL;
    node = thiz->head.next;// first node

    while (node != &(thiz->head)) {
        next = node->next;
        prev = node->prev;
        node->next = prev;
        node->prev = next;
        node = next;
    }
    next = thiz->head.next;
    prev = thiz->head.prev;
    thiz->head.next = prev;
    thiz->head.prev = next;
}

static   void g_list_assign(GList *gthis, const GList *gothis) {
    GDList* thiz = (GDList*)gthis;
    GDList* _othis = NULL;
    GDNode *node = NULL, *prev = NULL, *newnode = NULL;
    if (gothis == NULL) {
        return;
    }

    _othis = (GDList*)gothis;
    thiz->size = _othis->size;
    node = _othis->head.next;
    while (node != &(_othis->head)) {
        newnode = g_node_alloc(node->data, node->size);
        prev = thiz->head.prev;// last node
        g_node_insert(prev, newnode);
        node = node->next;
    }
}

static   gint    g_list_find(GList *gthis, gpointer data, guint size) {
    GDNode *node = NULL;
    GDList* thiz = (GDList*)gthis;
    guint index = 0;
    if ((data == NULL) || (size <= 0)) {
        return -1;
    }

    node = thiz->head.next;
    while(node != &(thiz->head)) {
        if (memcmp(data, node->data, size) != 0) {
            node = node->next;
            ++index;
            continue;
        }
        return index;
    }
    return -2;
}

static   GNode*  g_list_at(GList *gthis, guint index) {
    GDList* thiz = (GDList*)gthis;
    GDNode *node = NULL;
    if (index < 0)
        return NULL;

    node = thiz->head.next;
    while(node != &(thiz->head)) {
        if (index-- <= 0)
            return &(node->thiz);
        node = node->next;
    }
    return NULL;
}

static   void g_list_insert(GList *gthis, guint index, gpointer data, guint size) {
    GDList* thiz = (GDList*)gthis;
    GDNode *newnode = NULL, *node = (GDNode *)g_list_at(gthis, index);
    if (node == NULL) {
        g_list_push_back(gthis, data, size);
        return;
    }
    newnode = g_node_alloc(data, size);
    node = node->prev;
    g_node_insert(node, newnode);
    thiz->size = thiz->size + 1;
}

static   guint  g_list_empty(GList *gthis) {
    GDList* thiz = (GDList*)gthis;
    if (thiz->size <= 0)
        return 1;
    return 0;
}

static   guint  g_list_size(GList *gthis) {
    GDList* thiz = (GDList*)gthis;
    return thiz->size;
}

static    void g_list_swap(GList *thiz, GList *that) {
    GDNode            *next, *nxt;
    GDNode            *prev, *prv;
    GDList *gthiz = (GDList*) thiz, *gthat = (GDList*)that;
    guint             size = 0;
    if (that == NULL) {
        return;
    }

    next = gthiz->head.next;
    prev = gthiz->head.prev;
    size = gthiz->size;
    nxt  = gthat->head.next;
    prv  = gthat->head.prev;
    gthiz->size = gthat->size;
    gthat->size = size;

    if (nxt != &(gthat->head)) {
        gthiz->head.next = nxt;
        gthiz->head.prev = prv;
        nxt->prev = &(gthiz->head);
        prv->next = &(gthiz->head);
    } else {
        gthiz->head.next = &(gthiz->head);
        gthiz->head.prev = &(gthiz->head);
    }

    if (next != &(gthiz->head)) {
        gthat->head.next = next;
        gthat->head.prev = prev;
        next->prev = &(gthat->head);
        prev->next = &(gthat->head);
    } else {
        gthat->head.next = &(gthat->head);
        gthat->head.prev = &(gthat->head);
    }
}

GList* g_list_alloc() {
    GDList *gthis = (GDList *) malloc(sizeof(GDList));
    GList  *thiz = NULL;
    if (gthis == NULL) {
        return NULL;
    }

    g_node_init(&gthis->head, NULL, 0);
    gthis->head.next = &(gthis->head);
    gthis->head.prev = &(gthis->head);
    gthis->size      = 0;
    thiz = &gthis->thiz;
    /////////////////////////////////////
    thiz->clear        = g_list_alloc_clear;
    thiz->free         = g_list_alloc_free;

    thiz->push_back    = g_list_push_back;
    thiz->push_front   = g_list_push_front;
    thiz->remove       = g_list_remove;
    thiz->pop_back     = g_list_pop_back;
    thiz->pop_front    = g_list_pop_front;
    thiz->back         = g_list_back;
    thiz->front        = g_list_front;
    thiz->begin        = g_list_begin;
    thiz->end          = g_list_end;
    thiz->rbegin       = g_list_rbegin;
    thiz->rend         = g_list_rend;
    thiz->reverse      = g_list_reverse;
    thiz->assign       = g_list_assign;
    thiz->find         = g_list_find;
    thiz->at           = g_list_at;
    thiz->insert       = g_list_insert;
    thiz->empty        = g_list_empty;
    thiz->size         = g_list_size;
    thiz->swap         = g_list_swap;
    return thiz;
}

