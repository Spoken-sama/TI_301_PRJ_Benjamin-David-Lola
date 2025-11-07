#include <stdio.h>
#include "utils.h"

int main() {

    struct t_adjacency_list {
        struct vertex **dynamic_array;
        int size;
    };
    struct t_adjacency_list

    t_adjacency_list;
    extract_from_file("../data/exemple1.txt", &t_adjacency_list);
    print_adjacency_list(&t_adjacency_list);


    return 0;
}
