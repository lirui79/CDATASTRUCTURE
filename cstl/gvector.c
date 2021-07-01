#include  "gvector.h"
#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>



typedef struct _GDVector  GDVector;

struct _GDVector {
    GVector       thiz;
    gpointer      first;
    gpointer      last;
    gpointer      end;
    guint         size;// elementSize size > 0
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
    size = (size / gthiz->size);
    return size;
}

static guint g_vector_capacity(GVector *_this) {
    GDVector *gthiz = (GDVector*)_this;
    guint capacity = gthiz->end - gthiz->first;
    capacity = (capacity / gthiz->size);
    return capacity;
}

static void g_vector_resize(GVector *_this, guint n, GType val) {
    GDVector *gthiz = (GDVector*)_this;
    if (gthiz->first == NULL) {
        gthiz->first = malloc(n * gthiz->size);
        gthiz->end  = gthiz->first + n * gthiz->size;
    } else {
        if (n > _this->capacity(_this)) {
            free(gthiz->first);
            gthiz->first = malloc(n * gthiz->size);
            gthiz->end  = gthiz->first + n * gthiz->size;
        }
    }

    gthiz->last = gthiz->first;
    for (guint i = 0; i < n; ++i, gthiz->last += gthiz->size) {
        memcpy(gthiz->last, val.data, gthiz->size);
    }
}

static void g_vector_reserve(GVector *_this, guint capacity) {
    GDVector *gthiz = (GDVector*)_this;
    if (capacity <= 0)
        return;
    capacity = capacity * gthiz->size;
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


static GType g_vector_back(GVector *_this) {
    GDVector *gthiz = (GDVector*)_this;
    GType val = {gthiz->last, gthiz->size};
    guint size = gthiz->last - gthiz->first;
    if (size < gthiz->size)
        return val;
    val.data = gthiz->last - gthiz->size;
    return val;
}

static GType g_vector_front(GVector *_this) {
    GDVector *gthiz = (GDVector*)_this;
    GType val = {gthiz->last, gthiz->size};
    if (gthiz->first == gthiz->last)
        return val;
    val.data = gthiz->first;
    return val;
}

static GType  g_vector_at(GVector *thiz, guint index) {
    GDVector *gthiz = (GDVector*)thiz;
    GType val = {gthiz->last, gthiz->size};
    if (index >= thiz->size(thiz))
        return val;
    val.data = (gthiz->first + index * gthiz->size);
    return val;
}

static GType  g_vector_data(GVector *thiz) {
    GDVector *gthiz = (GDVector*)thiz;
    GType val = {gthiz->first, gthiz->size};
    return val;
}



static GIterator g_vector_begin(GVector *thiz) {
    GDVector *gthiz = (GDVector*)thiz;
    GIterator iterator = g_default_iterator(gthiz->first, gthiz->size, 1);
    return iterator;
}

static GIterator g_vector_end(GVector *thiz) {
    GDVector *gthiz = (GDVector*)thiz;
    GIterator iterator = g_default_iterator(gthiz->last, gthiz->size, 1);
    return iterator;
}

static GIterator g_vector_rbegin(GVector *thiz) {
    GDVector *gthiz = (GDVector*)thiz;
    GIterator iterator = g_default_iterator(gthiz->last - gthiz->size, gthiz->size, 0);
    return iterator;
}

static GIterator g_vector_rend(GVector *thiz) {
    GDVector *gthiz = (GDVector*)thiz;
    GIterator iterator = g_default_iterator(gthiz->first - gthiz->size, gthiz->size, 0);
    return iterator;
}



static void g_vector_push_back(GVector *_this, GType val) {
    GDVector *gthiz = (GDVector*)_this;
    GIterator iterator = g_default_iterator(gthiz->last, gthiz->size, 1);
    _this->fill(_this, iterator, 1, val);
}

static void g_vector_push_front(GVector *_this, GType val) {
    GDVector *gthiz = (GDVector*)_this;
    GIterator iterator = g_default_iterator(gthiz->first, gthiz->size, 1);
    _this->fill(_this, iterator, 1, val);
}

static void g_vector_pop_back(GVector *_this) {
    GDVector *gthiz = (GDVector*)_this;
    guint size = gthiz->last - gthiz->first;
    if (size < gthiz->size)
        return;
    gthiz->last = gthiz->last - gthiz->size;
}

static void g_vector_pop_front(GVector *_this) {
    GDVector *gthiz = (GDVector*)_this;
    gpointer first;
    guint size = gthiz->last - gthiz->first;
    if (size < gthiz->size)
        return;
    for (first = gthiz->first;first < gthiz->last; first = first + gthiz->size)
        memcpy(first, first + gthiz->size, gthiz->size);
    gthiz->last = gthiz->last - gthiz->size;
}

static GIterator g_vector_erase(GVector *thiz, GIterator itfirst, GIterator itlast) {
    GDVector *gthiz = (GDVector*)thiz;
    gpointer first = itfirst.get(&itfirst).data, last = itlast.get(&itlast).data;
    GType ref = {NULL, gthiz->size};
    if (first == NULL || last == NULL || first >= last)
        return itfirst;
    if (first >= last)
        return itfirst;
    if (first < gthiz->first)
        first = gthiz->first;
    if (last > gthiz->last)
        last = gthiz->last;
    for (;last < gthiz->last; first = first + gthiz->size, last = last + gthiz->size)
        memcpy(first, last, gthiz->size);
    gthiz->last = first;
    ref.data = first;
    itfirst.set(&itfirst, ref);
    return itfirst;
}

static GIterator g_vector_remove(GVector *thiz, GIterator position) {
    GDVector *gthiz = (GDVector*)thiz;
    GIterator first = position, last = position;
    GType ref = {position.get(&position).data + gthiz->size, gthiz->size};
    last.set(&last, ref);
    return thiz->erase(thiz, first, last);
}




static void g_vector_assign(GVector *thiz, GIterator itfirst, GIterator itlast) {
    GDVector *gthiz = (GDVector*)thiz;
    gpointer first = itfirst.get(&itfirst).data, last = itlast.get(&itlast).data;
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

static void g_vector_fill(GVector *thiz, GIterator itposition, guint n, GType val) {
    GDVector *gthiz = (GDVector*)thiz;
    gpointer position = itposition.get(&itposition).data;
    gpointer data = val.data;
    if (position == NULL || data == NULL || n <= 0)
        return;
    if (position < gthiz->first || position > gthiz->last)
        return;
    guint newsize = n * gthiz->size, size = gthiz->last - gthiz->first, capacity = gthiz->end - gthiz->first;
    if (newsize + size > capacity) {
        gpointer gptr = malloc(2 * (newsize + size));
        guint psize = position - gthiz->first;
        if (psize > 0)
            memcpy(gptr, gthiz->first, psize);
        for (guint step = 0; step < newsize; step = step + gthiz->size)
            memcpy(gptr + psize + step, val.data, gthiz->size);
        if (size > psize)
            memcpy(gptr + psize + newsize, position, size - psize);
        free(gthiz->first);
        gthiz->first = gptr;
        gthiz->last  = gptr + newsize + size;
        gthiz->end   = gptr + 2 * (newsize + size);
        return;
    }
    gpointer gptr = gthiz->last - gthiz->size;
    for ( ;gptr >= position; gptr = gptr - gthiz->size) {
        memcpy(gptr + newsize, gptr, gthiz->size);
    }

    for (guint step = 0; step < newsize; step = step + gthiz->size)
        memcpy(position + step, val.data, gthiz->size);
    gthiz->last = gthiz->last + newsize;
}

static void g_vector_insert(GVector *thiz, GIterator itposition, GIterator itfirst, GIterator itlast) {
    GDVector *gthiz = (GDVector*)thiz;
    gpointer first = itfirst.get(&itfirst).data, last = itlast.get(&itlast).data, position = itposition.get(&itposition).data;
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
    gpointer gptr = gthiz->last - gthiz->size;
    for ( ;gptr >= position; gptr = gptr - gthiz->size) {
        memcpy(gptr + newsize, gptr, gthiz->size);
    }
    memcpy(position, first, newsize);
    gthiz->last = gthiz->last + newsize;
}

static void g_vector_reverse(GVector *thiz) {
    GDVector *gthiz = (GDVector*)thiz;
    if (gthiz->first == gthiz->last)
        return;
    gpointer first = gthiz->first, last = gthiz->last - gthiz->size;
    gpointer gptr  = malloc(gthiz->size);
    for (; first < last; first = first + gthiz->size, last = last - gthiz->size) {
        memcpy(gptr,  first, gthiz->size);
        memcpy(first, last,  gthiz->size);
        memcpy(last,  gptr,  gthiz->size);
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

    guint c = gthiz->size;
    gthiz->size = gthat->size;
    gthat->size = c;
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
    gthiz->size = c;// unit size > 0

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
