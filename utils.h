#ifndef UTILS_H
#define UTILS_H

struct t_adjacency_list {
    int size;
    int **adjacency;
    int *adjacency_sizes;
};

void extract_from_file(const char *file_path, struct t_adjacency_list *p_adjacency_list);
void print_adjacency_list(const struct t_adjacency_list *p_adjacency_list);

#endif // UTILS_H