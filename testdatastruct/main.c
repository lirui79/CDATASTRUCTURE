#include <stdio.h>
#include <stdlib.h>

#include "test_vector.h"
#include "test_list_alloc.h"


typedef struct {
//   typedef gpointer iterator;
   int n;
}  my;

struct VAL {

 union  {
     float fVal;
     int   iVal;
 };
};


int main() {
    float val = 1.3;
    int   ival = 0;
    struct VAL  v;
    //struct VAL::iterator  it;

    memcpy(&ival, &val, sizeof(float));
    memcpy(&v, &val, sizeof(float));

    test_vector();
    test_list_alloc();

    printf("Hello world!\n");
    printf("ival = %d  val = %f   %d   %f\n", ival, val, v.iVal, v.fVal);
    return 0;
}



