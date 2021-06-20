
#include "gdeque.h"
#include "gvector.h"
#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>

typedef struct _GDDeque  GDDeque;

/*************************************/
/*
map   |  |  |
*/


struct _GDDeque {
    GDeque     _this;
    gpointer  *mptr;
    guint      rows;//map_ptr size
    guint      cols;// one row ptr size
    guint      csize;
    struct {
        gulong   cur; // one row idx
        gulong   node;// rows idx
    } start, finish;
};

static    void      g_deque_clear(GDeque *_this) {
    GDDeque *gthis = (GDDeque*) _this;
    gthis->start.node  = 0;
    gthis->start.cur   = 0;
    gthis->finish      = gthis->start;
}

static    void      g_deque_free(GDeque *_this) {
    GDDeque *gthis = (GDDeque*) _this;
    for (guint i = 0; i < gthis->rows; ++i) {
        gpointer gptr = gthis->mptr[i];
        free(gptr);
    }
    free(gthis->mptr);
    gthis->rows = 0;
    gthis->mptr = NULL;
    free(gthis);
}

static    guint     g_deque_size(GDeque *_this) {
    GDDeque   *gthis = (GDDeque*) _this;
    gulong      node = gthis->start.node;
    guint  size = (gthis->cols -gthis->start.cur);
    for (node += 1; node < gthis->finish.node; node += 1) {
        size += gthis->cols;
    }

    size += gthis->finish.cur;
    return size;
}

static    guint     g_deque_empty(GDeque *_this) {
    if (g_deque_size(_this) <= 0)
        return 1;
    return 0;
}

static    gpointer  g_deque_begin(GDeque *_this) {
    GDDeque *gthis = (GDDeque*) _this;
    gpointer gptr = gthis->mptr[gthis->start.node];
    gptr = gptr + gthis->start.cur * gthis->csize;
    return gptr;
}

static    gpointer  g_deque_end(GDeque *_this) {
    GDDeque *gthis = (GDDeque*) _this;
    gpointer gptr = gthis->mptr[gthis->finish.node];
    gptr = gptr + gthis->finish.cur * gthis->csize;
    return gptr;
}

static    gpointer  g_deque_backward(GDeque *_this, gpointer position, gint n) {
    GDDeque *gthis = (GDDeque*) _this;
    guint rows = 0, cols = 0;
    for (rows = 0; rows < gthis->rows; ++rows) {
        gpointer  first = gthis->mptr[rows];
        gpointer  last  = first + gthis->cols * gthis->csize;
        if (position < first || position >= last) {
                continue;
        }
        rows = rows + n / gthis->cols;
        cols = gthis->start.cur + n % gthis->cols;
        if (cols >= gthis->cols) {
            rows += 1;
            cols  = cols - gthis->cols;
        }

        if (rows > gthis->finish.node)
            return NULL;
        if (rows == gthis->finish.node) {
            if (cols >= gthis->finish.cur)
                return NULL;
        }

        gpointer gptr = gthis->mptr[rows];
        gptr = gptr + cols * gthis->csize;
        return gptr;
    }
    return NULL;
}

static    gpointer  g_deque_front(GDeque *_this) {
    GDDeque *gthis = (GDDeque*) _this;
    gpointer gptr = gthis->mptr[gthis->start.node];
    gptr = gptr + gthis->start.cur * gthis->csize;
    return gptr;
}

static    gpointer  g_deque_back(GDeque *_this) {
    GDDeque *gthis = (GDDeque*) _this;
    guint rows = gthis->finish.node;
    guint cols = gthis->finish.cur;
    if (cols <= 0) {
       if (gthis->finish.node <= gthis->start.node)
            return NULL;
       rows = rows - 1;
       cols = gthis->cols - 1;
    } else {
       cols = cols - 1;
    }

    gpointer gptr = gthis->mptr[rows];
    gptr = gptr + cols * gthis->csize;
    return gptr;
}

static    gpointer  g_deque_at(GDeque *_this, guint index) {
    GDDeque *gthis = (GDDeque*) _this;
    guint rows = index / gthis->cols;
    guint cols = index % gthis->cols;
    cols = cols + gthis->start.cur;
    if (cols >= gthis->cols) {
        rows += 1;
        cols = cols - gthis->cols;
    }

    rows = rows + gthis->start.node;
    if (rows > gthis->finish.node)
        return NULL;
    if (rows == gthis->finish.node) {
        if (cols >= gthis->finish.cur)
            return NULL;
    }

    gpointer gptr = gthis->mptr[rows];
    gptr = gptr + cols * gthis->csize;
    return gptr;
}

static    gpointer  g_deque_rbegin(GDeque *_this){
    GDDeque *gthis = (GDDeque*) _this;
    guint rows = gthis->finish.node;
    guint cols = gthis->finish.cur;
    if (cols <= 0) {
       if (gthis->finish.node <= gthis->start.node)
            return NULL;
       rows = rows - 1;
       cols = gthis->cols - 1;
    } else {
       cols = cols - 1;
    }

    gpointer gptr = gthis->mptr[rows];
    gptr = gptr + cols * gthis->csize;
    return gptr;
}

static    gpointer  g_deque_rend(GDeque *_this){
    GDDeque *gthis = (GDDeque*) _this;
    guint rows = gthis->start.node;
    guint cols = gthis->start.cur;
    if (cols <= 0) {
       if (gthis->start.node <= 0)
            return NULL;
       rows = rows - 1;
       cols = gthis->cols - 1;
    } else {
       cols = cols - 1;
    }

    gpointer gptr = gthis->mptr[rows];
    gptr = gptr + cols * gthis->csize;
    return gptr;
}

static    gpointer  g_deque_forward(GDeque *_this, gpointer position, gint n){
    GDDeque *gthis = (GDDeque*) _this;
    gint rows = 0, cols = 0;
    for (rows = 0; rows < gthis->rows; ++rows) {
        gpointer  first = gthis->mptr[rows];
        gpointer  last  = first + gthis->cols * gthis->csize;
        if (position < first || position >= last) {
                continue;
        }
        rows = rows - n / gthis->cols;
        cols = gthis->finish.cur - n % gthis->cols;
        if (cols < 0) {
            rows -= 1;
            cols  = cols + gthis->cols;
        }

        if (rows < gthis->start.node)
            return NULL;

        gpointer gptr = gthis->mptr[rows];
        gptr = gptr + cols * gthis->csize;
        return gptr;
    }
    return NULL;
}

static    void      g_deque_assign(GDeque *_this, gpointer first, gpointer last){
    GDDeque *gthis = (GDDeque*) _this;
    guint size = (last - first) / gthis->csize;
    guint rows = size / gthis->cols;
    guint cols = size % gthis->cols;
    if ((rows + 1) > gthis->rows) {
        gpointer *mptr = malloc(2 * (rows + 1) * sizeof(gpointer));
        memcpy(mptr, gthis->mptr, gthis->rows * sizeof(gpointer));
        if (gthis->mptr)
            free(gthis->mptr);
        gthis->mptr = mptr;
        for (; gthis->rows < 2 * (rows + 1); gthis->rows += 1) {
            gthis->mptr[gthis->rows] = malloc(gthis->cols * gthis->csize);
        }
    }
    guint i = 0;
    for (i = 0; i < rows; ++i, first += gthis->cols * gthis->csize) {
        memcpy(gthis->mptr[i], first, gthis->cols * gthis->csize);
    }

    gthis->start.cur  = 0;
    gthis->start.node = 0;

    gthis->finish.cur  = 0;
    gthis->finish.node = i;
    if (cols <= 0)
        return;
    memcpy(gthis->mptr[i], first, cols * gthis->csize);
    gthis->finish.cur = cols;
}

static    void      g_deque_fill(GDeque *_this, gpointer position, guint n, gpointer data){
    GDDeque *gthis = (GDDeque*) _this;
    guint rows = 0, cols = 0;
    for (rows = 0; rows < gthis->rows; ++rows) {
        gpointer  first = gthis->mptr[rows];
        gpointer  last  = first + gthis->cols * gthis->csize;
        if (position < first || position >= last) {
                continue;
        }

        cols = (position - first) / gthis->csize;

        guint nrows = n / gthis->cols;
        guint ncols = n % gthis->cols;
        ncols = cols + ncols;
        if (ncols >= gthis->cols) {
            nrows = nrows + 1;
            ncols = ncols - gthis->cols;
        }

        nrows = nrows + rows;
        if (nrows >= gthis->rows) {
            gpointer *mptr = malloc(2 * nrows * sizeof(gpointer));
            memcpy(mptr, gthis->mptr, gthis->rows * sizeof(gpointer));
            if (gthis->mptr)
                free(gthis->mptr);
            gthis->mptr = mptr;
            for (; gthis->rows < 2 * nrows; gthis->rows += 1) {
                gthis->mptr[gthis->rows] = malloc(gthis->cols * gthis->csize);
            }
        }

        gpointer gptr = gthis->mptr[rows];
        gptr = gptr + cols;
        guint i = 0;
        for (i = cols; (i < gthis->cols) && (n > 0); ++i, --n, gptr += gthis->csize) {
            memcpy(gptr, data, gthis->csize);
        }

        guint j = rows + 1;
        for (j = rows + 1; j < nrows; ++j) {
            gptr = gthis->mptr[j];
            for(i = 0;(i < gthis->cols) && (n > 0); ++i, --n, gptr += gthis->csize) {
                memcpy(gptr, data, gthis->csize);
            }
        }

        gptr = gthis->mptr[j];
        for (i = 0; (i < ncols) && (n > 0); ++i, --n, gptr += gthis->csize) {
            memcpy(gptr, data, gthis->csize);
        }

        if (gthis->start.node > rows) {
            gthis->start.node = rows;
        }

        if (gthis->start.cur > cols) {
            gthis->start.cur = cols;
        }

        if (gthis->finish.node < nrows) {
            gthis->finish.node = nrows;
        }

        if (gthis->finish.cur < ncols) {
            gthis->finish.cur = ncols;
        }
    }
}

static    void      g_deque_push_back(GDeque *_this, gpointer data){

}

static    void      g_deque_push_front(GDeque *_this, gpointer data){

}

static    void      g_deque_pop_back(GDeque *_this){

}

static    void      g_deque_pop_front(GDeque *_this){

}

static    void      g_deque_insert(GDeque *_this, gpointer position, gpointer first, gpointer last){

}

static    gpointer  g_deque_erase(GDeque *_this, gpointer first, gpointer last){

}

static    gpointer  g_deque_remove(GDeque *_this, gpointer data){

}

static    void      g_deque_swap(GDeque *_this, GDeque *_that){

}

static    void      g_deque_resize(GDeque *_this, guint n){

}


GDeque* g_deque_alloc(guint n, guint c) { //n - count   c - ElementSize
    GDDeque *gthis = NULL;
    GDeque   *_this = NULL;
    if (n <= 0 || c <= 0) {
        return _this;
    }

    gthis = malloc(sizeof(GDDeque));
    gthis->mptr   = malloc(n * sizeof(gpointer));
    gthis->rows  = n;//
    gthis->cols  = 8;
    gthis->csize = c;
    for (guint i = 0; i < gthis->rows; ++i) {
        gthis->mptr[i] = malloc(gthis->cols * c);
    }

    gthis->start.node  = 0;
    gthis->start.cur   = 0;
    gthis->finish      = gthis->start;

    _this            = &(gthis->_this);
    _this->free      = g_deque_free;
    _this->clear     = g_deque_clear;
    _this->size      = g_deque_size;
    _this->empty     = g_deque_empty;
    _this->begin     = g_deque_begin;
    _this->end       = g_deque_end;
    _this->backward  = g_deque_backward;
    _this->front     = g_deque_front;
    _this->back      = g_deque_back;
    _this->at        = g_deque_at;
    _this->rbegin    = g_deque_rbegin;
    _this->rend      = g_deque_rend;
    _this->forward   = g_deque_forward;
    _this->assign    = g_deque_assign;
    _this->fill      = g_deque_fill;
    _this->push_back    = g_deque_push_back;
    _this->push_front   = g_deque_push_front;
    _this->pop_back     = g_deque_pop_back;
    _this->pop_front    = g_deque_pop_front;
    _this->insert       = g_deque_insert;
    _this->erase        = g_deque_erase;
    _this->remove       = g_deque_remove;
    _this->swap         = g_deque_swap;
    _this->resize       = g_deque_resize;
    return _this;
}
