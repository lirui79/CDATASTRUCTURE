#ifndef GARRAY_H_INCLUDED
#define GARRAY_H_INCLUDED


#include "gtypes.h"


G_BEGIN_DECLS


typedef struct _GArray     GArray;



struct _GArray {
  gpointer   *data;
  guint       len;
};







G_END_DECLS


#endif // GARRAY_H_INCLUDED
