#include "gqueue.h"
#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>

typedef struct _GDQueue  GDQueue;

struct _GDQueue {
    GQueue     _this;
    gpointer   first;
    gpointer   last;
    gpointer   end;
    guint      csize;
};


static    void g_queue_free(GQueue *_this);  // free _this

static    guint  g_queue_size(GQueue *_this);

static    guint  g_queue_empty(GQueue *_this);

static    gpointer g_queue_front(GQueue *_this);

static    gpointer g_queue_back(GQueue *_this);

static    void    g_queue_push(GQueue *_this, gpointer data);

static    void    g_queue_pop(GQueue *_this);

static    void    g_queue_swap(GQueue *_this, GQueue *_that) {
}


GQueue* g_queue_alloc(int n, int c);
