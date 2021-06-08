
#include "garray.h"
#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>

typedef struct _GDArray  GDArray;

struct _GDArray {
    GArray     _this;
    gpointer   first;
    gpointer   last;
    int        cellsize;
};


static void g_array_free(GArray *_this) {  // free _this
    GDArray *_gthis = (GDArray*)_this;
    free(_gthis->first);
    _gthis->first = NULL;
    _gthis->last  = NULL;
    free(_gthis);
}

static gpointer g_array_back(GArray *_this) {
    GDArray *_gthis = (GDArray*)_this;
    gpointer gptr = _gthis->last - _gthis->cellsize;
    return gptr;
}

static gpointer g_array_front(GArray *_this) {
    GDArray *_gthis = (GDArray*)_this;
    return _gthis->first;
}

static gpointer g_array_begin(GArray *_this) {
    GDArray *_gthis = (GDArray*)_this;
    return _gthis->first;
}

static gpointer g_array_end(GArray *_this) {
    GDArray *_gthis = (GDArray*)_this;
    return _gthis->last;
}

static gpointer g_array_move(GArray *_this, gpointer position, int n) {
    GDArray *_gthis = (GDArray*)_this;
    position = position + n * _gthis->cellsize;
    return position;
}

static gpointer g_array_rbegin(GArray *_this) {
    GDArray *_gthis = (GDArray*)_this;
    gpointer gptr = _gthis->last - _gthis->cellsize;
    return gptr;
}

static gpointer g_array_rend(GArray *_this) {
    GDArray *_gthis = (GDArray*)_this;
    gpointer gptr = _gthis->first - _gthis->cellsize;
    return gptr;
}

static gpointer g_array_forward(GArray *_this, gpointer position, int n) {
    GDArray *_gthis = (GDArray*)_this;
    position = position - n * _gthis->cellsize;
    return position;
}

static gpointer g_array_reverse(GArray *_this) {
    GDArray *_gthis = (GDArray*)_this;
    gpointer first = _gthis->first, last = _gthis->last - _gthis->cellsize;
    gpointer gptr  = malloc(_gthis->cellsize);
    for (; first < last; first = first + _gthis->cellsize, last = last - _gthis->cellsize) {
        memcpy(gptr,  first, _gthis->cellsize);
        memcpy(first, last,  _gthis->cellsize);
        memcpy(last,  gptr,  _gthis->cellsize);
    }
    free(gptr);
    return _this;
}

static gpointer g_array_at(GArray *_this, int index) {
    GDArray *_gthis = (GDArray*)_this;
    if (index >= _this->size(_this))
        return NULL;
    return (_gthis->first + index * _gthis->cellsize);
}

static GArray* g_array_fill(GArray *_this, gpointer data) {
    GDArray *_gthis = (GDArray*) _this;
    gpointer gptr = NULL;
    for(gptr = _gthis->first; gptr < _gthis->last; gptr += _gthis->cellsize) {
        memcpy(gptr, data, _gthis->cellsize);
    }
    return _this;
}

static int  g_array_size(GArray *_this) {
    GDArray *_gthis = (GDArray*) _this;
    int size = _gthis->last - _gthis->first;
    size = size / _gthis->cellsize;
    return size;
}

static gpointer g_array_data(GArray *_this) {
    GDArray *_gthis = (GDArray*) _this;
    return _gthis->first;
}

static GArray* g_array_assign(GArray *_this, gpointer first, gpointer last) {
    GDArray *_gthis = (GDArray*) _this;
    if (first == NULL || last == NULL || first >= last) {
        return _this;
    }

    int size = _gthis->last - _gthis->first;
    int newsize = last - first;
    if (newsize > size)
        newsize = size;
    memcpy(_gthis->first, first, newsize);
    return _this;
}

static int  g_array_swap(GArray *_this, GArray *_that) {
    gpointer gptr = NULL;
    GDArray *_gthis = (GDArray*) _this, *_gthat = (GDArray*)_that;
    if (_that == NULL) {
        return -1;
    }
    gptr = _gthis->first;
    _gthis->first = _gthat->first;
    _gthat->first = gptr;

    gptr = _gthis->last;
    _gthis->last = _gthat->last;
    _gthat->last = gptr;

    int c = _gthis->cellsize;
    _gthis->cellsize = _gthat->cellsize;
    return 1;
}


GArray* g_array_alloc(int n, int c) { //n - count   c - ElementSize
    GDArray *_gthis = malloc(sizeof(GDArray));
    GArray *_this = NULL;
     if (n <= 0 || c <= 0) {
        return _this;
     }
    _gthis->first = malloc(n * c);
    _gthis->last  = _gthis->first + n * c;
    _gthis->cellsize = c;// unit size > 0

    _this     = &(_gthis->_this);
    _this->free  = g_array_free;  // free _this
    _this->back  = g_array_back;
    _this->front  = g_array_front;
    _this->begin  = g_array_begin;
    _this->end  = g_array_end;
    _this->move  = g_array_move;
    _this->rbegin  = g_array_rbegin;
    _this->rend  = g_array_rend;
    _this->forward  = g_array_forward;
    _this->reverse  = g_array_reverse;
    _this->at  = g_array_at;
    _this->fill  = g_array_fill;
    _this->size  = g_array_size;
    _this->data  = g_array_data;
    _this->assign  = g_array_assign;
    _this->swap  = g_array_swap;
     return _this;
}
