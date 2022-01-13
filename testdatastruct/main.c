#include <stdio.h>
#include <stdlib.h>


#include "test_array.h"
#include "test_stack.h"
#include "test_vector.h"
#include "test_list_alloc.h"
#include "test_queue.h"
#include "test_deque.h"
#include "test_set.h"
#include "test_map.h"

int main(int argc, const char *argv[]) {
    int id = atoi(argv[1]);
    switch(id) {
    case 0x01:
        test_vector();
        break;
    case 0x02:
        test_array();
        break;
    case 0x03:
        test_stack();
        break;
    case 0x04:
        test_list_alloc();
        break;
    case 0x05:
        test_queue();
        break;
    case 0x06:
        test_deque();
        break;
    case 0x07:
        test_set();
        break;
    case 0x08:
        test_map();
        break;
    case 0x00:
        test_array();
        test_vector();
        test_stack();
        test_list_alloc();
        test_queue();
        test_deque();
        test_set();
        test_map();
        break;
    }


    return 0;
}



