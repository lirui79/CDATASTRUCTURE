
#include "gdeque.h"
#include "gvector.h"
#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>


typedef struct _GDEQItem   GDEQItem;

struct _GDEQItem {
    void      (*free)(GDEQItem *_this);  // free _this
    guint     (*size)(GDEQItem *_this);
    gpointer  (*back)(GDEQItem *_this);
    gpointer  (*front)(GDEQItem *_this);
    gpointer  (*begin)(GDEQItem *_this);
    gpointer  (*end)(GDEQItem *_this);
    gpointer  (*backward)(GDEQItem *_this, gpointer position, gint n);
    gpointer  (*rbegin)(GDEQItem *_this);
    gpointer  (*rend)(GDEQItem *_this);
    gpointer  (*forward)(GDEQItem *_this, gpointer position, gint n);
    gpointer  (*at)(GDEQItem *_this, guint index);
    void     (*fill)(GDEQItem *_this, gpointer data);
    gpointer  (*data)(GDEQItem *_this);
    void     (*assign)(GDEQItem *_this, gpointer first, gpointer last);
    gint      (*find)(GDEQItem *_this, gpointer data);

    gpointer   first;
    gpointer   last;
    guint      csize;
};


static void g_dequeitem_free(GDEQItem *_gthis) {  // free _this
    free(_gthis->first);
    _gthis->first = NULL;
    _gthis->last  = NULL;
    free(_gthis);
}

static gpointer g_dequeitem_back(GDEQItem *_gthis) {
    gpointer gptr = _gthis->last - _gthis->csize;
    return gptr;
}

static gpointer g_dequeitem_front(GDEQItem *_gthis) {
    return _gthis->first;
}

static gpointer g_dequeitem_begin(GDEQItem *_gthis) {
    return _gthis->first;
}

static gpointer g_dequeitem_end(GDEQItem *_gthis) {
    return _gthis->last;
}

static gpointer g_dequeitem_backward(GDEQItem *_gthis, gpointer position, gint n) {
    position = position + n * _gthis->csize;
    return position;
}

static gpointer g_dequeitem_rbegin(GDEQItem *_gthis) {
    gpointer gptr = _gthis->last - _gthis->csize;
    return gptr;
}

static gpointer g_dequeitem_rend(GDEQItem *_gthis) {
    gpointer gptr = _gthis->first - _gthis->csize;
    return gptr;
}

static gpointer g_dequeitem_forward(GDEQItem *_gthis, gpointer position, gint n) {
    position = position - n * _gthis->csize;
    return position;
}

static gpointer g_dequeitem_at(GDEQItem *_gthis, guint index) {
    if (index >= _this->size(_this))
        return _gthis->last;
    return (_gthis->first + index * _gthis->csize);
}

static void g_dequeitem_fill(GDEQItem *_gthis, gpointer data) {
    gpointer gptr = NULL;
    for(gptr = _gthis->first; gptr < _gthis->last; gptr += _gthis->csize) {
        memcpy(gptr, data, _gthis->csize);
    }
}

static gint g_dequeitem_find(GDEQItem *_gthis, gpointer data) {
    gint index = -1;
    if (data < _gthis->first || data >= _gthis->last) {
        return index;
    }

    index = (data - _gthis->first) / _gthis->csize;
    return index;
}

static guint  g_dequeitem_size(GDEQItem *_gthis) {
    guint size = _gthis->last - _gthis->first;
    size = size / _gthis->csize;
    return size;
}

static gpointer g_dequeitem_data(GDEQItem *_gthis) {
    return _gthis->first;
}

static void g_dequeitem_assign(GDEQItem *_gthis, gpointer first, gpointer last) {
    if (first == NULL || last == NULL || first >= last) {
        return;
    }

    guint size = _gthis->last - _gthis->first;
    guint newsize = last - first;
    if (newsize > size)
        newsize = size;
    memcpy(_gthis->first, first, newsize);
}

static  GDEQItem* g_dequeitem_alloc(guint n, guint c) { //n - count   c - ElementSize
    GDEQItem *_gthis = NULL;
    if (n <= 0 || c <= 0) {
        return _gthis;
    }

    _gthis = malloc(sizeof(GDEQItem));
    _gthis->first = malloc(n * c);
    _gthis->last  = _gthis->first + n * c;
    _gthis->csize = c;// unit size > 0

    _gthis->free  = g_dequeitem_free;
    _gthis->back  = g_dequeitem_back;
    _gthis->front  = g_dequeitem_front;
    _gthis->begin  = g_dequeitem_begin;
    _gthis->end  = g_dequeitem_end;
    _gthis->backward  = g_dequeitem_backward;
    _gthis->rbegin  = g_dequeitem_rbegin;
    _gthis->rend  = g_dequeitem_rend;
    _gthis->forward  = g_dequeitem_forward;
    _gthis->at  = g_dequeitem_at;
    _gthis->fill  = g_dequeitem_fill;
    _gthis->size  = g_dequeitem_size;
    _gthis->data  = g_dequeitem_data;
    _gthis->assign= g_dequeitem_assign;
    _gthis->find  = g_dequeitem_find;

    return _gthis;
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
    GDeque     _this;
    GDEQItem  *qmap;
    guint      rows;// row number
    guint      cols;// col number
    guint      csize;
    struct {
        guint   rows; // row idx
        guint   cols;//  col idx
    }first, last;
};

static    void      g_deque_clear(GDeque *_this) {
    GDDeque *gthis = (GDDeque*) _this;
    gthis->first.rows = 0;
    gthis->first.cols = 0;
    gthis->last       = gthis->first;
}

static    void      g_deque_free(GDeque *_this) {
    GDDeque *gthis = (GDDeque*) _this;
    GDEQItem **qitem = gthis->qmap->at(gthis->qmap, 0);
    for (guint i = 0; i < gthis->rows; ++i, ++qitem) {
        free(qitem[0]);
    }

    gthis->qmap->free(gthis->qmap);
    gthis->rows = 0;
    gthis->qmap = NULL;
    free(gthis);
}

static    guint     g_deque_size(GDeque *_this) {
    GDDeque   *gthis = (GDDeque*) _this;
    guint      row   = gthis->first.rows;
    guint      size  = (gthis->cols - gthis->first.cols);
    if (gthis->first.rows == gthis->last.rows) {
        size = gthis->last.cols - gthis->first.cols;
        return size;
    }

    for (row += 1; row < gthis->last.rows; row += 1) {
        size += gthis->cols;
    }

    size += gthis->last.cols;
    return size;
}

static    guint     g_deque_empty(GDeque *_this) {
    if (g_deque_size(_this) <= 0)
        return 1;
    return 0;
}

static    gpointer  g_deque_begin(GDeque *_this) {
    GDDeque *gthis   = (GDDeque*) _this;
    gpointer gptr    = NULL;
    GDEQItem **qitem = NULL:
    if (gthis->first.rows > gthis->last.rows) {
        qitem = gthis->qmap->at(gthis->qmap, gthis->rows - 1);
        return qitem[0]->end(qitem[0]);
    }

    if (gthis->first.rows == gthis->last.rows) {
        if (gthis->first.cols >= gthis->last.cols) {
            qitem = gthis->qmap->at(gthis->qmap, gthis->rows - 1);
            return qitem[0]->end(qitem[0]);
        }
    }

    qitem  = gthis->qmap->at(gthis->qmap, gthis->first.rows);
    gptr   = qitem[0]->at(qitem[0], gthis->first.cols);
    return gptr;
}

static    gpointer  g_deque_end(GDeque *_this) {
    GDDeque *gthis = (GDDeque*) _this;
    gpointer gptr    = NULL;
    GDEQItem **qitem = NULL:
    if (gthis->first.rows > gthis->last.rows) {
        qitem = gthis->qmap->at(gthis->qmap, gthis->rows - 1);
        return qitem[0]->end(qitem[0]);
    }

    if (gthis->first.rows == gthis->last.rows) {
        if (gthis->first.cols >= gthis->last.cols) {
            qitem = gthis->qmap->at(gthis->qmap, gthis->rows - 1);
            return qitem[0]->end(qitem[0]);
        }
    }

    qitem = gthis->qmap->at(gthis->qmap, gthis->last.rows);
    gptr   = qitem[0]->at(qitem[0], gthis->last.cols);
    return gptr;
}

static    gpointer  g_deque_backward(GDeque *_this, gpointer position, gint n) {
    GDDeque *gthis = (GDDeque*) _this;
    guint rows = 0, cols = 0;
    gint  index = -1;
    gpointer gptr    = NULL;
    GDEQItem **qitem = gthis->qmap->at(gthis->qmap, rows);
    for (rows = 0; rows < gthis->rows; ++rows, ++qitem) {
        index = qitem[0]->find(qitem[0], position);
        if (index < 0) {
            continue;
        }

        cols = index;
        rows = rows + n / gthis->cols;
        cols = cols + n % gthis->cols;
        if (cols >= gthis->cols) {
            rows += 1;
            cols  = cols - gthis->cols;
        }

        if (rows > gthis->last.rows) {
            qitem = gthis->qmap->at(gthis->qmap, gthis->rows - 1);
            return qitem[0]->end(qitem[0]);
        }

        if (rows == gthis->last.rows) {
            if (cols >= gthis->last.cols) {
                qitem = gthis->qmap->at(gthis->qmap, gthis->rows - 1);
                return qitem[0]->end(qitem[0]);
            }
        }

        qitem = gthis->qmap->at(gthis->qmap, rows);
        gptr = qitem[0]->at(qitem[0], cols);
        return gptr;
    }

    qitem = gthis->qmap->at(gthis->qmap, gthis->rows - 1);
    return qitem[0]->end(qitem[0]);
}

static    gpointer  g_deque_front(GDeque *_this) {
    return _this->begin(_this);
}

static    gpointer  g_deque_back(GDeque *_this) {
    return _this->rbegin(_this);
}

static    gpointer  g_deque_at(GDeque *_this, guint index) {
    GDDeque *gthis = (GDDeque*) _this;
    guint rows = index / gthis->cols;
    guint cols = index % gthis->cols;
    cols = cols + gthis->first.cols;
    if (cols >= gthis->cols) {
        rows += 1;
        cols = cols - gthis->cols;
    }

    rows = rows + gthis->first.rows;
    if (rows > gthis->last.rows)
        return NULL;
    if (rows == gthis->last.rows) {
        if (cols >= gthis->last.cols)
            return NULL;
    }

    gpointer gptr = gthis->mptr[rows];
    gptr = gptr + cols * gthis->csize;
    return gptr;
}

static    gpointer  g_deque_rbegin(GDeque *_this){
    GDDeque *gthis = (GDDeque*) _this;
    gpointer gptr    = NULL;
    GDEQItem **qitem = NULL:
    if (gthis->first.rows > gthis->last.rows) {
        qitem = gthis->qmap->at(gthis->qmap, 0);
        return qitem[0]->rend(qitem[0]);
    }

    if (gthis->first.rows == gthis->last.rows) {
        if (gthis->first.cols >= gthis->last.cols) {
            qitem = gthis->qmap->at(gthis->qmap, 0);
            return qitem[0]->rend(qitem[0]);
        }
    }

    if (gthis->last.cols <= 0) {
        qitem = gthis->qmap->at(gthis->qmap, gthis->last.rows - 1);
        gptr   = qitem[0]->rbegin(qitem[0]);
    } else {
        qitem = gthis->qmap->at(gthis->qmap, gthis->last.rows);
        gptr   = qitem[0]->at(qitem[0], gthis->last.cols - 1);
    }

    return gptr;
}

static    gpointer  g_deque_rend(GDeque *_this){
    GDDeque *gthis = (GDDeque*) _this;
    gpointer gptr    = NULL;
    GDEQItem **qitem = NULL:

    guint rows = gthis->first.rows;
    guint cols = gthis->first.cols;
    if (cols <= 0) {
       if (gthis->first.rows <= 0) {
            qitem = gthis->qmap->at(gthis->qmap, 0);
            return qitem[0]->rend(qitem[0]);
       }

       rows = rows - 1;
       cols = gthis->cols - 1;
    } else {
       cols = cols - 1;
    }

    qitem = gthis->qmap->at(gthis->qmap, rows);
    gptr   = qitem[0]->at(qitem[0], cols);
    return gptr;
}

static    gpointer  g_deque_forward(GDeque *_this, gpointer position, gint n){
    GDDeque *gthis = (GDDeque*) _this;
    guint rows = 0, cols = 0;
    gint  index = -1;
    gpointer gptr    = NULL;
    GDEQItem **qitem = gthis->qmap->at(gthis->qmap, rows);
    for (rows = 0; rows < gthis->rows; ++rows, ++qitem) {
        index = qitem[0]->find(qitem[0], position);
        if (index < 0) {
            continue;
        }

        cols = index;
        rows = rows + n / gthis->cols;
        cols = gthis->first.cols + n % gthis->cols;
        if (cols >= gthis->cols) {
            rows += 1;
            cols  = cols - gthis->cols;
        }

        if (rows > gthis->last.rows) {
            qitem = gthis->qmap->at(gthis->qmap, gthis->rows - 1);
            return qitem[0]->end(qitem[0]);
        }

        if (rows == gthis->last.rows) {
            if (cols >= gthis->last.cols) {
                qitem = gthis->qmap->at(gthis->qmap, gthis->rows - 1);
                return qitem[0]->end(qitem[0]);
            }
        }

        qitem = gthis->qmap->at(gthis->qmap, rows);
        gptr = qitem[0]->at(qitem[0], cols);
        return gptr;
    }

    qitem = gthis->qmap->at(gthis->qmap, gthis->rows - 1);
    return qitem[0]->end(qitem[0]);


    GDDeque *gthis = (GDDeque*) _this;
    gint rows = 0, cols = 0;
    for (rows = 0; rows < gthis->rows; ++rows) {
        gpointer  first = gthis->mptr[rows];
        gpointer  last  = first + gthis->cols * gthis->csize;
        if (position < first || position >= last) {
                continue;
        }
        rows = rows - n / gthis->cols;
        cols = gthis->last.cols - n % gthis->cols;
        if (cols < 0) {
            rows -= 1;
            cols  = cols + gthis->cols;
        }

        if (rows < gthis->first.rows)
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

    gthis->first.cols  = 0;
    gthis->first.rows  = 0;

    gthis->last.cols   = 0;
    gthis->last.rows   = i;
    if (cols <= 0)
        return;
    memcpy(gthis->mptr[i], first, cols * gthis->csize);
    gthis->last.cols = cols;
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

        if (gthis->first.rows > rows) {
            gthis->first.rows = rows;
        }

        if (gthis->first.cols > cols) {
            gthis->first.cols = cols;
        }

        if (gthis->last.rows < nrows) {
            gthis->last.rows = nrows;
        }

        if (gthis->last.cols < ncols) {
            gthis->last.cols = ncols;
        }
    }
}

static    void      g_deque_push_back(GDeque *_this, gpointer data){
    GDDeque *gthis = (GDDeque*) _this;

}

static    void      g_deque_push_front(GDeque *_this, gpointer data){
    GDDeque *gthis = (GDDeque*) _this;

}

static    void      g_deque_pop_back(GDeque *_this){
    GDDeque *gthis = (GDDeque*) _this;

}

static    void      g_deque_pop_front(GDeque *_this){
    GDDeque *gthis = (GDDeque*) _this;

}

static    void      g_deque_insert(GDeque *_this, gpointer position, gpointer first, gpointer last){
    GDDeque *gthis = (GDDeque*) _this;

}

static    gpointer  g_deque_erase(GDeque *_this, gpointer first, gpointer last){
    GDDeque *gthis = (GDDeque*) _this;

}

static    gpointer  g_deque_remove(GDeque *_this, gpointer data){
    GDDeque *gthis = (GDDeque*) _this;

}

static    void      g_deque_swap(GDeque *_this, GDeque *_that){
    GDDeque *gthis = (GDDeque*) _this;

}

static    void      g_deque_resize(GDeque *_this, guint n, gpointer data){
    GDDeque *gthis = (GDDeque*) _this;

}


GDeque* g_deque_alloc(guint n, guint c) { //n - count   c - ElementSize
    GDDeque *gthis   = NULL;
    GDeque   *_this  = NULL;
    GDEQItem **qitem = NULL;
    if (n <= 0 || c <= 0) {
        return _this;
    }

    gthis = malloc(sizeof(GDDeque));
    gthis->qmap  = g_dequeitem_alloc(n, sizeof(GDEQItem *));
    gthis->rows  = n;//
    gthis->cols  = 8;
    gthis->csize = c;
    qitem = (GDEQItem**)gthis->qmap->at(gthis->qmap, 0);
    for (guint i = 0; i < gthis->rows; ++i, ++qitem) {
        qitem[0] = g_dequeitem_alloc(gthis->cols, c);
    }

    gthis->first.rows  = 0;
    gthis->first.cols  = 0;
    gthis->last        = gthis->first;

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
