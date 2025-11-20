#include <stdio.h>
#include "stdlib.h"
#include "utils.h"
#include "types.h"

int main() {

    //Graph
    t_adjacency_list graph = extract_from_file("../data/exemple2.txt");
    print_adjacency_list(&graph);

    //Create tarjan vertices
    t_tarjan_vertex *vertices = create_tarjan_vertex_array(&graph);
    //print_tarjan_vertices(vertices, graph.size);

    //Create partition
    t_partition partition = tarjan(&graph);
    print_t_partition(&partition);
}
