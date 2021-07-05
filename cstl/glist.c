
#include  "glist.h"
#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>





typedef struct  _GNode     GNode;

struct _GNode {
    GNode             *next;
    GNode             *prev;
    GType              data;
};

static GNode*          g_node_next(GNode  *thiz) {
    return  thiz->next;
}

static GNode*          g_node_prev(GNode  *thiz) {
    return  thiz->prev;
}

static GType      g_node_data(GNode  *thiz) {
    return thiz->data;
}



static GIterator       g_list_iterator_next(GIterator *thiz) {
    GNode *node  = (GNode*) thiz->idata.data;
    thiz->idata.data = node->next;
    return thiz[0];
}

static GIterator       g_list_iterator_prev(GIterator *thiz) {
    GNode *node  = (GNode*) thiz->idata.data;
    thiz->idata.data = node->prev;
    return thiz[0];
}

static GIterator       g_list_iterator_forward(GIterator *thiz, guint n) {
    GNode *node  = (GNode*) thiz->idata.data;
    while(n-- > 0) {
        node         = node->next;
        thiz->idata.data = node;
    }
    return thiz[0];
}

static GIterator       g_list_iterator_backward(GIterator *thiz, guint n) {
    GNode *node  = (GNode*) thiz->idata.data;
    while(n-- > 0) {
        node         = node->prev;
        thiz->idata.data = node;
    }
    return thiz[0];
}


static int              g_list_iterator_equal(GIterator *thiz, GIterator *that) {
    if (thiz->idata.data == that->idata.data)
        return 1;
    return 0;
}

static int              g_list_iterator_not_equal(GIterator *thiz, GIterator *that) {
    if (thiz->idata.data == that->idata.data)
        return 0;
    return 1;
}

static int              g_list_iterator_less(GIterator *thiz, GIterator *that) {
    if (thiz->idata.data < that->idata.data)
        return 1;
    return 0;
}

static int              g_list_iterator_less_equal(GIterator *thiz, GIterator *that) {
    if (thiz->idata.data <= that->idata.data)
        return 1;
    return 0;
}

static int              g_list_iterator_greater(GIterator *thiz, GIterator *that) {
    if (thiz->idata.data > that->idata.data)
        return 1;
    return 0;
}

static int              g_list_iterator_greater_equal(GIterator *thiz, GIterator *that) {
    if (thiz->idata.data >= that->idata.data)
        return 1;
    return 0;
}

static GRef       g_list_iterator_get(GIterator *thiz) {
    return thiz->idata;
}

static GIterator        g_list_iterator_set(GIterator *thiz, GRef val) {
    thiz->idata = val;
    return thiz[0];
}

static GType       g_list_iterator_data(GIterator *thiz) {
    GNode *node  = (GNode*) thiz->idata.data;
    return node->data;
}

GIterator g_list_iterator(gpointer data, guint size, int dir) {
    GIterator        thiz  = g_default_iterator(data, size, dir);
    if (dir > 0) {
        thiz.next  = g_list_iterator_next;
        thiz.prev  = g_list_iterator_prev;
        thiz.forward  = g_list_iterator_forward;
        thiz.backward  = g_list_iterator_backward;
    } else {
        thiz.next  = g_list_iterator_prev;
        thiz.prev  = g_list_iterator_next;
        thiz.forward  = g_list_iterator_backward;
        thiz.backward  = g_list_iterator_forward;
    }
//*
    thiz.equal  = g_list_iterator_equal;
    thiz.not_equal  = g_list_iterator_not_equal;
    thiz.less  = g_list_iterator_less;
    thiz.less_equal  = g_list_iterator_less_equal;
    thiz.greater  = g_list_iterator_greater;
    thiz.greater_equal  = g_list_iterator_greater_equal;
    thiz.get  = g_list_iterator_get;
    thiz.set  = g_list_iterator_set;//*/
    thiz.data = g_list_iterator_data;
    return thiz;
}


static GNode*          g_node_init(GNode *thiz, gpointer data, guint size) {
    GType     val = {data, size};
    thiz->next  = NULL;
    thiz->prev  = NULL;
    thiz->data  = val;
    return thiz;
}

static GNode*          g_node_alloc(GType val) {
    GNode *thiz = malloc(sizeof(GNode));
    if(val.data == NULL || val.size <= 0) {
        return g_node_init(thiz, NULL, 0);
    }
    gpointer newdata = malloc(val.size);
    memcpy(newdata, val.data, val.size);
    return g_node_init(thiz, newdata, val.size);
}


static void           g_node_free(GNode  *thiz) {
    if (thiz->data.data != NULL)
        free(thiz->data.data);
    free(thiz);
}

static GNode*          g_node_insert(GNode  *thiz, GNode  *newthis) {
    GNode *next  = thiz->next;
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
    GNode                  head;// list  head  no data        last-> head ->first
    guint                  size;
};

static   void  g_list_clear(GList *gthis) {
    GDList* thiz = (GDList*)gthis;
    GNode *node = NULL, *next = NULL;
    GType     val = {NULL, 0};
    node = thiz->head.next;
    while (node != &(thiz->head)) {
        next = node->next;
        g_node_free(node);
        node = next;
    }
    thiz->head.next = &(thiz->head);
    thiz->head.prev = &(thiz->head);
    thiz->head.data = val;
    thiz->size      = 0;
}

static   void  g_list_free(GList *gthiz) {
    GDList* thiz = (GDList*)gthiz;
    g_list_clear(gthiz);
    free(thiz);
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

static   GType g_list_back(GList *gthis) {
    GDList* thiz = (GDList*)gthis;
    GNode *node = thiz->head.prev;// last node
    return node->data;
}

static   GType g_list_front(GList *gthis) {
    GDList* thiz = (GDList*)gthis;
    GNode *node = thiz->head.next;// first node
    return node->data;
}

static   GType  g_list_at(GList *gthis, guint index) {
    GDList* thiz = (GDList*)gthis;
    GNode *node = NULL;
    GType value = {node, 0};
    if (index < 0)
        return value;

    node = thiz->head.next;
    while(node != &(thiz->head)) {
        if (index-- <= 0)
            return node->data;
        node = node->next;
    }
    return value;
}

static   GIterator    g_list_find(GList *gthis, GType val) {
    GNode *node = NULL;
    GDList* thiz = (GDList*)gthis;
    GType   value = {0};
    GIterator  iterator = g_list_iterator(&(thiz->head), sizeof(GNode), 1);
    if ((val.data == NULL) || (val.size <= 0)) {
        return iterator;
    }

    node = thiz->head.next;
    while(node != &(thiz->head)) {
        value = g_node_data(node);
        if ((val.data != value.data) && (memcmp(val.data, value.data, val.size) != 0)) {
            node = node->next;
            continue;
        }

        iterator.set(&iterator, g_default_ref(node, sizeof(GNode)));
        return iterator;
    }
    return iterator;
}



static   void g_list_push_back(GList *gthis, GType val) {
    GDList* thiz = (GDList*)gthis;
    GNode *node = NULL, *prev = NULL;
    prev = thiz->head.prev;// last node
    node = g_node_alloc(val);
    g_node_insert(prev, node);
    thiz->size = thiz->size + 1;
}

static   void g_list_push_front(GList *gthis, GType val) {
    GDList* thiz = (GDList*)gthis;
    GNode *node = NULL, *prev = NULL;
    prev = &(thiz->head);// head node
    node = g_node_alloc(val);
    g_node_insert(prev, node);
    thiz->size = thiz->size + 1;
}

static   void g_list_pop_back(GList *gthis) {
    GDList* thiz = (GDList*)gthis;
    GNode *node = NULL, *next = NULL, *prev = NULL;
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
    GNode *node = NULL, *next = NULL, *prev = NULL;
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

static   GIterator g_list_erase(GList *gthis, GIterator itfirst, GIterator itlast) {
    GDList    *thiz = (GDList*)gthis;
    GNode     *node = (GNode*)itfirst.get(&itfirst).data, *next = NULL, *prev = NULL, *last = (GNode*)itlast.get(&itlast).data;
    prev   = node->prev;
    next   = node->next;
    while(node != last) {
        thiz->size = thiz->size - 1;
        next = node->next;
        g_node_free(node);
        node = next;
    }

    prev->next = next;
    next->prev = prev;
    return itlast;
}

static   GIterator g_list_remove(GList *gthis, GIterator position) {
    GDList*   thiz = (GDList*)gthis;
    GType val = position.data(&position);
    GNode *node = NULL, *next = NULL, *prev = NULL;
    GIterator  iterator = g_list_iterator(&(thiz->head), sizeof(GNode), 1);
    GIterator  pos      = iterator;
    if ((val.data == NULL) || (val.size <= 0)) {
        return iterator;
    }

    node = thiz->head.next;
    while(node != &(thiz->head)) {//if ((node->data != data) || (node->size != size)) {
        pos.set(&pos, g_default_ref(node, sizeof(GNode)));
        if (pos.equal(&pos, &position) == 0) {
            node = node->next;
            continue;
        }

        next = node->next;
        prev = node->prev;
        g_node_free(node);
        prev->next = next;
        next->prev = prev;
        thiz->size = thiz->size - 1;
        iterator.set(&iterator, g_default_ref(next, sizeof(GNode)));
        return iterator;
    }

    return iterator;
}




static   GIterator g_list_begin(GList *gthis) {
    GDList* thiz = (GDList*)gthis;
    GIterator iterator = g_list_iterator(thiz->head.next, sizeof(GNode), 1);
    return iterator;// head node
}

static   GIterator g_list_end(GList *gthis) {
    GDList* thiz = (GDList*)gthis;
    GIterator iterator = g_list_iterator(&(thiz->head), sizeof(GNode), 1);
    return iterator;// head node
}

static   GIterator g_list_rbegin(GList *gthis) {
    GDList* thiz = (GDList*)gthis;
    GIterator iterator = g_list_iterator(thiz->head.prev, sizeof(GNode), 0);
    return iterator;// head node
}

static   GIterator g_list_rend(GList *gthis) {
    GDList* thiz = (GDList*)gthis;
    GIterator iterator = g_list_iterator(&(thiz->head), sizeof(GNode), 0);
    return iterator;// head node
}

static   void g_list_assign(GList *gthis, GIterator itfirst, GIterator itlast) {
    GDList    *thiz = (GDList*)gthis;
    GNode     *node = (GNode*)itfirst.get(&itfirst).data, *prev = NULL, *last = (GNode*)itlast.get(&itlast).data;
    GNode     *newnode = NULL;

    while(node != last) {
        newnode = g_node_alloc(node->data);
        prev = thiz->head.prev;// last node
        g_node_insert(prev, newnode);
        node = node->next;
        thiz->size = thiz->size + 1;
    }
}

static   void g_list_fill(GList *gthis, GIterator position, guint n, GType val) {
    GDList    *thiz = (GDList*)gthis;
    GNode     *node = (GNode*)position.get(&position).data, *newnode = NULL;

    while(n--) {
        newnode = g_node_alloc(val);
        g_node_insert(node, newnode);
        node = newnode;
        thiz->size = thiz->size + 1;
    }
}

static   void g_list_insert(GList *gthis, GIterator position, GIterator itfirst, GIterator itlast) {
    GDList    *thiz = (GDList*)gthis;
    GNode     *prev = (GNode*)position.get(&position).data, *node = (GNode*)itfirst.get(&itfirst).data, *last = (GNode*)itlast.get(&itlast).data;
    GNode     *newnode = NULL;

    while(node != last) {
        newnode = g_node_alloc(node->data);
        g_node_insert(prev, newnode);
        node = node->next;
        prev = newnode;
        thiz->size = thiz->size + 1;
    }
}

static   void g_list_reverse(GList *gthis) {
    GDList* thiz = (GDList*)gthis;
    GNode *node = NULL, *next = NULL, *prev = NULL;
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

static    void g_list_swap(GList *thiz, GList *that) {
    GNode            *next, *nxt;
    GNode            *prev, *prv;
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
    thiz->clear        = g_list_clear;
    thiz->free         = g_list_free;

    thiz->empty        = g_list_empty;
    thiz->size         = g_list_size;
    thiz->back         = g_list_back;
    thiz->front        = g_list_front;
    thiz->at           = g_list_at;
    thiz->find         = g_list_find;

    thiz->begin        = g_list_begin;
    thiz->end          = g_list_end;
    thiz->rbegin       = g_list_rbegin;
    thiz->rend         = g_list_rend;

    thiz->push_back    = g_list_push_back;
    thiz->push_front   = g_list_push_front;
    thiz->pop_back     = g_list_pop_back;
    thiz->pop_front    = g_list_pop_front;
    thiz->erase        = g_list_erase;
    thiz->remove       = g_list_remove;

    thiz->assign       = g_list_assign;
    thiz->fill         = g_list_fill;
    thiz->insert       = g_list_insert;
    thiz->reverse      = g_list_reverse;
    thiz->swap         = g_list_swap;
    return thiz;
}

