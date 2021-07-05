#include <stdio.h>
#include <stdlib.h>

#include "test_set.h"
#include "test_map.h"
#include "test_queue.h"
#include "test_deque.h"
#include "test_array.h"
#include "test_stack.h"
#include "test_vector.h"
#include "test_list_alloc.h"

int main() {


    test_set();
    test_map();
    test_stack();

    test_queue();
    test_deque();

    test_array();
    test_vector();
    test_list_alloc();

    return 0;
}



