
#include  "glist.h"
#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>

typedef struct _GDNode GDNode;

struct _GDNode {
    GNode             _this;
    GDNode            *next;
    GDNode            *prev;
    gpointer           data;// data pointer
    guint              size;//  data size
};

static GNode*          g_node_next(GNode  *_this);

static GNode*          g_node_prev(GNode  *_this);

static gpointer        g_node_data(GNode  *_this);

static guint           g_node_size(GNode  *_this);


static GDNode*          g_node_init(GDNode *_this, gpointer data, guint size) {
    _this->_this.next = g_node_next;
    _this->_this.prev = g_node_prev;
    _this->_this.data = g_node_data;
    _this->_this.size = g_node_size;
    _this->next  = NULL;
    _this->prev  = NULL;
    _this->data  = data;
    _this->size  = size;
    return _this;
}

static GDNode*          g_node_alloc(gpointer data, guint size) {
    GDNode *_this = malloc(sizeof(GDNode));
    if(data == NULL || size <= 0) {
        return g_node_init(_this, NULL, 0);
    }
    gpointer newdata = malloc(size);
    memcpy(newdata, data, size);
    return g_node_init(_this, newdata, size);
}

GNode*          g_node_next(GNode  *_this) {
    return (GNode*)((GDNode*) _this)->next;
}

GNode*          g_node_prev(GNode  *_this) {
    return (GNode*)((GDNode*) _this)->prev;
}

gpointer        g_node_data(GNode  *_this) {
    return ((GDNode*) _this)->data;
}

guint  g_node_size(GNode  *_this) {
    return ((GDNode*) _this)->size;
}

static void           g_node_free(GDNode  *_this) {
    if (_this->data != NULL)
        free(_this->data);
    free(_this);
}

static GDNode*          g_node_insert(GDNode  *_this, GDNode  *newthis) {
    GDNode *next   = _this->next;
    _this->next    = newthis;
    next->prev    = newthis;
    newthis->next = next;
    newthis->prev = _this;
    return _this;
}

/////////////////////////////////////////////////////////////////////////////////
typedef struct _GDList GDList;

struct _GDList {
    GList                  _this;
    GDNode                 head;// list  head  no data        last-> head ->first
    guint                  size;
};

static   void  g_list_alloc_clear(GList *gthis) {
    GDList* _this = (GDList*)gthis;
    GDNode *node = NULL, *next = NULL;
    node = _this->head.next;
    while (node != &(_this->head)) {
        next = node->next;
        g_node_free(node);
        node = next;
    }
    _this->head.next = &(_this->head);
    _this->head.prev = &(_this->head);
    _this->head.data = NULL;
    _this->head.size = 0;
    _this->size      = 0;
}

static   void  g_list_alloc_free(GList *gthis) {
    GDList* _this = (GDList*)gthis;
    g_list_alloc_clear(gthis);
    free(_this);
}

static   GList* g_list_push_back(GList *gthis, gpointer data, guint size) {
    GDList* _this = (GDList*)gthis;
    GDNode *node = NULL, *prev = NULL;
    prev = _this->head.prev;// last node
    node = g_node_alloc(data, size);
    g_node_insert(prev, node);
    _this->size = _this->size + 1;
    return gthis;
}

static   GList* g_list_push_front(GList *gthis, gpointer data, guint size) {
    GDList* _this = (GDList*)gthis;
    GDNode *node = NULL, *prev = NULL;
    prev = &(_this->head);// head node
    node = g_node_alloc(data, size);
    g_node_insert(prev, node);
    _this->size = _this->size + 1;
    return gthis;
}

static   GList* g_list_remove(GList *gthis, gpointer data, guint size) {
    GDList* _this = (GDList*)gthis;
    GDNode *node = NULL, *next = NULL, *prev = NULL;
    if ((data == NULL) || (size <= 0)) {
        return gthis;
    }

    node = _this->head.next;
    while(node != &(_this->head)) {
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
        _this->size = _this->size - 1;
        return gthis;
    }

    return gthis;
}

static   GList* g_list_pop_back(GList *gthis) {
    GDList* _this = (GDList*)gthis;
    GDNode *node = NULL, *next = NULL, *prev = NULL;
    node = _this->head.prev;// last node
    if (node == &(_this->head))
        return gthis;
    prev = node->prev;
    next = node->next;
    g_node_free(node);
    prev->next = next;
    next->prev = prev;
    _this->size = _this->size - 1;
    return gthis;
}

static   GList* g_list_pop_front(GList *gthis) {
    GDList* _this = (GDList*)gthis;
    GDNode *node = NULL, *next = NULL, *prev = NULL;
    node = _this->head.next;// first node
    if (node == &(_this->head))
        return gthis;
    prev = node->prev;
    next = node->next;
    g_node_free(node);
    prev->next = next;
    next->prev = prev;
    _this->size = _this->size - 1;
    return gthis;
}

static   GNode* g_list_back(GList *gthis) {
    GDList* _this = (GDList*)gthis;
    GDNode *node = _this->head.prev;// last node
    if (node == &(_this->head))
        return NULL;
    return &(node->_this);
}

static   GNode* g_list_front(GList *gthis) {
    GDList* _this = (GDList*)gthis;
    GDNode *node = _this->head.next;// first node
    if (node == &(_this->head))
        return NULL;
    return &(node->_this);
}

static   GNode* g_list_begin(GList *gthis) {
    GDList* _this = (GDList*)gthis;
    return &(_this->head.next->_this);// first node
}

static   GNode* g_list_end(GList *gthis) {
    GDList* _this = (GDList*)gthis;
    return &(_this->head._this);// head node
}

static   GNode* g_list_rbegin(GList *gthis) {
    GDList* _this = (GDList*)gthis;
    return &(_this->head.prev->_this);// last node
}

static   GNode* g_list_rend(GList *gthis) {
    GDList* _this = (GDList*)gthis;
    return &(_this->head._this);// head node
}

static   GList* g_list_reverse(GList *gthis) {
    GDList* _this = (GDList*)gthis;
    GDNode *node = NULL, *next = NULL, *prev = NULL;
    node = _this->head.next;// first node

    while (node != &(_this->head)) {
        next = node->next;
        prev = node->prev;
        node->next = prev;
        node->prev = next;
        node = next;
    }
    next = _this->head.next;
    prev = _this->head.prev;
    _this->head.next = prev;
    _this->head.prev = next;
    return gthis;
}

static   GList* g_list_assign(GList *gthis, const GList *gothis) {
    GDList* _this = (GDList*)gthis;
    GDList* _othis = NULL;
    GDNode *node = NULL, *prev = NULL, *newnode = NULL;
    if (gothis == NULL) {
        return gthis;
    }

    _othis = (GDList*)gothis;
    _this->size = _othis->size;
    node = _othis->head.next;
    while (node != &(_othis->head)) {
        newnode = g_node_alloc(node->data, node->size);
        prev = _this->head.prev;// last node
        g_node_insert(prev, newnode);
        node = node->next;
    }
    return gthis;
}

static   gint    g_list_find(GList *gthis, gpointer data, guint size) {
    GDNode *node = NULL;
    GDList* _this = (GDList*)gthis;
    guint index = 0;
    if ((data == NULL) || (size <= 0)) {
        return -1;
    }

    node = _this->head.next;
    while(node != &(_this->head)) {
        //if ((node->data != data) || (node->size != size)) {
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
    GDList* _this = (GDList*)gthis;
    GDNode *node = NULL;
    if (index < 0)
        return NULL;

    node = _this->head.next;
    while(node != &(_this->head)) {
        if (index-- <= 0)
            return &(node->_this);
        node = node->next;
    }
    return NULL;
}

static   GList* g_list_insert(GList *gthis, guint index, gpointer data, guint size) {
    GDList* _this = (GDList*)gthis;
    GDNode *newnode = NULL, *node = (GDNode *)g_list_at(gthis, index);
    if (node == NULL) {
        return g_list_push_back(gthis, data, size);
    }
    newnode = g_node_alloc(data, size);
    node = node->prev;
    g_node_insert(node, newnode);
    _this->size = _this->size + 1;
    return gthis;
}

static   guint  g_list_empty(GList *gthis) {
    GDList* _this = (GDList*)gthis;
    if (_this->size <= 0)
        return 1;
    return 0;
}

static   guint  g_list_size(GList *gthis) {
    GDList* _this = (GDList*)gthis;
    return _this->size;
}

static    void g_list_swap(GList *_this, GList *_that) {
    GDNode            *next, *nxt;
    GDNode            *prev, *prv;
    GDList *_gthis = (GDList*) _this, *_gthat = (GDList*)_that;
    if (_that == NULL) {
        return;
    }

    next = _gthis->head.next;
    prev = _gthis->head.prev;
    nxt  = _gthat->head.next;
    prv  = _gthat->head.prev;

    if (nxt != &(_gthat->head)) {
        _gthis->head.next = nxt;
        _gthis->head.prev = prv;
        nxt->prev = &(_gthis->head);
        prv->next = &(_gthis->head);
    } else {
        _gthis->head.next = &(_gthis->head);
        _gthis->head.prev = &(_gthis->head);
    }

    if (next != &(_gthis->head)) {
        _gthat->head.next = next;
        _gthat->head.prev = prev;
        next->prev = &(_gthat->head);
        prev->next = &(_gthat->head);
    } else {
        _gthat->head.next = &(_gthat->head);
        _gthat->head.prev = &(_gthat->head);
    }
    return;
}

GList* g_list_alloc() {
    GDList *gthis = (GDList *) malloc(sizeof(GDList));
    GList  *_this = NULL;
    if (gthis == NULL)
        return NULL;
    g_node_init(&gthis->head, NULL, 0);
    gthis->head.next = &(gthis->head);
    gthis->head.prev = &(gthis->head);
    gthis->size      = 0;
    _this = &gthis->_this;
    /////////////////////////////////////
    _this->clear        = g_list_alloc_clear;
    _this->free         = g_list_alloc_free;

    _this->push_back    = g_list_push_back;
    _this->push_front   = g_list_push_front;
    _this->remove       = g_list_remove;
    _this->pop_back     = g_list_pop_back;
    _this->pop_front    = g_list_pop_front;
    _this->back         = g_list_back;
    _this->front        = g_list_front;
    _this->begin        = g_list_begin;
    _this->end          = g_list_end;
    _this->rbegin       = g_list_rbegin;
    _this->rend         = g_list_rend;
    _this->reverse      = g_list_reverse;
    _this->assign       = g_list_assign;
    _this->find         = g_list_find;
    _this->at           = g_list_at;
    _this->insert       = g_list_insert;
    _this->empty        = g_list_empty;
    _this->size         = g_list_size;
    _this->swap         = g_list_swap;
    return _this;
}

