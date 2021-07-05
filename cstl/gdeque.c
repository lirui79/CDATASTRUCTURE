
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
    guint      size;
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

static int              g_deque_iterator_not_equal(GIterator *thiz, GIterator *that) {
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

static GType       g_deque_iterator_data(GIterator *thiz) {
    GDDeque *gthiz = (GDDeque*) thiz->idata.container;
    gpointer gptr = NULL;
    if (g_deque_in(gthiz, thiz->idata.rows, thiz->idata.cols)) {
        gptr = gthiz->mptr[thiz->idata.rows];
        gptr = gptr + thiz->idata.cols * gthiz->size;
    } else {
        gptr = gthiz->mptr[gthiz->last.rows];
        gptr = gptr + gthiz->last.cols * gthiz->size;
    }

    return g_default_type(gptr, gthiz->size);
}

static  int g_deque_iterator_back_copy(GIterator *from, GIterator *to, guint size, GIterator *last) {
    int cnt = 0;
    for( ; from->less(from, last); from->next(from), to->next(to), ++cnt) {
        memcpy(to->data(to).data, from->data(from).data, size);
    }
    return cnt;
}

static  int g_deque_iterator_front_copy(GIterator *from, GIterator *to, guint size, GIterator *last) {
    int cnt = 0;
    for( ; from->greater_equal(from, last); from->prev(from), to->prev(to), ++cnt) {
        memcpy(to->data(to).data, from->data(from).data, size);
    }
    return cnt;
}

static int              g_deque_iterator_size(GIterator *first, GIterator *last) {
    GDDeque *gthiz = (GDDeque*) first->idata.container;
    int rows = last->idata.rows - first->idata.rows + 1, cols = last->idata.cols - first->idata.cols;
    return (rows * gthiz->cols + cols);
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
    thiz.not_equal  = g_deque_iterator_not_equal;
    thiz.less  = g_deque_iterator_less;
    thiz.less_equal  = g_deque_iterator_less_equal;
    thiz.greater  = g_deque_iterator_greater;
    thiz.greater_equal  = g_deque_iterator_greater_equal;
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



static    GType  g_deque_front(GDeque *thiz) {
    GDDeque *gthiz = (GDDeque*) thiz;
    gint rows = gthiz->first.rows, cols = gthiz->first.cols;
    gpointer gptr = gthiz->mptr[rows];
    gptr += cols * gthiz->size;
    return g_default_type(gptr, gthiz->size);
}

static    GType  g_deque_back(GDeque *thiz) {
    GDDeque *gthiz = (GDDeque*) thiz;
    gint rows = gthiz->last.rows, cols = gthiz->last.cols;
    cols -= 1;
    if (cols < 0) {
        rows -= 1;
        cols  = gthiz->cols + cols;
    }

    gpointer gptr = gthiz->mptr[rows];
    gptr += cols * gthiz->size;
    return g_default_type(gptr, gthiz->size);
}

static    GType  g_deque_at(GDeque *thiz, guint index) {
    GDDeque *gthiz = (GDDeque*) thiz;
    gint rows = gthiz->first.rows + index / gthiz->cols, cols = gthiz->first.cols + index % gthiz->cols;
    if (cols >= gthiz->cols) {
        rows += 1;
        cols  = cols - gthiz->cols;
    }

    gpointer gptr = gthiz->mptr[rows];
    gptr += cols * gthiz->size;
    return g_default_type(gptr, gthiz->size);
}




static    GIterator  g_deque_begin(GDeque *thiz) {
    GDDeque *gthiz = (GDDeque*) thiz;
    GIterator  iterator = g_deque_iterator(gthiz, gthiz->size, 1);
    iterator.idata.rows = gthiz->first.rows;
    iterator.idata.cols = gthiz->first.cols;
    return iterator;
}

static    GIterator  g_deque_end(GDeque *thiz) {
    GDDeque *gthiz = (GDDeque*) thiz;
    GIterator  iterator = g_deque_iterator(gthiz, gthiz->size, 1);
    iterator.idata.rows = gthiz->last.rows;
    iterator.idata.cols = gthiz->last.cols;
    return iterator;
}

static    GIterator  g_deque_rbegin(GDeque *thiz){
    GDDeque *gthiz = (GDDeque*) thiz;
    GIterator  iterator = g_deque_iterator(gthiz, gthiz->size, 0);
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
    GIterator  iterator = g_deque_iterator(gthiz, gthiz->size, 0);
    iterator.idata.rows = gthiz->first.rows;
    iterator.idata.cols = gthiz->first.cols;
    iterator.idata.cols -= 1;
    if (iterator.idata.cols < 0) {
        iterator.idata.rows -= 1;
        iterator.idata.cols  = iterator.idata.cols + gthiz->cols;
    }
    return iterator;
}



static    void      g_deque_push_back(GDeque *thiz, GType val){
    GDDeque *gthiz = (GDDeque*) thiz;
    GIterator end = g_deque_end(thiz);
    thiz->fill(thiz, end, 1, val);
}

static    void      g_deque_push_front(GDeque *thiz, GType val){
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
    if (first.greater(&first, &last) <= 0) {
        return first;
    }

    if (begin.greater(&begin, &first))
        first = begin;

    if (last.greater(&last, &end))
        last = end;

    begin = first;
    g_deque_iterator_back_copy(&last, &first, gthiz->size, &end);

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
            gthiz->mptr[i] = malloc(gthiz->cols * gthiz->size);
        }
        gthiz->rows = rows;
    }

    g_deque_iterator_back_copy(&first, &(gthiz->last), gthiz->size, &last);
}

static    void      g_deque_fill(GDeque *thiz, GIterator position, guint n, GType val){
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
            gthiz->mptr[i] = malloc(gthiz->cols * gthiz->size);
        }
        gthiz->rows = lrows;
    }

    if (nocopy == 1) {
        GIterator from = g_deque_end(thiz);
        from.prev(&from);
        GIterator to = from;
        to.forward(&to, n);
        g_deque_iterator_front_copy(&from, &to, gthiz->size, &position);
    }

    GIterator *first = &position;
    for (gint i = 0; i < n; ++i, first->next(first)) {
        memcpy(first->data(first).data, val.data, val.size);
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
            gthiz->mptr[i] = malloc(gthiz->cols * gthiz->size);
        }
        gthiz->rows = lrows;
    }

    if (nocopy == 1) {
        GIterator from = g_deque_end(thiz);
        from.prev(&from);
        GIterator to = from;
        to.forward(&to, n);
        g_deque_iterator_front_copy(&from, &to, gthiz->size, &position);
    }

    GIterator to = position;
    g_deque_iterator_back_copy(&first, &to, gthiz->size, &last);

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

    guint size     = gthiz->size;
    gthiz->size    = gthat->size;
    gthat->size    = size;

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

static    void      g_deque_resize(GDeque *thiz, guint n, GType val){
    GDDeque *gthiz = (GDDeque*) thiz;
    guint size = g_deque_size(thiz);
    GIterator end = g_deque_end(thiz);
    if (size >= n) {
        return;
    }

    n = n - size;
    g_deque_fill(thiz, end, n, val);
}

GDeque* g_deque_alloc(guint size) { //n - count   c - ElementSize
    GDDeque *gthiz = NULL;
    GDeque   *thiz = NULL;
    if (size <= 0) {
        return thiz;
    }

    gthiz = malloc(sizeof(GDDeque));
    gthiz->rows  = 4;//
    gthiz->cols  = 8;
    gthiz->mptr   = malloc(gthiz->rows * sizeof(gpointer));
    gthiz->size = size;
    for (gint i = 0; i < gthiz->rows; ++i) {
        gthiz->mptr[i] = malloc(gthiz->cols * size);
    }

    gthiz->first.rows  = 1;
    gthiz->first.cols  = 0;
    gthiz->last        = gthiz->first;

    thiz            = &(gthiz->thiz);
    thiz->free      = g_deque_free;
    thiz->clear     = g_deque_clear;

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
