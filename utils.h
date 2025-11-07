#ifndef UTILS_H
#define UTILS_H

struct t_adjacency_list {
    int size;
    int **adjacency;
    int *adjacency_sizes;
};

void add_cell_to_list(struct list *lst, int to, float probability);
void display_list(const struct list *lst);
void extract_from_file(const char *file_path, struct t_adjacency_list *p_adjacency_list);
void print_adjacency_list(const struct t_adjacency_list *p_adjacency_list) ;

#endif // UTILS_H