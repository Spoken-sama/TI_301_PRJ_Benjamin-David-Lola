#ifndef UTILS_H
#define UTILS_H

#include "types.h"

void add_cell_to_list(list *lst, int to, float probability);
void display_list(const list *lst);
t_adjacency_list create_empty_adjacency_list(int size);
t_adjacency_list extract_from_file(const char *file_path);
void print_adjacency_list(const t_adjacency_list *p_adjacency_list);
t_tarjan_vertex *create_tarjan_vertex_array(const t_adjacency_list *p_adjacency_list);
void print_tarjan_vertices(const t_tarjan_vertex *tarjan_vertices, int size);
t_class create_empty_class(int id);
t_partition create_empty_partition();
void add_class_to_partition(t_partition *partition, t_class new_class);
void print_t_partition (const t_partition *partition);
t_partition tarjan(const t_adjacency_list *graph);
#endif // UTILS_H
