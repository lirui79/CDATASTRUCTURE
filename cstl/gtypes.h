#ifndef GTYPES_H_INCLUDED
#define GTYPES_H_INCLUDED

#include "gmacros.h"


G_BEGIN_DECLS

/* Provide type definitions for commonly used types.
 *  These are useful because a "gint8" can be adjusted
 *  to be 1 byte (8 bits) on all platforms. Similarly and
 *  more importantly, "gint32" can be adjusted to be
 *  4 bytes (32 bits) on all platforms.
 */

typedef char   gchar;
typedef short  gshort;
typedef long   glong;
typedef int    gint;
typedef int    gboolean;

typedef unsigned char   guchar;
typedef unsigned short  gushort;
typedef unsigned long   gulong;
typedef unsigned int    guint;

typedef float   gfloat;
typedef double  gdouble;

typedef void*   gpointer;
typedef const void*   gconstpointer;


typedef struct  _GType    GType;

struct  _GType {
    gpointer     data;
    guint        size;
};



GType  g_default_type(gpointer data, guint size);



G_END_DECLS



#endif // GTYPES_H_INCLUDED
