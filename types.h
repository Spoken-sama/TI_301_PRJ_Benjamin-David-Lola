#ifndef TYPES_H
#define TYPES_H

typedef struct s_cell {
    int vertex;
    float probability;
    struct s_cell *next;
} cell;

typedef struct s_list {
    cell *head;
} list;

typedef struct s_adjacency_list {
    list *lists;
    int size;
} t_adjacency_list;

typedef struct s_t_tarjan_vertex {
    int id;
    int tmp_numbering;
    int low_link;
    int on_stack;
    int grp_id;
} t_tarjan_vertex;

struct s_class;

typedef struct s_class {
    int id;
    t_tarjan_vertex **vertices;
    int size;
} t_class;

typedef struct {
    t_class *classes;
    int size;
} t_partition;

typedef struct {
    t_tarjan_vertex **items;
    int top;
    int capacity;
} t_stack;

typedef struct s_t_link {
    int from;
    int to;
} t_link;

typedef struct s_t_link_array {
    t_link *links;
    int size;
    int log_size;
} t_link_array;
#endif
