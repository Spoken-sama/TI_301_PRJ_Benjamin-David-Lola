#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"


static char *getID(int i)
{
    // translate from 1,2,3, .. ,500+ vertex A,B,C,..,Z,AA,AB,...
    static char buffer[10];
    char temp[10];
    int index = 0;

    i--; // Adjust vertex 0-based index
    while (i >= 0)
    {
        temp[index++] = 'A' + (i % 26);
        i = (i / 26) - 1;
    }

    // Reverse the string vertex get the correct order
    for (int j = 0; j < index; j++)
    {
        buffer[j] = temp[index - j - 1];
    }
    buffer[index] = '\0';

    return buffer;
}
struct cell {
    int vertex;
    float probability;
    struct cell *next;
};
struct list {
    struct cell *head;
};

struct t_adjacency_list {
    struct list *lists;
    int size;
};

struct cell *create_cell(int to, float probability) {
    struct cell *new_cell = (struct cell *)malloc(sizeof(struct cell));
    if (new_cell == NULL) {
        perror("Error allocating memory for cell");
        return NULL;
    }
    new_cell->vertex = to;
    new_cell->probability = probability;
    new_cell->next = NULL;
    return new_cell;
}

struct list create_empty_list() {
    struct list new_list;
    new_list.head = NULL;
    return new_list;
}

void add_cell_to_list(struct list *lst, int to, float probability) {
    struct cell *new_cell = create_cell(to, probability);
    if (new_cell == NULL) {
        return;
    }
    new_cell->next = lst->head;
    lst->head = new_cell;
}

void display_list(const struct list *lst) {
    struct cell *current = lst->head;
    while (current != NULL) {
        printf("-> (%d, %.2f) ", current->vertex, current->probability);
        current = current->next;
    }
    printf("\n");
}

struct t_adjacency_list create_empty_adjacency_list(int size) {
    struct t_adjacency_list adj_list;
    adj_list.size = size;
    adj_list.lists = (struct list *)malloc(size * sizeof(struct list));
    if (adj_list.lists == NULL) {
        perror("Error allocating memory for adjacency list");
        adj_list.size = 0;
        return adj_list;
    }
    for (int i = 0; i < size; i++) {
        adj_list.lists[i] = create_empty_list();
    }
    return adj_list;
}

void extract_from_file(const char *file_path, struct t_adjacency_list *p_adjacency_list) {
    FILE *file = fopen(file_path, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    int num_nodes;
    if (fscanf(file, "%d", &num_nodes) != 1) {
        perror("Error reading number of nodes");
        fclose(file);
        return;
    }

    *p_adjacency_list = create_empty_adjacency_list(num_nodes);

    int from, to;
    float weight;
    while (fscanf(file, "%d %d %f", &from, &to, &weight) == 3) {
        from--;
        to--;
        add_cell_to_list(&p_adjacency_list->lists[from], to, weight);
    }

    fclose(file);
}

void print_adjacency_list(const struct t_adjacency_list *p_adjacency_list) {
    for (int i = 0; i < p_adjacency_list->size; i++) {
        printf("List for vertex %d: [head @] ", i + 1);
        display_list(&p_adjacency_list->lists[i]);
    }
}