
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
    guint      rows;// row number
    guint      cols;// col number
    guint      csize;
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
    for (guint i = 0; i < gthiz->rows; ++i) {
        gpointer gptr = gthiz->mptr[i];
        free(gptr);
    }
    free(gthiz->mptr);
    gthiz->rows = 0;
    gthiz->mptr = NULL;
    free(gthiz);
}

static    guint     g_deque_size(GDeque *thiz) {
    GDDeque   *gthiz = (GDDeque*) thiz;
    guint      row = gthiz->first.rows;
    guint      size = (gthiz->cols - gthiz->first.cols);
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

static    gpointer  g_deque_begin(GDeque *thiz) {
    GDDeque *gthiz = (GDDeque*) thiz;
    gpointer gptr = gthiz->mptr[gthiz->first.rows];
    gptr = gptr + gthiz->first.cols * gthiz->csize;
    return gptr;
}

static    gpointer  g_deque_end(GDeque *thiz) {
    GDDeque *gthiz = (GDDeque*) thiz;
    gpointer gptr = gthiz->mptr[gthiz->last.rows];
    gptr = gptr + gthiz->last.cols * gthiz->csize;
    return gptr;
}

static    gpointer  g_deque_backward(GDeque *thiz, gpointer position, gint n) {
    GDDeque *gthiz = (GDDeque*) thiz;
    guint rows = 0, cols = 0;
    for (rows = 0; rows < gthiz->rows; ++rows) {
        gpointer  first = gthiz->mptr[rows];
        gpointer  last  = first + gthiz->cols * gthiz->csize;
        if (position < first || position >= last) {
                continue;
        }
        rows = rows + n / gthiz->cols;
        cols = gthiz->first.cols + n % gthiz->cols;
        if (cols >= gthiz->cols) {
            rows += 1;
            cols  = cols - gthiz->cols;
        }

        if (rows > gthiz->last.rows)
            return NULL;
        if (rows == gthiz->last.rows) {
            if (cols >= gthiz->last.cols)
                return NULL;
        }

        gpointer gptr = gthiz->mptr[rows];
        gptr = gptr + cols * gthiz->csize;
        return gptr;
    }
    return NULL;
}

static    gpointer  g_deque_front(GDeque *thiz) {
    GDDeque *gthiz = (GDDeque*) thiz;
    gpointer gptr = gthiz->mptr[gthiz->first.rows];
    gptr = gptr + gthiz->first.cols * gthiz->csize;
    return gptr;
}

static    gpointer  g_deque_back(GDeque *thiz) {
    GDDeque *gthiz = (GDDeque*) thiz;
    guint rows = gthiz->last.rows;
    guint cols = gthiz->last.cols;
    if (cols <= 0) {
       if (gthiz->last.rows <= gthiz->first.rows)
            return NULL;
       rows = rows - 1;
       cols = gthiz->cols - 1;
    } else {
       cols = cols - 1;
    }

    gpointer gptr = gthiz->mptr[rows];
    gptr = gptr + cols * gthiz->csize;
    return gptr;
}

static    gpointer  g_deque_at(GDeque *thiz, guint index) {
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
    gptr = gptr + cols * gthiz->csize;
    return gptr;
}

static    gpointer  g_deque_rbegin(GDeque *thiz){
    GDDeque *gthiz = (GDDeque*) thiz;
    guint rows = gthiz->last.rows;
    guint cols = gthiz->last.cols;
    if (cols <= 0) {
       if (gthiz->last.rows <= gthiz->first.rows)
            return NULL;
       rows = rows - 1;
       cols = gthiz->cols - 1;
    } else {
       cols = cols - 1;
    }

    gpointer gptr = gthiz->mptr[rows];
    gptr = gptr + cols * gthiz->csize;
    return gptr;
}

static    gpointer  g_deque_rend(GDeque *thiz){
    GDDeque *gthiz = (GDDeque*) thiz;
    guint rows = gthiz->first.rows;
    guint cols = gthiz->first.cols;
    if (cols <= 0) {
       if (gthiz->first.rows <= 0)
            return NULL;
       rows = rows - 1;
       cols = gthiz->cols - 1;
    } else {
       cols = cols - 1;
    }

    gpointer gptr = gthiz->mptr[rows];
    gptr = gptr + cols * gthiz->csize;
    return gptr;
}

static    gpointer  g_deque_forward(GDeque *thiz, gpointer position, gint n){
    GDDeque *gthiz = (GDDeque*) thiz;
    gint rows = 0, cols = 0;
    for (rows = 0; rows < gthiz->rows; ++rows) {
        gpointer  first = gthiz->mptr[rows];
        gpointer  last  = first + gthiz->cols * gthiz->csize;
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
        gptr = gptr + cols * gthiz->csize;
        return gptr;
    }
    return NULL;
}

static    void      g_deque_assign(GDeque *thiz, gpointer first, gpointer last){
    GDDeque *gthiz = (GDDeque*) thiz;
    guint size = (last - first) / gthiz->csize;
    guint rows = size / gthiz->cols;
    guint cols = size % gthiz->cols;
    if ((rows + 1) > gthiz->rows) {
        gpointer *mptr = malloc(2 * (rows + 1) * sizeof(gpointer));
        memcpy(mptr, gthiz->mptr, gthiz->rows * sizeof(gpointer));
        if (gthiz->mptr)
            free(gthiz->mptr);
        gthiz->mptr = mptr;
        for (; gthiz->rows < 2 * (rows + 1); gthiz->rows += 1) {
            gthiz->mptr[gthiz->rows] = malloc(gthiz->cols * gthiz->csize);
        }
    }
    guint i = 0;
    for (i = 0; i < rows; ++i, first += gthiz->cols * gthiz->csize) {
        memcpy(gthiz->mptr[i], first, gthiz->cols * gthiz->csize);
    }

    gthiz->first.cols  = 0;
    gthiz->first.rows  = 0;

    gthiz->last.cols   = 0;
    gthiz->last.rows   = i;
    if (cols <= 0)
        return;
    memcpy(gthiz->mptr[i], first, cols * gthiz->csize);
    gthiz->last.cols = cols;
}

static    void      g_deque_fill(GDeque *thiz, gpointer position, guint n, gpointer data){
    GDDeque *gthiz = (GDDeque*) thiz;
    guint rows = 0, cols = 0;
    for (rows = 0; rows < gthiz->rows; ++rows) {
        gpointer  first = gthiz->mptr[rows];
        gpointer  last  = first + gthiz->cols * gthiz->csize;
        if (position < first || position >= last) {
                continue;
        }

        cols = (position - first) / gthiz->csize;

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
                gthiz->mptr[gthiz->rows] = malloc(gthiz->cols * gthiz->csize);
            }
        }

        gpointer gptr = gthiz->mptr[rows];
        gptr = gptr + cols;
        guint i = 0;
        for (i = cols; (i < gthiz->cols) && (n > 0); ++i, --n, gptr += gthiz->csize) {
            memcpy(gptr, data, gthiz->csize);
        }

        guint j = rows + 1;
        for (j = rows + 1; j < nrows; ++j) {
            gptr = gthiz->mptr[j];
            for(i = 0;(i < gthiz->cols) && (n > 0); ++i, --n, gptr += gthiz->csize) {
                memcpy(gptr, data, gthiz->csize);
            }
        }

        gptr = gthiz->mptr[j];
        for (i = 0; (i < ncols) && (n > 0); ++i, --n, gptr += gthiz->csize) {
            memcpy(gptr, data, gthiz->csize);
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

static    void      g_deque_push_back(GDeque *thiz, gpointer data){
    GDDeque *gthiz = (GDDeque*) thiz;

}

static    void      g_deque_push_front(GDeque *thiz, gpointer data){
    GDDeque *gthiz = (GDDeque*) thiz;

}

static    void      g_deque_pop_back(GDeque *thiz){
    GDDeque *gthiz = (GDDeque*) thiz;

}

static    void      g_deque_pop_front(GDeque *thiz){
    GDDeque *gthiz = (GDDeque*) thiz;

}

static    void      g_deque_insert(GDeque *thiz, gpointer position, gpointer first, gpointer last){
    GDDeque *gthiz = (GDDeque*) thiz;

}

static    gpointer  g_deque_erase(GDeque *thiz, gpointer first, gpointer last){
    GDDeque *gthiz = (GDDeque*) thiz;

}

static    gpointer  g_deque_remove(GDeque *thiz, gpointer data){
    GDDeque *gthiz = (GDDeque*) thiz;

}

static    void      g_deque_swap(GDeque *thiz, GDeque *_that){
    GDDeque *gthiz = (GDDeque*) thiz;

}

static    void      g_deque_resize(GDeque *thiz, guint n, gpointer data){
    GDDeque *gthiz = (GDDeque*) thiz;

}


GDeque* g_deque_alloc(guint n, guint c) { //n - count   c - ElementSize
    GDDeque *gthiz = NULL;
    GDeque   *thiz = NULL;
    if (n <= 0 || c <= 0) {
        return thiz;
    }

    gthiz = malloc(sizeof(GDDeque));
    gthiz->mptr   = malloc(n * sizeof(gpointer));
    gthiz->rows  = n;//
    gthiz->cols  = 8;
    gthiz->csize = c;
    for (guint i = 0; i < gthiz->rows; ++i) {
        gthiz->mptr[i] = malloc(gthiz->cols * c);
    }

    gthiz->first.rows  = 0;
    gthiz->first.cols  = 0;
    gthiz->last        = gthiz->first;

    thiz            = &(gthiz->thiz);
    thiz->free      = g_deque_free;
    thiz->clear     = g_deque_clear;
    thiz->size      = g_deque_size;
    thiz->empty     = g_deque_empty;
    thiz->begin     = g_deque_begin;
    thiz->end       = g_deque_end;
    thiz->backward  = g_deque_backward;
    thiz->front     = g_deque_front;
    thiz->back      = g_deque_back;
    thiz->at        = g_deque_at;
    thiz->rbegin    = g_deque_rbegin;
    thiz->rend      = g_deque_rend;
    thiz->forward   = g_deque_forward;
    thiz->assign    = g_deque_assign;
    thiz->fill      = g_deque_fill;
    thiz->push_back    = g_deque_push_back;
    thiz->push_front   = g_deque_push_front;
    thiz->pop_back     = g_deque_pop_back;
    thiz->pop_front    = g_deque_pop_front;
    thiz->insert       = g_deque_insert;
    thiz->erase        = g_deque_erase;
    thiz->remove       = g_deque_remove;
    thiz->swap         = g_deque_swap;
    thiz->resize       = g_deque_resize;
    return thiz;
}
