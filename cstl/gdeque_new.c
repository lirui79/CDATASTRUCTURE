
#include "gdeque.h"
#include "gvector.h"
#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>



static    gpointer  g_deque_backward(GDeque *thiz, gpointer position, gint n) {
    GDDeque *gthiz = (GDDeque*) thiz;
    guint rows = 0, cols = 0;
    gint  index = -1;
    gpointer gptr    = NULL;
    GDEQItem **qitem = gthiz->qmap->at(gthiz->qmap, rows);
    for (rows = 0; rows < gthiz->rows; ++rows, ++qitem) {
        index = qitem[0]->find(qitem[0], position);
        if (index < 0) {
            continue;
        }

        cols = index;
        rows = rows + n / gthiz->cols;
        cols = cols + n % gthiz->cols;
        if (cols >= gthiz->cols) {
            rows += 1;
            cols  = cols - gthiz->cols;
        }

        if (rows > gthiz->last.rows) {
            qitem = gthiz->qmap->at(gthiz->qmap, gthiz->rows - 1);
            return qitem[0]->end(qitem[0]);
        }

        if (rows == gthiz->last.rows) {
            if (cols >= gthiz->last.cols) {
                qitem = gthiz->qmap->at(gthiz->qmap, gthiz->rows - 1);
                return qitem[0]->end(qitem[0]);
            }
        }

        qitem = gthiz->qmap->at(gthiz->qmap, rows);
        gptr = qitem[0]->at(qitem[0], cols);
        return gptr;
    }

    qitem = gthiz->qmap->at(gthiz->qmap, gthiz->rows - 1);
    return qitem[0]->end(qitem[0]);
}


static    gpointer  g_deque_forward(GDeque *thiz, gpointer position, gint n){
    GDDeque *gthiz = (GDDeque*) thiz;
    guint rows = 0, cols = 0;
    gint  index = -1;
    gpointer gptr    = NULL;
    GDEQItem **qitem = gthiz->qmap->at(gthiz->qmap, rows);
    for (rows = 0; rows < gthiz->rows; ++rows, ++qitem) {
        index = qitem[0]->find(qitem[0], position);
        if (index < 0) {
            continue;
        }

        cols = index;
        rows = rows + n / gthiz->cols;
        cols = gthiz->first.cols + n % gthiz->cols;
        if (cols >= gthiz->cols) {
            rows += 1;
            cols  = cols - gthiz->cols;
        }

        if (rows > gthiz->last.rows) {
            qitem = gthiz->qmap->at(gthiz->qmap, gthiz->rows - 1);
            return qitem[0]->end(qitem[0]);
        }

        if (rows == gthiz->last.rows) {
            if (cols >= gthiz->last.cols) {
                qitem = gthiz->qmap->at(gthiz->qmap, gthiz->rows - 1);
                return qitem[0]->end(qitem[0]);
            }
        }

        qitem = gthiz->qmap->at(gthiz->qmap, rows);
        gptr = qitem[0]->at(qitem[0], cols);
        return gptr;
    }

    qitem = gthiz->qmap->at(gthiz->qmap, gthiz->rows - 1);
    return qitem[0]->end(qitem[0]);


    GDDeque *gthiz = (GDDeque*) thiz;
    gint rows = 0, cols = 0;
    for (rows = 0; rows < gthiz->rows; ++rows) {
        gpointer  first = gthiz->mptr[rows];
        gpointer  last  = first + gthiz->cols * gthiz->size;
        if (position < first || position >= last) {
                continue;
        }
        rows = rows - n / gthiz->cols;
        cols = gthiz->last.cols - n % gthiz->cols;
        if (cols < 0) {
            rows -= 1;
            cols  = cols + gthiz->cols;
        }

        if (rows < gthiz->first.rows)
            return NULL;

        gpointer gptr = gthiz->mptr[rows];
        gptr = gptr + cols * gthiz->size;
        return gptr;
    }
    return NULL;
}



typedef struct _GDEQItem   GDEQItem;

struct _GDEQItem {
    void      (*free)(GDEQItem *thiz);  // free thiz

    gpointer  (*at)(GDEQItem *thiz, guint index);
    gpointer  (*begin)(GDEQItem *thiz);
    gpointer  (*end)(GDEQItem *thiz);
    gpointer  (*rbegin)(GDEQItem *thiz);
    gpointer  (*rend)(GDEQItem *thiz);

    gpointer  (*forward)(GDEQItem *thiz, gpointer position, gint n);
    void     (*fill)(GDEQItem *thiz, gpointer data);
    void     (*assign)(GDEQItem *thiz, gpointer first, gpointer last);
    gint      (*find)(GDEQItem *thiz, gpointer data);

    gpointer   first;
    gpointer   last;
    guint      size;
};


static void g_dequeitem_free(GDEQItem *thiz) {  // free thiz
    free(thiz->first);
    thiz->first = NULL;
    thiz->last  = NULL;
    free(thiz);
}

static gpointer g_dequeitem_at(GDEQItem *thiz, guint index) {
    if (index >= thiz->size(thiz))
        return thiz->last;
    return (thiz->first + index * thiz->size);
}

static gpointer g_dequeitem_begin(GDEQItem *thiz) {
    return thiz->first;
}

static gpointer g_dequeitem_end(GDEQItem *thiz) {
    return thiz->last;
}

static gpointer g_dequeitem_rbegin(GDEQItem *thiz) {
    return (thiz->last - thiz->size);
}

static gpointer g_dequeitem_rend(GDEQItem *thiz) {
    return (thiz->first - thiz->size);
}

static gpointer g_dequeitem_forward(GDEQItem *thiz, gpointer position, gint n) {
    return (position + n * thiz->size);
}


static void g_dequeitem_fill(GDEQItem *thiz, gpointer data) {
    gpointer gptr = NULL;
    for(gptr = thiz->first; gptr < thiz->last; gptr += thiz->size) {
        memcpy(gptr, data, thiz->size);
    }
}

static gint g_dequeitem_find(GDEQItem *thiz, gpointer data) {
    gint index = -1;
    if (data < thiz->first || data >= thiz->last) {
        return index;
    }

    index = (data - thiz->first) / thiz->size;
    return index;
}

static void g_dequeitem_assign(GDEQItem *thiz, gpointer first, gpointer last) {
    if (first == NULL || last == NULL || first >= last) {
        return;
    }

    guint size = thiz->last - thiz->first;
    guint newsize = last - first;
    if (newsize > size)
        newsize = size;
    memcpy(thiz->first, first, newsize);
}

static  GDEQItem* g_dequeitem_alloc(guint n, guint c) { //n - count   c - ElementSize
    GDEQItem *thiz = NULL;
    if (n <= 0 || c <= 0) {
        return thiz;
    }

    thiz = malloc(sizeof(GDEQItem));
    thiz->first = malloc(n * c);
    thiz->last  = thiz->first + n * c;
    thiz->size  = c;// unit size > 0

    thiz->free   = g_dequeitem_free;

    thiz->at     = g_dequeitem_at;
    thiz->begin  = g_dequeitem_begin;
    thiz->end    = g_dequeitem_end;
    thiz->rbegin = g_dequeitem_rbegin;
    thiz->rend   = g_dequeitem_rend;

    thiz->forward   = g_dequeitem_forward;
    thiz->fill  = g_dequeitem_fill;
    thiz->assign= g_dequeitem_assign;
    thiz->find  = g_dequeitem_find;

    return thiz;
}



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
    GDEQItem  *qmap;
    guint      rows;// row number
    guint      cols;// col number
    guint      size;
    struct {
        guint   rows; // row idx
        guint   cols;//  col idx
    }first, last;
};

static    void      g_deque_clear(GDeque *thiz) {
    GDDeque *gthiz = (GDDeque*) thiz;
    gthiz->first.rows = 0;
    gthiz->first.cols = 0;
    gthiz->last       = gthiz->first;
}

static    void      g_deque_free(GDeque *thiz) {
    GDDeque *gthiz = (GDDeque*) thiz;
    GDEQItem **qitem = gthiz->qmap->at(gthiz->qmap, 0);
    for (guint i = 0; i < gthiz->rows; ++i, ++qitem) {
        qitem[0]->free(qitem[0]);
    }

    gthiz->qmap->free(gthiz->qmap);
    gthiz->rows = 0;
    gthiz->qmap = NULL;
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
    GType val = {}
    return thiz->begin(thiz);
}

static    GType  g_deque_back(GDeque *thiz) {
    return thiz->rbegin(thiz);
}

static    GType  g_deque_at(GDeque *thiz, guint index) {
    GDDeque *gthiz = (GDDeque*) thiz;
    guint rows = index / gthiz->cols;
    guint cols = index % gthiz->cols;
    cols = cols + gthiz->first.cols;
    if (cols >= gthiz->cols) {
        rows += 1;
        cols = cols - gthiz->cols;
    }

    rows = rows + gthiz->first.rows;
    if (rows > gthiz->last.rows)
        return NULL;
    if (rows == gthiz->last.rows) {
        if (cols >= gthiz->last.cols)
            return NULL;
    }

    gpointer gptr = gthiz->mptr[rows];
    gptr = gptr + cols * gthiz->size;
    return gptr;
}



static    GIterator  g_deque_begin(GDeque *thiz) {
    GDDeque *gthiz   = (GDDeque*) thiz;
    gpointer gptr    = NULL;
    GDEQItem **qitem = NULL:
    if (gthiz->first.rows > gthiz->last.rows) {
        qitem = gthiz->qmap->at(gthiz->qmap, gthiz->rows - 1);
        return qitem[0]->end(qitem[0]);
    }

    if (gthiz->first.rows == gthiz->last.rows) {
        if (gthiz->first.cols >= gthiz->last.cols) {
            qitem = gthiz->qmap->at(gthiz->qmap, gthiz->rows - 1);
            return qitem[0]->end(qitem[0]);
        }
    }

    qitem  = gthiz->qmap->at(gthiz->qmap, gthiz->first.rows);
    gptr   = qitem[0]->at(qitem[0], gthiz->first.cols);
    return gptr;
}

static    GIterator  g_deque_end(GDeque *thiz) {
    GDDeque *gthiz = (GDDeque*) thiz;
    gpointer gptr    = NULL;
    GDEQItem **qitem = NULL:
    if (gthiz->first.rows > gthiz->last.rows) {
        qitem = gthiz->qmap->at(gthiz->qmap, gthiz->rows - 1);
        return qitem[0]->end(qitem[0]);
    }

    if (gthiz->first.rows == gthiz->last.rows) {
        if (gthiz->first.cols >= gthiz->last.cols) {
            qitem = gthiz->qmap->at(gthiz->qmap, gthiz->rows - 1);
            return qitem[0]->end(qitem[0]);
        }
    }

    qitem = gthiz->qmap->at(gthiz->qmap, gthiz->last.rows);
    gptr   = qitem[0]->at(qitem[0], gthiz->last.cols);
    return gptr;
}


static    GIterator  g_deque_rbegin(GDeque *thiz){
    GDDeque *gthiz = (GDDeque*) thiz;
    gpointer gptr    = NULL;
    GDEQItem **qitem = NULL:
    if (gthiz->first.rows > gthiz->last.rows) {
        qitem = gthiz->qmap->at(gthiz->qmap, 0);
        return qitem[0]->rend(qitem[0]);
    }

    if (gthiz->first.rows == gthiz->last.rows) {
        if (gthiz->first.cols >= gthiz->last.cols) {
            qitem = gthiz->qmap->at(gthiz->qmap, 0);
            return qitem[0]->rend(qitem[0]);
        }
    }

    if (gthiz->last.cols <= 0) {
        qitem = gthiz->qmap->at(gthiz->qmap, gthiz->last.rows - 1);
        gptr   = qitem[0]->rbegin(qitem[0]);
    } else {
        qitem = gthiz->qmap->at(gthiz->qmap, gthiz->last.rows);
        gptr   = qitem[0]->at(qitem[0], gthiz->last.cols - 1);
    }

    return gptr;
}

static    GIterator  g_deque_rend(GDeque *thiz){
    GDDeque *gthiz = (GDDeque*) thiz;
    gpointer gptr    = NULL;
    GDEQItem **qitem = NULL:

    guint rows = gthiz->first.rows;
    guint cols = gthiz->first.cols;
    if (cols <= 0) {
       if (gthiz->first.rows <= 0) {
            qitem = gthiz->qmap->at(gthiz->qmap, 0);
            return qitem[0]->rend(qitem[0]);
       }

       rows = rows - 1;
       cols = gthiz->cols - 1;
    } else {
       cols = cols - 1;
    }

    qitem = gthiz->qmap->at(gthiz->qmap, rows);
    gptr   = qitem[0]->at(qitem[0], cols);
    return gptr;
}




static    void      g_deque_push_back(GDeque *thiz, GType val){
    GDDeque *gthiz = (GDDeque*) thiz;

}

static    void      g_deque_push_front(GDeque *thiz, GType val){
    GDDeque *gthiz = (GDDeque*) thiz;

}

static    void      g_deque_pop_back(GDeque *thiz){
    GDDeque *gthiz = (GDDeque*) thiz;

}

static    void      g_deque_pop_front(GDeque *thiz){
    GDDeque *gthiz = (GDDeque*) thiz;

}

static    GIterator  g_deque_erase(GDeque *thiz, GIterator first, GIterator last){
    GDDeque *gthiz = (GDDeque*) thiz;

}

static    GIterator  g_deque_remove(GDeque *thiz, GIterator position){
    GDDeque *gthiz = (GDDeque*) thiz;

}




static    void      g_deque_assign(GDeque *thiz, GIterator first, GIterator last){
    GDDeque *gthiz = (GDDeque*) thiz;
    guint size = (last - first) / gthiz->size;
    guint rows = size / gthiz->cols;
    guint cols = size % gthiz->cols;
    if ((rows + 1) > gthiz->rows) {
        gpointer *mptr = malloc(2 * (rows + 1) * sizeof(gpointer));
        memcpy(mptr, gthiz->mptr, gthiz->rows * sizeof(gpointer));
        if (gthiz->mptr)
            free(gthiz->mptr);
        gthiz->mptr = mptr;
        for (; gthiz->rows < 2 * (rows + 1); gthiz->rows += 1) {
            gthiz->mptr[gthiz->rows] = malloc(gthiz->cols * gthiz->size);
        }
    }
    guint i = 0;
    for (i = 0; i < rows; ++i, first += gthiz->cols * gthiz->size) {
        memcpy(gthiz->mptr[i], first, gthiz->cols * gthiz->size);
    }

    gthiz->first.cols  = 0;
    gthiz->first.rows  = 0;

    gthiz->last.cols   = 0;
    gthiz->last.rows   = i;
    if (cols <= 0)
        return;
    memcpy(gthiz->mptr[i], first, cols * gthiz->size);
    gthiz->last.cols = cols;
}

static    void      g_deque_fill(GDeque *thiz, GIterator position, guint n, GType val){
    GDDeque *gthiz = (GDDeque*) thiz;
    guint rows = 0, cols = 0;
    for (rows = 0; rows < gthiz->rows; ++rows) {
        gpointer  first = gthiz->mptr[rows];
        gpointer  last  = first + gthiz->cols * gthiz->size;
        if (position < first || position >= last) {
                continue;
        }

        cols = (position - first) / gthiz->size;

        guint nrows = n / gthiz->cols;
        guint ncols = n % gthiz->cols;
        ncols = cols + ncols;
        if (ncols >= gthiz->cols) {
            nrows = nrows + 1;
            ncols = ncols - gthiz->cols;
        }

        nrows = nrows + rows;
        if (nrows >= gthiz->rows) {
            gpointer *mptr = malloc(2 * nrows * sizeof(gpointer));
            memcpy(mptr, gthiz->mptr, gthiz->rows * sizeof(gpointer));
            if (gthiz->mptr)
                free(gthiz->mptr);
            gthiz->mptr = mptr;
            for (; gthiz->rows < 2 * nrows; gthiz->rows += 1) {
                gthiz->mptr[gthiz->rows] = malloc(gthiz->cols * gthiz->size);
            }
        }

        gpointer gptr = gthiz->mptr[rows];
        gptr = gptr + cols;
        guint i = 0;
        for (i = cols; (i < gthiz->cols) && (n > 0); ++i, --n, gptr += gthiz->size) {
            memcpy(gptr, data, gthiz->size);
        }

        guint j = rows + 1;
        for (j = rows + 1; j < nrows; ++j) {
            gptr = gthiz->mptr[j];
            for(i = 0;(i < gthiz->cols) && (n > 0); ++i, --n, gptr += gthiz->size) {
                memcpy(gptr, data, gthiz->size);
            }
        }

        gptr = gthiz->mptr[j];
        for (i = 0; (i < ncols) && (n > 0); ++i, --n, gptr += gthiz->size) {
            memcpy(gptr, data, gthiz->size);
        }

        if (gthiz->first.rows > rows) {
            gthiz->first.rows = rows;
        }

        if (gthiz->first.cols > cols) {
            gthiz->first.cols = cols;
        }

        if (gthiz->last.rows < nrows) {
            gthiz->last.rows = nrows;
        }

        if (gthiz->last.cols < ncols) {
            gthiz->last.cols = ncols;
        }
    }
}

static    void      g_deque_insert(GDeque *thiz, GIterator position, GIterator first, GIterator last){
    GDDeque *gthiz = (GDDeque*) thiz;

}


static    void      g_deque_swap(GDeque *thiz, GDeque *_that){
    GDDeque *gthiz = (GDDeque*) thiz;

}

static    void      g_deque_resize(GDeque *thiz, guint n, GType val){
    GDDeque *gthiz = (GDDeque*) thiz;

}


GDeque* g_deque_alloc(guint n, guint c) { //n - count   c - ElementSize
    GDDeque   *gthiz   = NULL;
    GDeque    *thiz  = NULL;
    GDEQItem **qitem = NULL;
    if (n <= 0 || c <= 0) {
        return thiz;
    }

    gthiz = malloc(sizeof(GDDeque));
    gthiz->qmap  = g_dequeitem_alloc(n, sizeof(GDEQItem *));
    gthiz->rows  = n;//
    gthiz->cols  = 8;
    gthiz->size = c;
    qitem = (GDEQItem**)gthiz->qmap->at(gthiz->qmap, 0);
    for (guint i = 0; i < gthiz->rows; ++i, ++qitem) {
        qitem[0] = g_dequeitem_alloc(gthiz->cols, c);
    }

    gthiz->first.rows  = 0;
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
