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
    guint         typesize;// elementSize typesize > 0
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

static guint  g_vector_typesize(GVector *thiz) {
    GDVector *gthiz = (GDVector*)thiz;
    return gthiz->typesize;
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
    size = (size / gthiz->typesize);
    return size;
}

static guint g_vector_capacity(GVector *_this) {
    GDVector *gthiz = (GDVector*)_this;
    guint capacity = gthiz->end - gthiz->first;
    capacity = (capacity / gthiz->typesize);
    return capacity;
}

static void g_vector_resize(GVector *_this, guint n, gconstpointer val) {
    GDVector *gthiz = (GDVector*)_this;
    if (gthiz->first == NULL) {
        gthiz->first = malloc(n * gthiz->typesize);
        gthiz->end  = gthiz->first + n * gthiz->typesize;
    } else {
        if (n > _this->capacity(_this)) {
            free(gthiz->first);
            gthiz->first = malloc(n * gthiz->typesize);
            gthiz->end  = gthiz->first + n * gthiz->typesize;
        }
    }

    gthiz->last = gthiz->first;
    for (guint i = 0; i < n; ++i, gthiz->last += gthiz->typesize) {
        memcpy(gthiz->last, val, gthiz->typesize);
    }
}

static void g_vector_reserve(GVector *_this, guint capacity) {
    GDVector *gthiz = (GDVector*)_this;
    if (capacity <= 0)
        return;
    capacity = capacity * gthiz->typesize;
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
    gpointer val = NULL;
    guint size = gthiz->last - gthiz->first;
    if (size < gthiz->typesize)
        return gthiz->last;
    val = gthiz->last - gthiz->typesize;
    return val;
}

static gpointer g_vector_front(GVector *_this) {
    GDVector *gthiz = (GDVector*)_this;
    if (gthiz->first == gthiz->last)
        return gthiz->last;
    return gthiz->first;
}

static gpointer  g_vector_at(GVector *thiz, guint index) {
    GDVector *gthiz = (GDVector*)thiz;
    gpointer val = gthiz->last;
    if (index >= thiz->size(thiz))
        return val;
    val = (gthiz->first + index * gthiz->typesize);
    return val;
}

static gpointer  g_vector_data(GVector *thiz) {
    GDVector *gthiz = (GDVector*)thiz;
    return gthiz->first;
}



static GIterator g_vector_begin(GVector *thiz) {
    GDVector *gthiz = (GDVector*)thiz;
    GIterator iterator = g_default_iterator(gthiz->first, gthiz->typesize, 1);
    return iterator;
}

static GIterator g_vector_end(GVector *thiz) {
    GDVector *gthiz = (GDVector*)thiz;
    GIterator iterator = g_default_iterator(gthiz->last, gthiz->typesize, 1);
    return iterator;
}

static GIterator g_vector_rbegin(GVector *thiz) {
    GDVector *gthiz = (GDVector*)thiz;
    GIterator iterator = g_default_iterator(gthiz->last - gthiz->typesize, gthiz->typesize, 0);
    return iterator;
}

static GIterator g_vector_rend(GVector *thiz) {
    GDVector *gthiz = (GDVector*)thiz;
    GIterator iterator = g_default_iterator(gthiz->first - gthiz->typesize, gthiz->typesize, 0);
    return iterator;
}



static void g_vector_push_back(GVector *_this, gconstpointer val) {
    GDVector *gthiz = (GDVector*)_this;
    GIterator iterator = g_default_iterator(gthiz->last, gthiz->typesize, 1);
    _this->fill(_this, iterator, 1, val);
}

static void g_vector_push_front(GVector *_this, gconstpointer val) {
    GDVector *gthiz = (GDVector*)_this;
    GIterator iterator = g_default_iterator(gthiz->first, gthiz->typesize, 1);
    _this->fill(_this, iterator, 1, val);
}

static void g_vector_pop_back(GVector *_this) {
    GDVector *gthiz = (GDVector*)_this;
    guint size = gthiz->last - gthiz->first;
    if (size < gthiz->typesize)
        return;
    gthiz->last = gthiz->last - gthiz->typesize;
}

static void g_vector_pop_front(GVector *_this) {
    GDVector *gthiz = (GDVector*)_this;
    gpointer first;
    guint size = gthiz->last - gthiz->first;
    if (size < gthiz->typesize)
        return;
    for (first = gthiz->first;first < gthiz->last; first = first + gthiz->typesize)
        memcpy(first, first + gthiz->typesize, gthiz->typesize);
    gthiz->last = gthiz->last - gthiz->typesize;
}

static GIterator g_vector_erase(GVector *thiz, GIterator itfirst, GIterator itlast) {
    GDVector *gthiz = (GDVector*)thiz;
    gpointer first = itfirst.get(&itfirst).data, last = itlast.get(&itlast).data;
    if (first == NULL || last == NULL || first >= last)
        return itfirst;
    if (first < gthiz->first)
        first = gthiz->first;
    if (last > gthiz->last)
        last = gthiz->last;
    for (;last < gthiz->last; first = first + gthiz->typesize, last = last + gthiz->typesize)
        memcpy(first, last, gthiz->typesize);
    gthiz->last = first;
    itfirst.set(&itfirst, g_default_ref(first, gthiz->typesize));
    return itfirst;
}

static GIterator g_vector_remove(GVector *thiz, GIterator position) {
    GDVector *gthiz = (GDVector*)thiz;
    GIterator first = position, last = position;
    GRef ref = {position.get(&position).data + gthiz->typesize, gthiz->typesize};
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

static void g_vector_fill(GVector *thiz, GIterator itposition, guint n, gconstpointer val) {
    GDVector *gthiz = (GDVector*)thiz;
    gpointer position = itposition.get(&itposition).data;
    gpointer data = val;
    if (position == NULL || data == NULL || n <= 0)
        return;
    if (position < gthiz->first || position > gthiz->last)
        return;
    guint newsize = n * gthiz->typesize, size = gthiz->last - gthiz->first, capacity = gthiz->end - gthiz->first;
    if (newsize + size > capacity) {
        gpointer gptr = malloc(2 * (newsize + size));
        guint psize = position - gthiz->first;
        if (psize > 0)
            memcpy(gptr, gthiz->first, psize);
        for (guint step = 0; step < newsize; step = step + gthiz->typesize)
            memcpy(gptr + psize + step, val, gthiz->typesize);
        if (size > psize)
            memcpy(gptr + psize + newsize, position, size - psize);
        free(gthiz->first);
        gthiz->first = gptr;
        gthiz->last  = gptr + newsize + size;
        gthiz->end   = gptr + 2 * (newsize + size);
        return;
    }
    gpointer gptr = gthiz->last - gthiz->typesize;
    for ( ;gptr >= position; gptr = gptr - gthiz->typesize) {
        memcpy(gptr + newsize, gptr, gthiz->typesize);
    }

    for (guint step = 0; step < newsize; step = step + gthiz->typesize)
        memcpy(position + step, val, gthiz->typesize);
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
    gpointer gptr = gthiz->last - gthiz->typesize;
    for ( ;gptr >= position; gptr = gptr - gthiz->typesize) {
        memcpy(gptr + newsize, gptr, gthiz->typesize);
    }
    memcpy(position, first, newsize);
    gthiz->last = gthiz->last + newsize;
}

static void g_vector_reverse(GVector *thiz) {
    GDVector *gthiz = (GDVector*)thiz;
    if (gthiz->first == gthiz->last)
        return;
    gpointer first = gthiz->first, last = gthiz->last - gthiz->typesize;
    gpointer gptr  = malloc(gthiz->typesize);
    for (; first < last; first = first + gthiz->typesize, last = last - gthiz->typesize) {
        memcpy(gptr,  first, gthiz->typesize);
        memcpy(first, last,  gthiz->typesize);
        memcpy(last,  gptr,  gthiz->typesize);
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

    guint c = gthiz->typesize;
    gthiz->typesize = gthat->typesize;
    gthat->typesize = c;
}


GVector* g_vector_alloc(guint cnt, guint typesize) {//vector unit size
    GDVector* gthiz = NULL;
    GVector*  thiz  = NULL;
    if (cnt <= 0 || typesize <= 0) {
        return thiz;
    }
    gthiz = (GDVector*) malloc(sizeof(GDVector));
    gthiz->first = malloc(cnt * typesize);
    gthiz->last  = gthiz->first;
    gthiz->end   = gthiz->first + cnt * typesize;
    gthiz->typesize  = typesize;// unit typesize > 0

    thiz     = &(gthiz->thiz);
    thiz->clear  = g_vector_clear;// but not free thiz
    thiz->free   = g_vector_free;
    thiz->typesize   = g_vector_typesize;

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
