
#include "gdeque.h"
#include "gvector.h"
#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>


typedef struct _GDDeque  GDDeque;

/*************************************/
/*
map(row, col)
 ++++++++
 ++++++++
 ++++++++
 ++++++++
*/

struct _GDDeque {
    GDeque     thiz;
    gpointer  *mptr;
    gint       rows;// row number
    gint       cols;// col number
    guint      typesize;
    struct {
        gint   rows; // row idx
        gint   cols;//  col idx
    }first, last;
};

static int  g_deque_in(GDDeque* thiz, gint rows, gint cols) {
    if (thiz->first.rows > rows) {
        return 0;
    }

    if ((thiz->first.rows == rows) && (thiz->first.cols > cols)) {
        return 0;
    }

    if (thiz->last.rows < rows) {
        return 0;
    }

    if ((thiz->last.rows == rows) && (thiz->last.cols <= cols)) {
        return 0;
    }

    return 1;
}


static GIterator       g_deque_iterator_next(GIterator *thiz) {
    GDDeque *gthiz = (GDDeque*) thiz->idata.container;
    thiz->idata.cols += 1;
    if (thiz->idata.cols >= gthiz->cols) {
        thiz->idata.rows += 1;
        thiz->idata.cols  = thiz->idata.cols - gthiz->cols;
    }

    return thiz[0];
}

static GIterator       g_deque_iterator_prev(GIterator *thiz) {
    GDDeque *gthiz = (GDDeque*) thiz->idata.container;
    thiz->idata.cols -= 1;
    if (thiz->idata.cols < 0) {
        thiz->idata.rows -= 1;
        thiz->idata.cols  = thiz->idata.cols + gthiz->cols;
    }

    return thiz[0];
}

static GIterator       g_deque_iterator_forward(GIterator *thiz, guint n) {
    GDDeque *gthiz = (GDDeque*) thiz->idata.container;
    int rows = n / gthiz->cols, cols = n % gthiz->cols;
    thiz->idata.rows += rows;
    thiz->idata.cols += cols;
    if (thiz->idata.cols >= gthiz->cols) {
        thiz->idata.rows += 1;
        thiz->idata.cols  = thiz->idata.cols - gthiz->cols;
    }

    return thiz[0];
}

static GIterator       g_deque_iterator_backward(GIterator *thiz, guint n) {
    GDDeque *gthiz = (GDDeque*) thiz->idata.container;
    int rows = n / gthiz->cols, cols = n % gthiz->cols;
    thiz->idata.rows -= rows;
    thiz->idata.cols -= cols;
    if (thiz->idata.cols < 0) {
        thiz->idata.rows -= 1;
        thiz->idata.cols  = thiz->idata.cols + gthiz->cols;
    }

    return thiz[0];
}

static int              g_deque_iterator_equal(GIterator *thiz, GIterator *that) {
    if ((thiz->idata.rows == that->idata.rows) && (thiz->idata.cols == that->idata.cols))
        return 1;
    return 0;
}

static int              g_deque_iterator_unequal(GIterator *thiz, GIterator *that) {
    if ((thiz->idata.rows != that->idata.rows) || (thiz->idata.cols != that->idata.cols))
        return 1;
    return 0;
}

static int              g_deque_iterator_less(GIterator *thiz, GIterator *that) {
    if (thiz->idata.rows < that->idata.rows)
        return 1;
    if ((thiz->idata.rows == that->idata.rows) && (thiz->idata.cols < that->idata.cols))
        return 1;
    return 0;
}

static int              g_deque_iterator_less_equal(GIterator *thiz, GIterator *that) {
    if (thiz->idata.rows < that->idata.rows)
        return 1;
    if ((thiz->idata.rows == that->idata.rows) && (thiz->idata.cols <= that->idata.cols))
        return 1;
    return 0;
}

static int              g_deque_iterator_greater(GIterator *thiz, GIterator *that) {
    if (thiz->idata.rows > that->idata.rows)
        return 1;
    if ((thiz->idata.rows == that->idata.rows) && (thiz->idata.cols > that->idata.cols))
        return 1;
    return 0;
}

static int              g_deque_iterator_greater_equal(GIterator *thiz, GIterator *that) {
    if (thiz->idata.rows > that->idata.rows)
        return 1;
    if ((thiz->idata.rows == that->idata.rows) && (thiz->idata.cols >= that->idata.cols))
        return 1;
    return 0;
}

static GRef         g_deque_iterator_get(GIterator *thiz) {
    return thiz->idata;
}

static GIterator       g_deque_iterator_set(GIterator *thiz, GRef val) {
    thiz->idata = val;
    return thiz[0];
}

static gpointer       g_deque_iterator_data(GIterator *thiz) {
    GDDeque *gthiz = (GDDeque*) thiz->idata.container;
    gpointer gptr = gthiz->mptr[thiz->idata.rows];
    gptr = gptr + thiz->idata.cols * gthiz->typesize;
    return gptr;
}

static  int g_deque_iterator_back_copy(GIterator *from, GIterator *to, guint size, GIterator *last) {
    int cnt = 0;
    for( ; from->less(from, last); from->next(from), to->next(to), ++cnt) {
        memcpy(to->data(to), from->data(from), size);
    }
    return cnt;
}

static  int g_deque_iterator_front_copy(GIterator *from, GIterator *to, guint size, GIterator *last) {
    int cnt = 0;
    for( ; from->gequal(from, last); from->prev(from), to->prev(to), ++cnt) {
        memcpy(to->data(to), from->data(from), size);
    }
    return cnt;
}

static int              g_deque_iterator_size(GIterator *first, GIterator *last) {
    GDDeque *gthiz = (GDDeque*) first->idata.container;
    int size = gthiz->cols - first->idata.cols;

    for (int rows = first->idata.rows + 1; rows < last->idata.rows; ++rows) {
        size += gthiz->cols;
    }

    size += last->idata.cols;
    return size;
}

GIterator g_deque_iterator(gpointer data, guint size, int dir) {
    GIterator        thiz  = {0};
    thiz.idata.container   = data;
    thiz.dir         = dir;
    if (dir > 0) {
        thiz.next  = g_deque_iterator_next;
        thiz.prev  = g_deque_iterator_prev;
        thiz.forward  = g_deque_iterator_forward;
        thiz.backward  = g_deque_iterator_backward;
    } else {
        thiz.next  = g_deque_iterator_prev;
        thiz.prev  = g_deque_iterator_next;
        thiz.forward  = g_deque_iterator_backward;
        thiz.backward  = g_deque_iterator_forward;
    }
    thiz.equal  = g_deque_iterator_equal;
    thiz.unequal  = g_deque_iterator_unequal;
    thiz.less  = g_deque_iterator_less;
    thiz.lequal  = g_deque_iterator_less_equal;
    thiz.greater  = g_deque_iterator_greater;
    thiz.gequal  = g_deque_iterator_greater_equal;
    thiz.get  = g_deque_iterator_get;
    thiz.set  = g_deque_iterator_set;
    thiz.data = g_deque_iterator_data;
    return thiz;
}




static    void      g_deque_clear(GDeque *thiz) {
    GDDeque *gthiz = (GDDeque*) thiz;
    gthiz->first.rows = 1;
    gthiz->first.cols = 0;
    gthiz->last       = gthiz->first;
}

static    void      g_deque_free(GDeque *thiz) {
    GDDeque *gthiz = (GDDeque*) thiz;
    gpointer gptr = NULL;
    for (guint i = 0; i < gthiz->rows; ++i) {
        gptr = gthiz->mptr[i];
        free(gptr);
    }

    free(gthiz->mptr);
    gthiz->rows = 0;
    gthiz->mptr = NULL;
    free(gthiz);
}

static    guint     g_deque_typesize(GDeque *thiz) {
    GDDeque   *gthiz = (GDDeque*) thiz;
    return gthiz->typesize;
}

static    guint     g_deque_size(GDeque *thiz) {
    GDDeque   *gthiz = (GDDeque*) thiz;
    guint      row   = gthiz->first.rows;
    guint      size  = (gthiz->cols - gthiz->first.cols);
    if (gthiz->first.rows == gthiz->last.rows) {
        size = gthiz->last.cols - gthiz->first.cols;
        return size;
    }

    for (row += 1; row < gthiz->last.rows; row += 1) {
        size += gthiz->cols;
    }

    size += gthiz->last.cols;
    return size;
}

static    guint     g_deque_empty(GDeque *thiz) {
    if (g_deque_size(thiz) <= 0)
        return 1;
    return 0;
}



static    gpointer  g_deque_front(GDeque *thiz) {
    GDDeque *gthiz = (GDDeque*) thiz;
    gint rows = gthiz->first.rows, cols = gthiz->first.cols;
    gpointer gptr = gthiz->mptr[rows];
    gptr += cols * gthiz->typesize;
    return gptr;
}

static    gpointer  g_deque_back(GDeque *thiz) {
    GDDeque *gthiz = (GDDeque*) thiz;
    gint rows = gthiz->last.rows, cols = gthiz->last.cols;
    cols -= 1;
    if (cols < 0) {
        rows -= 1;
        cols  = gthiz->cols + cols;
    }

    gpointer gptr = gthiz->mptr[rows];
    gptr += cols * gthiz->typesize;
    return gptr;
}

static    gpointer  g_deque_at(GDeque *thiz, guint index) {
    GDDeque *gthiz = (GDDeque*) thiz;
    gint rows = gthiz->first.rows + index / gthiz->cols, cols = gthiz->first.cols + index % gthiz->cols;
    if (cols >= gthiz->cols) {
        rows += 1;
        cols  = cols - gthiz->cols;
    }

    gpointer gptr = gthiz->mptr[rows];
    gptr += cols * gthiz->typesize;
    return gptr;
}




static    GIterator  g_deque_begin(GDeque *thiz) {
    GDDeque *gthiz = (GDDeque*) thiz;
    GIterator  iterator = g_deque_iterator(gthiz, gthiz->typesize, 1);
    iterator.idata.rows = gthiz->first.rows;
    iterator.idata.cols = gthiz->first.cols;
    return iterator;
}

static    GIterator  g_deque_end(GDeque *thiz) {
    GDDeque *gthiz = (GDDeque*) thiz;
    GIterator  iterator = g_deque_iterator(gthiz, gthiz->typesize, 1);
    iterator.idata.rows = gthiz->last.rows;
    iterator.idata.cols = gthiz->last.cols;
    return iterator;
}

static    GIterator  g_deque_rbegin(GDeque *thiz){
    GDDeque *gthiz = (GDDeque*) thiz;
    GIterator  iterator = g_deque_iterator(gthiz, gthiz->typesize, 0);
    iterator.idata.rows = gthiz->last.rows;
    iterator.idata.cols = gthiz->last.cols;
    iterator.idata.cols -= 1;
    if (iterator.idata.cols < 0) {
        iterator.idata.rows -= 1;
        iterator.idata.cols  = iterator.idata.cols + gthiz->cols;
    }
    return iterator;
}

static    GIterator  g_deque_rend(GDeque *thiz){
    GDDeque *gthiz = (GDDeque*) thiz;
    GIterator  iterator = g_deque_iterator(gthiz, gthiz->typesize, 0);
    iterator.idata.rows = gthiz->first.rows;
    iterator.idata.cols = gthiz->first.cols;
    iterator.idata.cols -= 1;
    if (iterator.idata.cols < 0) {
        iterator.idata.rows -= 1;
        iterator.idata.cols  = iterator.idata.cols + gthiz->cols;
    }
    return iterator;
}



static    void      g_deque_push_back(GDeque *thiz, gconstpointer val){
    GDDeque *gthiz = (GDDeque*) thiz;
    GIterator end = g_deque_end(thiz);
    thiz->fill(thiz, end, 1, val);
}

static    void      g_deque_push_front(GDeque *thiz, gconstpointer val){
    GDDeque *gthiz = (GDDeque*) thiz;
    GIterator begin = g_deque_begin(thiz);
    thiz->fill(thiz, begin, 1, val);
}

static    void      g_deque_pop_back(GDeque *thiz){
    GDDeque *gthiz = (GDDeque*) thiz;
    if (g_deque_empty(thiz)) {
        return;
    }

    gthiz->last.cols -= 1;
    if (gthiz->last.cols < 0) {
        gthiz->last.rows -= 1;
        gthiz->last.cols  = gthiz->last.cols + gthiz->cols;
    }
}

static    void      g_deque_pop_front(GDeque *thiz){
    GDDeque *gthiz = (GDDeque*) thiz;
    if (g_deque_empty(thiz)) {
        return;
    }

    gthiz->first.cols += 1;
    if (gthiz->first.cols >= gthiz->cols) {
        gthiz->first.rows += 1;
        gthiz->first.cols  = gthiz->first.cols - gthiz->cols;
    }
}

static    GIterator  g_deque_erase(GDeque *thiz, GIterator first, GIterator last){
    GDDeque *gthiz  = (GDDeque*) thiz;
    GIterator begin = thiz->begin(thiz);
    GIterator end   = thiz->end(thiz);
    if (first.greater(&first, &last)) {
        return first;
    }

    if (begin.greater(&begin, &first))
        first = begin;

    if (last.greater(&last, &end))
        last = end;

    begin = first;
    g_deque_iterator_back_copy(&last, &first, gthiz->typesize, &end);

    gthiz->last.rows = first.idata.rows;
    gthiz->last.cols = first.idata.cols;
    return begin;
}

static    GIterator  g_deque_remove(GDeque *thiz, GIterator position){
    GIterator first = position, last = position;
    last.next(&last);
    return g_deque_erase(thiz, first, last);
}

static    void      g_deque_assign(GDeque *thiz, GIterator first, GIterator last){
    GDDeque *gthiz = (GDDeque*) thiz;
    int size = g_deque_iterator_size(&first, &last);
    int rows = (size + gthiz->cols - 1) / gthiz->cols + 2;
    gthiz->first.rows = 1;
    gthiz->first.cols = 0;
    gthiz->last = gthiz->first;

    if (gthiz->rows < rows ) {
        gpointer *mptr = malloc(rows * sizeof(gpointer));
        memcpy(mptr, gthiz->mptr, gthiz->rows * sizeof(gpointer));
        free(gthiz->mptr);
        gthiz->mptr = mptr;
        for (int i = gthiz->rows; i < rows; ++i) {
            gthiz->mptr[i] = malloc(gthiz->cols * gthiz->typesize);
        }
        gthiz->rows = rows;
    }

    GIterator begin = g_deque_begin(thiz);

    g_deque_iterator_back_copy(&first, &begin, gthiz->typesize, &last);
    gthiz->last.rows = begin.idata.rows;
    gthiz->last.cols = begin.idata.cols;
}

static    void      g_deque_fill(GDeque *thiz, GIterator position, guint n, gconstpointer val){
    GDDeque *gthiz = (GDDeque*) thiz;
    gint nocopy = 1;
    if (g_deque_in(gthiz, position.idata.rows, position.idata.cols) < 1) {
        GIterator end = g_deque_end(thiz);
        if (position.equal(&position, &end) < 1) {
            return;
        }

        nocopy = 0;
    }

    gint rows = n / gthiz->cols, cols = n % gthiz->cols;
    gint lrows = rows + position.idata.rows, lcols = cols + position.idata.cols;
    if (lcols >= gthiz->cols) {
        lrows += 1;
        lcols  = lcols - gthiz->cols;
    }
    lrows += 1;

    if (gthiz->rows < lrows) {
        gpointer *mptr = malloc(lrows * sizeof(gpointer));
        memcpy(mptr, gthiz->mptr, gthiz->rows * sizeof(gpointer));
        free(gthiz->mptr);
        gthiz->mptr = mptr;
        for (int i = gthiz->rows; i < lrows; ++i) {
            gthiz->mptr[i] = malloc(gthiz->cols * gthiz->typesize);
        }
        gthiz->rows = lrows;
    }

    if (nocopy == 1) {
        GIterator from = g_deque_end(thiz);
        from.prev(&from);
        GIterator to = from;
        to.forward(&to, n);
        g_deque_iterator_front_copy(&from, &to, gthiz->typesize, &position);
    }

    GIterator *first = &position;
    for (gint i = 0; i < n; ++i, first->next(first)) {
        memcpy(first->data(first), val, gthiz->typesize);
    }

    gthiz->last.rows += rows;
    gthiz->last.cols += cols;
    if (gthiz->last.cols >= gthiz->cols) {
        gthiz->last.rows += 1;
        gthiz->last.cols -= gthiz->cols;
    }
}

static    void      g_deque_insert(GDeque *thiz, GIterator position, GIterator first, GIterator last){
    GDDeque *gthiz = (GDDeque*) thiz;
    int n = g_deque_iterator_size(&first, &last);
    gint nocopy = 1;
    if (g_deque_in(gthiz, position.idata.rows, position.idata.cols) < 1) {
        GIterator end = g_deque_end(thiz);
        if (position.equal(&position, &end) < 1) {
            return;
        }

        nocopy = 0;
    }

    gint rows = n / gthiz->cols, cols = n % gthiz->cols;
    gint lrows = rows + position.idata.rows, lcols = cols + position.idata.cols;
    if (lcols >= gthiz->cols) {
        lrows += 1;
        lcols  = lcols - gthiz->cols;
    }
    lrows += 1;

    if (gthiz->rows < lrows) {
        gpointer *mptr = malloc(lrows * sizeof(gpointer));
        memcpy(mptr, gthiz->mptr, gthiz->rows * sizeof(gpointer));
        free(gthiz->mptr);
        gthiz->mptr = mptr;
        for (int i = gthiz->rows; i < lrows; ++i) {
            gthiz->mptr[i] = malloc(gthiz->cols * gthiz->typesize);
        }
        gthiz->rows = lrows;
    }

    if (nocopy == 1) {
        GIterator from = g_deque_end(thiz);
        from.prev(&from);
        GIterator to = from;
        to.forward(&to, n);
        g_deque_iterator_front_copy(&from, &to, gthiz->typesize, &position);
    }

    GIterator to = position;
    g_deque_iterator_back_copy(&first, &to, gthiz->typesize, &last);

    gthiz->last.rows += rows;
    gthiz->last.cols += cols;
    if (gthiz->last.cols >= gthiz->cols) {
        gthiz->last.rows += 1;
        gthiz->last.cols -= gthiz->cols;
    }
}

static    void      g_deque_swap(GDeque *thiz, GDeque *that){
    GDDeque *gthiz = (GDDeque*) thiz;
    GDDeque *gthat = (GDDeque*) that;

    gpointer  *mptr= gthiz->mptr;
    gthiz->mptr    = gthat->mptr;
    gthat->mptr    = mptr;

    gint val       = gthiz->rows;
    gthiz->rows    = gthat->rows;
    gthat->rows    = val;

    val            = gthiz->cols;
    gthiz->cols    = gthat->cols;
    gthat->cols    = val;

    guint typesize     = gthiz->typesize;
    gthiz->typesize    = gthat->typesize;
    gthat->typesize    = typesize;

    gint  rows     = gthiz->first.rows;
    gint  cols     = gthiz->first.cols;
    gthiz->first   = gthat->first;
    gthat->first.rows = rows;
    gthat->first.cols = cols;

    rows           = gthiz->last.rows;
    cols           = gthiz->last.cols;
    gthiz->last    = gthat->last;
    gthat->last.rows = rows;
    gthat->last.cols = cols;
}

static    void      g_deque_resize(GDeque *thiz, guint n, gconstpointer val){
    GDDeque *gthiz = (GDDeque*) thiz;
    guint size = g_deque_size(thiz);
    GIterator last = g_deque_end(thiz);
    if (size >= n) {
        GIterator first = g_deque_begin(thiz);
        first.forward(&first, n);
        g_deque_erase(thiz, first, last);
        return;
    }

    n = n - size;
    g_deque_fill(thiz, last, n, val);
}

GDeque* g_deque_alloc(guint typesize) { //n - count   c - ElementSize
    GDDeque *gthiz = NULL;
    GDeque   *thiz = NULL;
    if (typesize <= 0) {
        return thiz;
    }

    gthiz = malloc(sizeof(GDDeque));
    gthiz->rows  = 4;//
    gthiz->cols  = 8;
    gthiz->mptr   = malloc(gthiz->rows * sizeof(gpointer));
    gthiz->typesize = typesize;
    for (gint i = 0; i < gthiz->rows; ++i) {
        gthiz->mptr[i] = malloc(gthiz->cols * typesize);
    }

    gthiz->first.rows  = 1;
    gthiz->first.cols  = 0;
    gthiz->last        = gthiz->first;

    thiz            = &(gthiz->thiz);
    thiz->free      = g_deque_free;
    thiz->clear     = g_deque_clear;
    thiz->typesize     = g_deque_typesize;

    thiz->size      = g_deque_size;
    thiz->empty     = g_deque_empty;
    thiz->front     = g_deque_front;
    thiz->back      = g_deque_back;
    thiz->at        = g_deque_at;

    thiz->begin     = g_deque_begin;
    thiz->end       = g_deque_end;
    thiz->rbegin    = g_deque_rbegin;
    thiz->rend      = g_deque_rend;

    thiz->push_back    = g_deque_push_back;
    thiz->push_front   = g_deque_push_front;
    thiz->pop_back     = g_deque_pop_back;
    thiz->pop_front    = g_deque_pop_front;
    thiz->erase        = g_deque_erase;
    thiz->remove       = g_deque_remove;

    thiz->assign       = g_deque_assign;
    thiz->fill         = g_deque_fill;
    thiz->insert       = g_deque_insert;
    thiz->swap         = g_deque_swap;
    thiz->resize       = g_deque_resize;
    return thiz;
}
