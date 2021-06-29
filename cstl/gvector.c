#include  "gvector.h"
#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>



static GIterator       g_dvector_iterator_next(GIterator *thiz) {
    thiz->itdata += thiz->itsize;
    return thiz[0];
}

static GIterator       g_dvector_iterator_prev(GIterator *thiz) {
    thiz->itdata -= thiz->itsize;
    return thiz[0];
}

static GIterator       g_dvector_iterator_forward(GIterator *thiz, guint n) {
    thiz->itdata += n * thiz->itsize;
    return thiz[0];
}

static GIterator       g_dvector_iterator_backward(GIterator *thiz, guint n) {
    thiz->itdata -= n * thiz->itsize;
    return thiz[0];
}

static int              g_dvector_iterator_equal(GIterator *thiz, GIterator *that) {
    if (thiz->itdata == that->itdata)
        return 1;
    return 0;
}

static int              g_dvector_iterator_not_equal(GIterator *thiz, GIterator *that) {
    if (thiz->itdata != that->itdata)
        return 1;
    return 0;
}

static int              g_dvector_iterator_less(GIterator *thiz, GIterator *that) {
    if (thiz->itdata < that->itdata)
        return 1;
    return 0;
}

static int              g_dvector_iterator_less_equal(GIterator *thiz, GIterator *that) {
    if (thiz->itdata <= that->itdata)
        return 1;
    return 0;
}

static int              g_dvector_iterator_greater(GIterator *thiz, GIterator *that) {
    if (thiz->itdata > that->itdata)
        return 1;
    return 0;
}

static int              g_dvector_iterator_greater_equal(GIterator *thiz, GIterator *that) {
    if (thiz->itdata >= that->itdata)
        return 1;
    return 0;
}

static gpointer         g_dvector_iterator_data(GIterator *thiz) {
    return thiz->itdata;
}

static guint            g_dvector_iterator_size(GIterator *thiz) {
    return thiz->itsize;
}

static GIterator       g_dvector_iterator_set(GIterator *thiz, gpointer data, guint sz) {
    thiz->itdata = data;
    thiz->itsize = sz;
    return thiz[0];
}

static GIterator g_dvector_iterator_init(gpointer data, guint size, guint dir) {
    GIterator        thiz  = {0};
    thiz.itdata  = data;
    thiz.itsize  = size;
    thiz.dir   = dir;
    if (dir > 0) {
        thiz.next  = g_dvector_iterator_next;
        thiz.prev  = g_dvector_iterator_prev;
        thiz.forward  = g_dvector_iterator_forward;
        thiz.backward  = g_dvector_iterator_backward;
    } else {
        thiz.next  = g_dvector_iterator_prev;
        thiz.prev  = g_dvector_iterator_next;
        thiz.forward  = g_dvector_iterator_backward;
        thiz.backward  = g_dvector_iterator_forward;
    }
    thiz.equal  = g_dvector_iterator_equal;
    thiz.not_equal  = g_dvector_iterator_not_equal;
    thiz.less  = g_dvector_iterator_less;
    thiz.less_equal  = g_dvector_iterator_less_equal;
    thiz.greater  = g_dvector_iterator_greater;
    thiz.greater_equal  = g_dvector_iterator_greater_equal;
    thiz.data  = g_dvector_iterator_data;
    thiz.size  = g_dvector_iterator_size;
    thiz.set  = g_dvector_iterator_set;
    return thiz;
}

GIterator g_vector_iterator() {
    return g_dvector_iterator_init(NULL, 0, 1);
}

typedef struct _GDVector  GDVector;

struct _GDVector {
    GVector       thiz;
    gpointer      first;
    gpointer      last;
    gpointer      end;
    guint         csize;// elementSize size > 0
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
    GDVector *gthiz = (GDVector*)_this;
    gthiz->last = gthiz->first;
}

static void g_vector_free(GVector *_this) {
    GDVector *gthiz = (GDVector*)_this;
    if (gthiz->first == NULL) {
        free(gthiz);
        return;
    }
    free(gthiz->first);
    gthiz->first = NULL;
    gthiz->last  = NULL;
    gthiz->end   = NULL;
    free(gthiz);
}

static guint  g_vector_empty(GVector *_this) {
    GDVector *gthiz = (GDVector*)_this;
    if (gthiz->first == gthiz->last)
        return 1;
    return 0;
}

static guint  g_vector_size(GVector *_this) {
    GDVector *gthiz = (GDVector*)_this;
    guint size = gthiz->last - gthiz->first;
    size = (size / gthiz->csize);
    return size;
}

static guint g_vector_capacity(GVector *_this) {
    GDVector *gthiz = (GDVector*)_this;
    guint capacity = gthiz->end - gthiz->first;
    capacity = (capacity / gthiz->csize);
    return capacity;
}

static void g_vector_resize(GVector *_this, guint n, gpointer data) {
    GDVector *gthiz = (GDVector*)_this;
    if (gthiz->first == NULL) {
        gthiz->first = malloc(n * gthiz->csize);
        gthiz->end  = gthiz->first + n * gthiz->csize;
    } else {
        if (n > _this->capacity(_this)) {
            free(gthiz->first);
            gthiz->first = malloc(n * gthiz->csize);
            gthiz->end  = gthiz->first + n * gthiz->csize;
        }
    }
    gthiz->last = gthiz->first;
    for (guint i = 0; i < n; ++i, gthiz->last += gthiz->csize) {
        memcpy(gthiz->last, data, gthiz->csize);
    }
}

static void g_vector_reserve(GVector *_this, guint capacity) {
    GDVector *gthiz = (GDVector*)_this;
    if (capacity <= 0)
        return;
    capacity = capacity * gthiz->csize;
    guint size = gthiz->end - gthiz->first;
    if (size == capacity)
        return;
    gpointer gptr = malloc(capacity);
    size = gthiz->last - gthiz->first;
    if (size > capacity)
        size = capacity;
    if (size > 0)
        memcpy(gptr, gthiz->first, size);
    free(gthiz->first);
    gthiz->first = gptr;
    gthiz->last  = gptr + size;
    gthiz->end   = gptr + capacity;
}


static gpointer g_vector_back(GVector *_this) {
    GDVector *gthiz = (GDVector*)_this;
    gpointer gptr = NULL;
    guint size = gthiz->last - gthiz->first;
    if (size < gthiz->csize)
        return gthiz->last;
    gptr = gthiz->last - gthiz->csize;
    return gptr;
}

static gpointer g_vector_front(GVector *_this) {
    GDVector *gthiz = (GDVector*)_this;
    if (gthiz->first == gthiz->last)
        return gthiz->last;
    return gthiz->first;
}

static gpointer  g_vector_at(GVector *thiz, guint index) {
    GDVector *gthiz = (GDVector*)thiz;
    if (index >= thiz->size(thiz))
        return gthiz->last;
    return (gthiz->first + index * gthiz->csize);
}

static gpointer  g_vector_data(GVector *thiz) {
    GDVector *gthiz = (GDVector*)thiz;
    return gthiz->first;
}



static GIterator g_vector_begin(GVector *thiz) {
    GDVector *gthiz = (GDVector*)thiz;
    GIterator iterator = g_dvector_iterator_init(gthiz->first, gthiz->csize, 1);
    return iterator;
}

static GIterator g_vector_end(GVector *thiz) {
    GDVector *gthiz = (GDVector*)thiz;
    GIterator iterator = g_dvector_iterator_init(gthiz->last, gthiz->csize, 1);
    return iterator;
}

static GIterator g_vector_rbegin(GVector *thiz) {
    GDVector *gthiz = (GDVector*)thiz;
    GIterator iterator = g_dvector_iterator_init(gthiz->last - gthiz->csize, gthiz->csize, 0);
    return iterator;
}

static GIterator g_vector_rend(GVector *thiz) {
    GDVector *gthiz = (GDVector*)thiz;
    GIterator iterator = g_dvector_iterator_init(gthiz->first - gthiz->csize, gthiz->csize, 0);
    return iterator;
}



static void g_vector_push_back(GVector *_this, gpointer data) {
    GDVector *gthiz = (GDVector*)_this;
    GIterator iterator = g_dvector_iterator_init(gthiz->last, gthiz->csize, 1);
    _this->fill(_this, iterator, 1, data);
}

static void g_vector_push_front(GVector *_this, gpointer data) {
    GDVector *gthiz = (GDVector*)_this;
    GIterator iterator = g_dvector_iterator_init(gthiz->first, gthiz->csize, 1);
    _this->fill(_this, iterator, 1, data);
}

static void g_vector_pop_back(GVector *_this) {
    GDVector *gthiz = (GDVector*)_this;
    guint size = gthiz->last - gthiz->first;
    if (size < gthiz->csize)
        return;
    gthiz->last = gthiz->last - gthiz->csize;
}

static void g_vector_pop_front(GVector *_this) {
    GDVector *gthiz = (GDVector*)_this;
    gpointer first;
    guint size = gthiz->last - gthiz->first;
    if (size < gthiz->csize)
        return;
    for (first = gthiz->first;first < gthiz->last; first = first + gthiz->csize)
        memcpy(first, first + gthiz->csize, gthiz->csize);
    gthiz->last = gthiz->last - gthiz->csize;
}

static GIterator g_vector_erase(GVector *thiz, GIterator itfirst, GIterator itlast) {
    GDVector *gthiz = (GDVector*)thiz;
    gpointer first = itfirst.data(&itfirst), last = itlast.data(&itlast);
    if (first == NULL || last == NULL || first >= last)
        return itfirst;
    if (first >= last)
        return itfirst;
    if (first < gthiz->first)
        first = gthiz->first;
    if (last > gthiz->last)
        last = gthiz->last;
    for (;last < gthiz->last; first = first + gthiz->csize, last = last + gthiz->csize)
        memcpy(first, last, gthiz->csize);
    gthiz->last = first;
    itfirst.set(&itfirst, first, gthiz->csize);
    return itfirst;
}

static GIterator g_vector_remove(GVector *thiz, GIterator position) {
    GDVector *gthiz = (GDVector*)thiz;
    GIterator first = position, last = position;
    last.set(&last, position.data(&position) + gthiz->csize, gthiz->csize);
    return thiz->erase(thiz, first, last);
}




static void g_vector_assign(GVector *thiz, GIterator itfirst, GIterator itlast) {
    GDVector *gthiz = (GDVector*)thiz;
    gpointer first = itfirst.data(&itfirst), last = itlast.data(&itlast);
    if (first == NULL || last == NULL || first >= last)
        return;
    guint size = (last - first);

    if (gthiz->first == NULL) {
        gthiz->first = malloc(size);
        gthiz->end  = gthiz->first + size;
    } else {
        if (size > (gthiz->end - gthiz->first)) {
            free(gthiz->first);
            gthiz->first = malloc(size);
            gthiz->end  = gthiz->first + size;
        }
    }

    gthiz->last = gthiz->first + size;
    memcpy(gthiz->first, first, size);
}

static void g_vector_fill(GVector *thiz, GIterator itposition, guint n, gpointer data) {
    GDVector *gthiz = (GDVector*)thiz;
    gpointer position = itposition.data(&itposition);
    if (position == NULL || data == NULL || n <= 0)
        return;
    if (position < gthiz->first || position > gthiz->last)
        return;
    guint newsize = n * gthiz->csize, size = gthiz->last - gthiz->first, capacity = gthiz->end - gthiz->first;
    if (newsize + size > capacity) {
        gpointer gptr = malloc(2 * (newsize + size));
        guint psize = position - gthiz->first;
        if (psize > 0)
            memcpy(gptr, gthiz->first, psize);
        for (guint step = 0; step < newsize; step = step + gthiz->csize)
            memcpy(gptr + psize + step, data, gthiz->csize);
        if (size > psize)
            memcpy(gptr + psize + newsize, position, size - psize);
        free(gthiz->first);
        gthiz->first = gptr;
        gthiz->last  = gptr + newsize + size;
        gthiz->end   = gptr + 2 * (newsize + size);
        return;
    }
    gpointer gptr = gthiz->last - gthiz->csize;
    for ( ;gptr >= position; gptr = gptr - gthiz->csize) {
        memcpy(gptr + newsize, gptr, gthiz->csize);
    }

    for (guint step = 0; step < newsize; step = step + gthiz->csize)
        memcpy(position + step, data, gthiz->csize);
    gthiz->last = gthiz->last + newsize;
}

static void g_vector_insert(GVector *thiz, GIterator itposition, GIterator itfirst, GIterator itlast) {
    GDVector *gthiz = (GDVector*)thiz;
    gpointer first = itfirst.data(&itfirst), last = itlast.data(&itlast), position = itposition.data(&itposition);
    if (position == NULL || first == NULL || last == NULL)
        return;
    if (position < gthiz->first || position > gthiz->last || first >= last)
        return;
    guint newsize = last - first, size = gthiz->last - gthiz->first, capacity = gthiz->end - gthiz->first;
    if (newsize + size > capacity) {
        gpointer gptr = malloc(2 * (newsize + size));
        guint psize = position - gthiz->first;
        if (psize > 0)
            memcpy(gptr, gthiz->first, psize);
        memcpy(gptr + psize, first, newsize);
        if (size > psize)
            memcpy(gptr + psize + newsize, position, size - psize);
        free(gthiz->first);
        gthiz->first = gptr;
        gthiz->last  = gptr + newsize + size;
        gthiz->end   = gptr + 2 * (newsize + size);
        return;
    }
    gpointer gptr = gthiz->last - gthiz->csize;
    for ( ;gptr >= position; gptr = gptr - gthiz->csize) {
        memcpy(gptr + newsize, gptr, gthiz->csize);
    }
    memcpy(position, first, newsize);
    gthiz->last = gthiz->last + newsize;
}

static void g_vector_reverse(GVector *thiz) {
    GDVector *gthiz = (GDVector*)thiz;
    if (gthiz->first == gthiz->last)
        return;
    gpointer first = gthiz->first, last = gthiz->last - gthiz->csize;
    gpointer gptr  = malloc(gthiz->csize);
    for (; first < last; first = first + gthiz->csize, last = last - gthiz->csize) {
        memcpy(gptr,  first, gthiz->csize);
        memcpy(first, last,  gthiz->csize);
        memcpy(last,  gptr,  gthiz->csize);
    }
    free(gptr);
}

static    void g_vector_swap(GVector *thiz, GVector *that) {
    gpointer gptr = NULL;
    GDVector *gthiz = (GDVector*) thiz, *gthat = (GDVector*)that;
    if (that == NULL) {
        return;
    }
    gptr = gthiz->first;
    gthiz->first = gthat->first;
    gthat->first = gptr;

    gptr = gthiz->last;
    gthiz->last = gthat->last;
    gthat->last = gptr;

    gptr = gthiz->end;
    gthiz->end = gthat->end;
    gthat->end = gptr;

    guint c = gthiz->csize;
    gthiz->csize = gthat->csize;
    gthat->csize = c;
}


GVector* g_vector_alloc(guint n, guint c) {//vector unit size
    GDVector* gthiz = NULL;
    GVector*  thiz  = NULL;
    if (n <= 0 || c <= 0) {
        return thiz;
    }
    gthiz = (GDVector*) malloc(sizeof(GDVector));
    gthiz->first = malloc(n * c);
    gthiz->last  = gthiz->first;
    gthiz->end   = gthiz->first + n * c;
    gthiz->csize = c;// unit size > 0

    thiz     = &(gthiz->thiz);
    thiz->clear  = g_vector_clear;// but not free thiz
    thiz->free   = g_vector_free;

    thiz->empty    = g_vector_empty;
    thiz->size     = g_vector_size;
    thiz->capacity = g_vector_capacity;
    thiz->resize   = g_vector_resize;
    thiz->reserve  = g_vector_reserve;

    thiz->back     = g_vector_back;
    thiz->front    = g_vector_front;
    thiz->at       = g_vector_at;
    thiz->data     = g_vector_data;

    thiz->begin    = g_vector_begin;
    thiz->end      = g_vector_end;
    thiz->rbegin   = g_vector_rbegin;
    thiz->rend     = g_vector_rend;

    thiz->push_back  = g_vector_push_back;
    thiz->push_front = g_vector_push_front;
    thiz->pop_back   = g_vector_pop_back;
    thiz->pop_front  = g_vector_pop_front;
    thiz->erase      = g_vector_erase;
    thiz->remove     = g_vector_remove;

    thiz->assign     = g_vector_assign;
    thiz->fill       = g_vector_fill;
    thiz->insert     = g_vector_insert;
    thiz->reverse    = g_vector_reverse;
    thiz->swap       = g_vector_swap;
    return thiz;
}
