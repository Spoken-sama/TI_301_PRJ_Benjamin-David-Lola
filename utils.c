#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include "types.h"
#include "math.h"

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

void print_files_in_data_directory(const char *directory_path) {
    DIR *dir = opendir(directory_path);
    if (!dir) {
        perror("Error opening directory");
        return;
    }

    struct dirent *entry;
    printf("Files in '%s':\n", directory_path);
    int row = -1;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] != '.') {
            printf("%d %s\n",row, entry->d_name);
        }
        row++;
    }

    closedir(dir);
}

char *get_row_file_path(const char *directory_path, int row) {
    DIR *dir = opendir(directory_path);
    if (!dir) {
        perror("Error opening directory");
        return NULL;
    }

    struct dirent *entry;
    int current_row = -1;
    char *file_path = NULL;

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] != '.') {
            current_row++;
            if (current_row == row) {
                file_path = (char *)malloc(strlen(directory_path) + strlen(entry->d_name) + 2);
                if (!file_path) {
                    perror("Error allocating memory for file path");
                    closedir(dir);
                    return NULL;
                }
                sprintf(file_path, "%s/%s", directory_path, entry->d_name);
                break;
            }
        }
    }

    closedir(dir);
    return file_path;
}

//Graph functions

cell *create_cell(int to, float probability) {
    cell *new_cell = (cell *)malloc(sizeof(cell));
    if (new_cell == NULL) {
        perror("Error allocating memory for cell");
        return NULL;
    }
    new_cell->vertex = to;
    new_cell->probability = probability;
    new_cell->next = NULL;
    return new_cell;
}

list create_empty_list() {
    list new_list;
    new_list.head = NULL;
    return new_list;
}

void add_cell_to_list(list *lst, int to, float probability) {
    cell *new_cell = create_cell(to, probability);
    if (new_cell == NULL) {
        return;
    }
    new_cell->next = lst->head;
    lst->head = new_cell;
}

void display_list(const list *lst) {
    cell *current = lst->head;
    while (current != NULL) {
        printf("-> (%d, %.2f) ", current->vertex, current->probability);
        current = current->next;
    }
    printf("\n");
}

t_adjacency_list create_empty_adjacency_list(int size) {
    t_adjacency_list adj_list;
    adj_list.size = size;
    adj_list.lists = (list *)malloc(size * sizeof(list));
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

t_adjacency_list extract_from_file(const char *file_path) {
    FILE *file = fopen(file_path, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    int num_nodes;
    if (fscanf(file, "%d", &num_nodes) != 1) {
        perror("Error reading number of nodes");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    t_adjacency_list adjacency_list = create_empty_adjacency_list(num_nodes);

    int from, to;
    float weight;
    while (fscanf(file, "%d %d %f", &from, &to, &weight) == 3) {
        from--; // Conversion 1-based -> 0-based
        to--;   // AJOUTER CETTE LIGNE : Conversion 1-based -> 0-based
        add_cell_to_list(&adjacency_list.lists[from], to, weight);
    }

    fclose(file);
    return adjacency_list;
}

void print_adjacency_list(const t_adjacency_list *p_adjacency_list) {
    for (int i = 0; i < p_adjacency_list->size; i++) {
        printf("List for vertex %d: [head @] ", i + 1);
        display_list(&p_adjacency_list->lists[i]);
    }
}

void generate_mermaid_file(const char *filename, const t_adjacency_list *graph) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Error opening file for writing");
        return;
    }

    // 1. Print Configuration Header
    fprintf(file, "---\n");
    fprintf(file, "config:\n");
    fprintf(file, " layout: elk\n");
    fprintf(file, " theme: neo\n");
    fprintf(file, " look: neo\n");
    fprintf(file, "---\n");
    fprintf(file, "flowchart LR\n");

    // 2. Print Vertices: A((1))
    // We iterate 0 to size, but display i+1
    for (int i = 0; i < graph->size; i++) {
        char node_id[20];
        // We copy the result because getID returns a static buffer
        strcpy(node_id, getID(i + 1));

        fprintf(file, "%s((%d))\n", node_id, i + 1);
    }

    // 3. Print Edges: A -->|0.01|B
    for (int i = 0; i < graph->size; i++) {
        char src_id[20];
        strcpy(src_id, getID(i + 1)); // Save Source ID

        cell *current = graph->lists[i].head;
        while (current != NULL) {
            char dest_id[20];
            strcpy(dest_id, getID(current->vertex + 1)); // Save Dest ID

            // Print edge with weight
            // Format: SRC -->|Weight| DEST
            fprintf(file, "%s -->|%.2f|%s\n", src_id, current->probability, dest_id);

            current = current->next;
        }
    }

    fclose(file);
    printf("Mermaid file generated: %s\n", filename);
}
//Tarjan vertex functions

t_tarjan_vertex *create_tarjan_vertex_array(const t_adjacency_list *p_adjacency_list) {
    t_tarjan_vertex *tarjan_vertices = (t_tarjan_vertex *)malloc(p_adjacency_list->size * sizeof(t_tarjan_vertex));
    if (!tarjan_vertices) {
        perror("Error allocating memory for Tarjan vertices");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < p_adjacency_list->size; i++) {
        tarjan_vertices[i].id = i;
        tarjan_vertices[i].tmp_numbering = -1;
        tarjan_vertices[i].low_link = -1;
        tarjan_vertices[i].on_stack = 0;
        tarjan_vertices[i].grp_id = -1;
    }
    return tarjan_vertices;
}

#define EPSILON 0.001f // Marge de tolérance pour les comparaisons de float

int is_graph_markovian(const t_adjacency_list *graph) {
    if (!graph || graph->size == 0) {
        printf("Error: Graph is empty or NULL.\n");
        return 0;
    }

    for (int i = 0; i < graph->size; i++) {
        float sum = 0.0f;
        cell *current = graph->lists[i].head;

        // Somme des probabilités des arcs sortants
        while (current != NULL) {
            sum += current->probability;
            current = current->next;
        }

        // Vérification : |somme - 1.0| > epsilon
        // Si la différence est trop grande, ce n'est pas markovien
        if (fabs(sum - 1.0f) > EPSILON) {
            // On affiche le sommet coupable pour aider au debug
            // On utilise i+1 car vos sommets s'affichent généralement en 1-based
            printf("Graph is NOT Markovian. Vertex %d has outgoing sum: %.4f\n", i + 1, sum);
            return 0; // Faux
        }
    }

    return 1; // Vrai
}




//Partition functions

t_class create_empty_class(int id) {
    t_class new_class;
    new_class.id = id;
    new_class.vertices = NULL;
    new_class.size = 0;
    return new_class;
}

t_partition create_empty_partition() {
    t_partition partition;
    partition.size = 0;
    partition.classes = NULL; // Initialiser à NULL pour éviter des comportements indéfinis
    return partition;
}

void free_partition(t_partition *partition) {
    for (int i = 0; i < partition->size; i++) {
        free(partition->classes[i].vertices); // Libérer les sommets de chaque classe
    }
    free(partition->classes); // Libérer le tableau de classes
    partition->classes = NULL;
    partition->size = 0;
}

void add_class_to_partition(t_partition *partition, t_class new_class) {
    partition->classes = realloc(partition->classes, (partition->size + 1) * sizeof(t_class));
    if (!partition->classes) {
        perror("Error reallocating memory for partition classes");
        exit(EXIT_FAILURE);
    }
    partition->classes[partition->size] = new_class;
    partition->size++;
}

t_stack *create_stack(int size) {
    t_stack *s = malloc(sizeof(t_stack));
    s->items = malloc(size * sizeof(t_tarjan_vertex *));
    s->top = 0;
    s->capacity = size;
    return s;
}

void push_stack(t_stack *s, t_tarjan_vertex *v) {
    if (s->top >= s->capacity) {
        perror("Stack overflow");
        exit(EXIT_FAILURE);
    }
    s->items[s->top++] = v;
}

t_tarjan_vertex *pop_stack(t_stack *s) {
    if (s->top == 0) return NULL;
    return s->items[--s->top];
}

void parcours(int v, const t_adjacency_list *graph, t_tarjan_vertex *vertices,
              t_stack *stack, t_partition *partition, int *index_counter) {
    vertices[v].tmp_numbering = *index_counter;
    vertices[v].low_link = *index_counter;
    (*index_counter)++;

    push_stack(stack, &vertices[v]);
    vertices[v].on_stack = 1;

    for (cell *c = graph->lists[v].head; c; c = c->next) {
        int w = c->vertex;

        if (vertices[w].tmp_numbering == -1) {
            parcours(w, graph, vertices, stack, partition, index_counter);

            if (vertices[w].low_link < vertices[v].low_link)
                vertices[v].low_link = vertices[w].low_link;
        } else if (vertices[w].on_stack) {
            if (vertices[w].tmp_numbering < vertices[v].low_link)
                vertices[v].low_link = vertices[w].tmp_numbering;
        }
    }

    if (vertices[v].low_link == vertices[v].tmp_numbering) {
        t_class new_class = create_empty_class(partition->size + 1);
        t_tarjan_vertex *w_ptr;

        do {
            w_ptr = pop_stack(stack);
            w_ptr->on_stack = 0;
            w_ptr->grp_id = new_class.id;

            new_class.size++;
            new_class.vertices = realloc(new_class.vertices, new_class.size * sizeof(t_tarjan_vertex *));
            if (!new_class.vertices) {
                perror("Error reallocating memory for class vertices");
                exit(EXIT_FAILURE);
            }
            new_class.vertices[new_class.size - 1] = w_ptr;
        } while (w_ptr != &vertices[v]);

        add_class_to_partition(partition, new_class);
    }
}

t_partition tarjan(const t_adjacency_list *graph) {
    t_partition partition = create_empty_partition();
    t_tarjan_vertex *vertices = create_tarjan_vertex_array(graph);
    t_stack *stack = create_stack(graph->size);
    int index_counter = 0;

    for (int v = 0; v < graph->size; v++) {
        if (vertices[v].tmp_numbering == -1) {
            parcours(v, graph, vertices, stack, &partition, &index_counter);
        }
    }

    return partition;
}

void print_t_partition(const t_partition *partition) {
    for (int i = 0; i < partition->size; i++) {
        printf("Component C%d: Size: %d, Vertices: {", partition->classes[i].id, partition->classes[i].size);
        for (int j = 0; j < partition->classes[i].size; j++) {
            printf("%d", partition->classes[i].vertices[j]->id + 1); // Ajuster l'indexation
            if (j < partition->classes[i].size - 1) printf(", ");
        }
        printf("}\n");
    }
}



// Optional Part 2

t_link_array create_empty_link_array() {
    t_link_array arr;
    arr.links = malloc(sizeof(t_link) * 4);
    arr.size = 0;
    arr.log_size = 4;
    return arr;
}

void add_link(t_link_array *link_array, int from, int to) {
    if (link_array->size >= link_array->log_size) {
        link_array->log_size *= 2;
        link_array->links = realloc(link_array->links, link_array->log_size * sizeof(t_link));
        if (!link_array->links) {
            perror("Error reallocating memory for links");
            exit(EXIT_FAILURE);
        }
    }
    link_array->links[link_array->size].from = from;
    link_array->links[link_array->size].to = to;
    link_array->size++;
}

int link_exists(t_link_array *link_array, int from, int to) {
    for (int i = 0; i < link_array->size; i++) {
        if (link_array->links[i].from == from && link_array->links[i].to == to)
            return 1;
    }
    return 0;
}

t_link_array build_links(const t_adjacency_list *graph, const t_partition *partition) {
    t_link_array link_array = create_empty_link_array();
    int *class_of_vertex = malloc(graph->size * sizeof(int));
    if (!class_of_vertex) {
        perror("Error allocating memory for class_of_vertex");
        exit(EXIT_FAILURE);
    }

    // Associer chaque sommet à sa classe
    for (int i = 0; i < partition->size; i++) {
        for (int j = 0; j < partition->classes[i].size; j++) {
            int v = partition->classes[i].vertices[j]->id;
            class_of_vertex[v] = partition->classes[i].id;
        }
    }

    // Construire les liens entre les classes
    for (int v = 0; v < graph->size; v++) {
        int Ci = class_of_vertex[v];
        for (cell *c = graph->lists[v].head; c; c = c->next) {
            int w = c->vertex;
            int Cj = class_of_vertex[w];
            if (Ci != Cj && !link_exists(&link_array, Ci, Cj)) {
                add_link(&link_array, Ci, Cj);
            }
        }
    }

    free(class_of_vertex);
    return link_array;
}
void free_link_array(t_link_array *link_array) {
    free(link_array->links);
    link_array->links = NULL;
    link_array->size = 0;
    link_array->log_size = 0;
}
void print_hasse(const t_link_array *link_array) {
    printf("graph TD\n");
    for (int i = 0; i < link_array->size; i++) {
        printf("C%d --> C%d\n", link_array->links[i].from, link_array->links[i].to);
    }
}