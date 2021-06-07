#include  "gvector.h"
#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>

typedef struct _GDVector  GDVector;

struct _GDVector {
    GVector       _this;
    gpointer      first;
    gpointer      last;
    gpointer      end;
    int           cellsize;// elementSize size > 0
};
// vector
// first                last             end
// |                     |               |
// V                     V               V
// +-------------------------------------+
// |    ... data ...     |               |
// +-------------------------------------+
// |<--------size()----->|
// |<---------------capacity()---------->|

static void g_vector_clear(GVector *_this) {
    GDVector *_gthis = (GDVector*)_this;
    _gthis->last = _gthis->first;
}

static void g_vector_free(GVector *_this) {
    GDVector *_gthis = (GDVector*)_this;
    if (_gthis->first == NULL) {
        free(_gthis);
        return;
    }
    free(_gthis->first);
    _gthis->first = NULL;
    _gthis->last  = NULL;
    _gthis->end   = NULL;
    free(_gthis);
}

static GVector* g_vector_push_back(GVector *_this, gpointer data) {
    GDVector *_gthis = (GDVector*)_this;
    return _this->fill(_this, _gthis->last, 1, data);
}

static GVector* g_vector_push_front(GVector *_this, gpointer data) {
    GDVector *_gthis = (GDVector*)_this;
    return _this->fill(_this, _gthis->first, 1, data);
}

static GVector* g_vector_remove(GVector *_this, gpointer position) {
    GDVector *_gthis = (GDVector*)_this;
    return _this->erase(_this, position, position + _gthis->cellsize);
}

static GVector* g_vector_pop_back(GVector *_this) {
    GDVector *_gthis = (GDVector*)_this;
    int size = _gthis->last - _gthis->first;
    if (size < _gthis->cellsize)
        return _this;
    _gthis->last = _gthis->last - _gthis->cellsize;
    return _this;
}

static GVector* g_vector_pop_front(GVector *_this) {
    GDVector *_gthis = (GDVector*)_this;
    gpointer first;
    int size = _gthis->last - _gthis->first;
    if (size < _gthis->cellsize)
        return _this;
    for (first = _gthis->first;first < _gthis->last; first = first + _gthis->cellsize)
        memcpy(first, first + _gthis->cellsize, _gthis->cellsize);
    _gthis->last = _gthis->last - _gthis->cellsize;
    return _this;
}

static gpointer g_vector_back(GVector *_this) {
    GDVector *_gthis = (GDVector*)_this;
    gpointer gptr = NULL;
    int size = _gthis->last - _gthis->first;
    if (size < _gthis->cellsize)
        return gptr;
    gptr = _gthis->last - _gthis->cellsize;
    return gptr;
}

static gpointer g_vector_front(GVector *_this) {
    GDVector *_gthis = (GDVector*)_this;
    if (_gthis->first == _gthis->last)
        return NULL;
    return _gthis->first;
}

static gpointer g_vector_begin(GVector *_this) {
    GDVector *_gthis = (GDVector*)_this;
    return _gthis->first;
}

static gpointer g_vector_end(GVector *_this) {
    GDVector *_gthis = (GDVector*)_this;
    return _gthis->last;
}

static gpointer  g_vector_move(GVector *_this, gpointer position, int n) {
    GDVector *_gthis = (GDVector*)_this;
    position = position + n * _gthis->cellsize;
    return position;
}

static gpointer g_vector_rbegin(GVector *_this) {
    GDVector *_gthis = (GDVector*)_this;
    gpointer gptr = _gthis->last - _gthis->cellsize;
    return gptr;
}

static gpointer g_vector_rend(GVector *_this) {
    GDVector *_gthis = (GDVector*)_this;
    gpointer gptr = _gthis->first - _gthis->cellsize;
    return gptr;
}

static gpointer g_vector_forward(GVector *_this, gpointer position, int n) {
    GDVector *_gthis = (GDVector*)_this;
    position = position - n * _gthis->cellsize;
    return position;
}

static GVector* g_vector_reverse(GVector *_this) {
    GDVector *_gthis = (GDVector*)_this;
    if (_gthis->first == _gthis->last)
        return _this;
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

static GVector* g_vector_copy(GVector *_this, GVector *new_this) {
    GDVector *_gthis = (GDVector*)_this;
    if (new_this == NULL) {
        new_this = g_vector_alloc(_gthis->cellsize);
    }
    return new_this->assign(new_this, _gthis->first, _gthis->last);
}

static gpointer  g_vector_at(GVector *_this, int index) {
    GDVector *_gthis = (GDVector*)_this;
    if (index >= _this->size(_this))
        return NULL;
    return (_gthis->first + index * _gthis->cellsize);
}

static GVector* g_vector_fill(GVector *_this, gpointer position, int n, gpointer data) {
    GDVector *_gthis = (GDVector*)_this;
    if (position == NULL || data == NULL || n <= 0)
        return _this;
    if (position < _gthis->first || position > _gthis->last)
        return _this;
    int newsize = n * _gthis->cellsize, size = _gthis->last - _gthis->first, capacity = _gthis->end - _gthis->first;
    if (newsize + size > capacity) {
        gpointer gptr = malloc(2 * (newsize + size));
        int psize = position - _gthis->first;
        if (psize > 0)
            memcpy(gptr, _gthis->first, psize);
        for (int step = 0; step < newsize; step = step + _gthis->cellsize)
            memcpy(gptr + psize + step, data, _gthis->cellsize);
        if (size > psize)
            memcpy(gptr + psize + newsize, position, size - psize);
        free(_gthis->first);
        _gthis->first = gptr;
        _gthis->last  = gptr + newsize + size;
        _gthis->end   = gptr + 2 * (newsize + size);
        return _this;
    }
    gpointer gptr = _gthis->last - _gthis->cellsize;
    for ( ;gptr >= position; gptr = gptr - _gthis->cellsize) {
        memcpy(gptr + newsize, gptr, _gthis->cellsize);
    }

    for (int step = 0; step < newsize; step = step + _gthis->cellsize)
        memcpy(position + step, data, _gthis->cellsize);
    _gthis->last = _gthis->last + newsize;
    return _this;
}

static int  g_vector_empty(GVector *_this) {
    GDVector *_gthis = (GDVector*)_this;
    if (_gthis->first == _gthis->last)
        return 1;
    return 0;
}

static int  g_vector_size(GVector *_this) {
    GDVector *_gthis = (GDVector*)_this;
    int size = _gthis->last - _gthis->first;
    size = (size / _gthis->cellsize);
    return size;
}

static GVector* g_vector_erase(GVector *_this, gpointer first, gpointer last) {
    GDVector *_gthis = (GDVector*)_this;
    if (first == NULL || last == NULL)
        return _this;
    if (first >= last)
        return _this;
    if (first < _gthis->first)
        first = _gthis->first;
    if (last > _gthis->last)
        last = _gthis->last;
    for (;last < _gthis->last; first = first + _gthis->cellsize, last = last + _gthis->cellsize)
        memcpy(first, last, _gthis->cellsize);
    _gthis->last = first;
    return _this;
}

static gpointer  g_vector_data(GVector *_this) {
    GDVector *_gthis = (GDVector*)_this;
    return _gthis->first;
}

static GVector* g_vector_resize(GVector *_this, int n, gpointer data) {
    GDVector *_gthis = (GDVector*)_this;
    if (_gthis->first == NULL) {
        _gthis->first = malloc(n * _gthis->cellsize);
        _gthis->end  = _gthis->first + n * _gthis->cellsize;
    } else {
        if (n > _this->capacity(_this)) {
            free(_gthis->first);
            _gthis->first = malloc(n * _gthis->cellsize);
            _gthis->end  = _gthis->first + n * _gthis->cellsize;
        }
    }
    _gthis->last = _gthis->first;
    for (int i = 0; i < n; ++i, _gthis->last += _gthis->cellsize) {
        memcpy(_gthis->last, data, _gthis->cellsize);
    }

    return _this;
}

static GVector* g_vector_reserve(GVector *_this, int capacity) {
    GDVector *_gthis = (GDVector*)_this;
    if (capacity <= 0)
        return _this;
    capacity = capacity * _gthis->cellsize;
    int size = _gthis->end - _gthis->first;
    if (size == capacity)
        return _this;
    gpointer gptr = malloc(capacity);
    size = _gthis->last - _gthis->first;
    if (size > capacity)
        size = capacity;
    if (size > 0)
        memcpy(gptr, _gthis->first, size);
    free(_gthis->first);
    _gthis->first = gptr;
    _gthis->last  = gptr + size;
    _gthis->end   = gptr + capacity;
    return _this;
}

static GVector* g_vector_assign(GVector *_this, gpointer first, gpointer last) {
    GDVector *_gthis = (GDVector*)_this;
    if (first == NULL || last == NULL)
        return _this;
    int size = (last - first);

    if (_gthis->first == NULL) {
        _gthis->first = malloc(size);
        _gthis->end  = _gthis->first + size;
    } else {
        if (size > (_gthis->end - _gthis->first)) {
            free(_gthis->first);
            _gthis->first = malloc(size);
            _gthis->end  = _gthis->first + size;
        }
    }

    _gthis->last = _gthis->first + size;
    memcpy(_gthis->first, first, size);
    return _this;
}

static GVector* g_vector_insert(GVector *_this, gpointer position, gpointer first, gpointer last) {
    GDVector *_gthis = (GDVector*)_this;
    if (position == NULL || first == NULL || last == NULL)
        return _this;
    if (position < _gthis->first || position > _gthis->last || first >= last)
        return _this;
    int newsize = last - first, size = _gthis->last - _gthis->first, capacity = _gthis->end - _gthis->first;
    if (newsize + size > capacity) {
        gpointer gptr = malloc(2 * (newsize + size));
        int psize = position - _gthis->first;
        if (psize > 0)
            memcpy(gptr, _gthis->first, psize);
        memcpy(gptr + psize, first, newsize);
        if (size > psize)
            memcpy(gptr + psize + newsize, position, size - psize);
        free(_gthis->first);
        _gthis->first = gptr;
        _gthis->last  = gptr + newsize + size;
        _gthis->end   = gptr + 2 * (newsize + size);
        return _this;
    }
    gpointer gptr = _gthis->last - _gthis->cellsize;
    for ( ;gptr >= position; gptr = gptr - _gthis->cellsize) {
        memcpy(gptr + newsize, gptr, _gthis->cellsize);
    }
    memcpy(position, first, newsize);
    _gthis->last = _gthis->last + newsize;
    return _this;
}

static int g_vector_capacity(GVector *_this) {
    GDVector *_gthis = (GDVector*)_this;
    int capacity = _gthis->end - _gthis->first;
    capacity = (capacity / _gthis->cellsize);
    return capacity;
}

GVector* g_vector_alloc(int cellsize) {//vector unit size
    GDVector* _gthis = (GDVector*) malloc(sizeof(GDVector));
    GVector*  _this  = NULL;
    const int cellcount = 4;
    if (cellsize <= 0 || _gthis == NULL) {
        return NULL;
    }
    _gthis->first = malloc(cellcount * cellsize);
    _gthis->last  = _gthis->first;
    _gthis->end   = _gthis->first + cellcount * cellsize;
    _gthis->cellsize = cellsize;// unit size > 0

    _this     = &(_gthis->_this);
    _this->clear  = g_vector_clear;// but not free _this
    _this->free   = g_vector_free;
    _this->push_back  = g_vector_push_back;
    _this->push_front = g_vector_push_front;
    _this->remove     = g_vector_remove;
    _this->pop_back   = g_vector_pop_back;
    _this->pop_front  = g_vector_pop_front;
    _this->back  = g_vector_back;
    _this->front = g_vector_front;
    _this->begin = g_vector_begin;
    _this->end    = g_vector_end;
    _this->move   = g_vector_move;
    _this->rbegin = g_vector_rbegin;
    _this->rend   = g_vector_rend;
    _this->forward   = g_vector_forward;
    _this->reverse = g_vector_reverse;
    _this->copy    = g_vector_copy;
    _this->at      = g_vector_at;
    _this->fill    = g_vector_fill;
    _this->empty   = g_vector_empty;
    _this->size    = g_vector_size;
    _this->erase   = g_vector_erase;
    _this->data    = g_vector_data;
    _this->resize = g_vector_resize;
    _this->reserve = g_vector_reserve;
    _this->assign = g_vector_assign;
    _this->insert   = g_vector_insert;
    _this->capacity = g_vector_capacity;
    return _this;
}
