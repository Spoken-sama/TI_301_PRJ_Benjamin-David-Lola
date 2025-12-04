#include <stdio.h>
#include "stdlib.h"
#include "utils.h"
#include "types.h"
#include "matrix.h"
#include "hasse.h"

void display_menu() {
    printf("\n--- Menu ---\n");
    printf("1. Load adjacency list from file\n");
    printf("2. Create Tarjan vertices\n");
    printf("3. Create partition\n");
    printf("4. Create adjacency matrix\n");
    printf("5. Create empty matrix\n");
    printf("6. Copy adjacency matrix to empty matrix\n");
    printf("7. Multiply matrices\n");
    printf("8. Multiply matrix n times\n");
    printf("9. Compute difference matrix\n");
    printf("10. Extract submatrix for a component\n");
    printf("11. Optional Part 2\n");
    printf("12. Build mermaid\n");
    printf("13. Verify Markov property\n");
    printf("14. Compute probability distribution after n steps\n");
    printf("15. Exit\n");
    printf("Enter your choice: ");
}

int main() {
    t_adjacency_list graph = {0};
    t_tarjan_vertex *vertices = NULL;
    t_partition partition = {0};
    float **adj_matrix = NULL;
    float **empty_matrix = NULL;
    float **final_matrix = NULL;
    float **multiplied_matrix = NULL;
    float **multiplied_matrix_n = NULL;
    float **diff_matrix = NULL;
    float **submatrix = NULL;

    int choice;
    char *path = NULL;

    do {
        display_menu();
        scanf("%d", &choice);

        switch (choice) {
            case 1: {
                if (path) free(path);
                printf("\n");
                print_files_in_data_directory("../data");
                printf("Please enter the row number of the file you want to use: ");
                int row;
                scanf("%d", &row);
                path = get_row_file_path("../data", row-1);
                if (!path) {
                    fprintf(stderr, "Failed to construct file path.\n");
                    break;
                }
                printf("Extracted adjacency list from file: %s\n", path);
                graph = extract_from_file(path);
                printf("\nThis is our adjacency list:\n\n");
                print_adjacency_list(&graph);
                break;
            }
            case 2: {
                if (graph.size == 0) {
                    printf("Load the adjacency list first.\n");
                    break;
                }
                if (vertices) free(vertices);
                vertices = create_tarjan_vertex_array(&graph);
                printf("\nTarjan vertices created successfully.\n");
                break;
            }
            case 3: {
                if (!vertices) {
                    printf("Create Tarjan vertices first.\n");
                    break;
                }
                if (partition.classes) free(partition.classes);
                partition = tarjan(&graph);
                printf("\nPartition created successfully.\n");
                print_t_partition(&partition);
                break;
            }
            case 4: {
                if (graph.size == 0) {
                    printf("Load the adjacency list first.\n");
                    break;
                }
                if (adj_matrix) free(adj_matrix);
                adj_matrix = create_matrix(&graph);
                printf("\nAdjacency matrix created successfully.\n");
                print_matrix(graph.size, adj_matrix);
                break;
            }
            case 5: {
                if (graph.size == 0) {
                    printf("Load the adjacency list first.\n");
                    break;
                }
                if (empty_matrix) free(empty_matrix);
                empty_matrix = create_empty_matrix(graph.size);
                printf("\nEmpty matrix created successfully.\n");
                print_matrix(graph.size, empty_matrix);
                break;
            }
            case 6: {
                if (!adj_matrix || !empty_matrix) {
                    printf("Create the adjacency matrix and empty matrix first.\n");
                    break;
                }
                float **final_matrix = fill_matrix_from_matrix(graph.size, adj_matrix, empty_matrix);
                printf("\nMatrix copied successfully.\n");
                print_matrix(graph.size, final_matrix);
                free(final_matrix); // Free the temporary matrix
                break;
            }
            case 7: {
                if (!adj_matrix) {
                    printf("Create the adjacency matrix first.\n");
                    break;
                }
                if (multiplied_matrix) free(multiplied_matrix);
                multiplied_matrix = multiply_matrices(graph.size, adj_matrix, adj_matrix);
                printf("\nMatrices multiplied successfully.\n");
                print_matrix(graph.size, multiplied_matrix);
                break;
            }
            case 8: {
                if (!adj_matrix) {
                    printf("Create the adjacency matrix first.\n");
                    break;
                }
                int n;
                printf("Enter the number of times to multiply the matrices: ");
                scanf("%d", &n);
                if (n <= 0) {
                    printf("Invalid number of multiplications.\n");
                    break;
                }

                // Nettoyage propre (utilisation de free_matrix ajoutée précédemment)
                if (multiplied_matrix_n) free_matrix(multiplied_matrix_n, graph.size);

                multiplied_matrix_n = multiply_n_times(n, graph.size, adj_matrix);

                printf("\nMatrix multiplied %d times successfully.\n", n);

                // --- CORRECTION ICI ---
                // Ne JAMAIS utiliser sizeof sur un pointeur dynamique
                print_matrix(graph.size, multiplied_matrix_n);
                break;
            }
            case 9: {
                if (!adj_matrix || !multiplied_matrix) {
                    printf("Create the adjacency matrix and multiplied matrix first.\n");
                    break;
                }
                if (diff_matrix) free_matrix(diff_matrix, graph.size);

                diff_matrix = diff(graph.size, adj_matrix, multiplied_matrix);
                printf("\nDifference matrix computed successfully.\n");
                print_matrix(sizeof diff_matrix, diff_matrix);
                break;
            }
            case 10: {
                if (!adj_matrix || !partition.classes) {
                    printf("Create the adjacency matrix and partition first.\n");
                    break;
                }
                int compo_index;
                printf("Enter the component index: ");
                scanf("%d", &compo_index);
                if (submatrix) {
                    for (int i = 0; i < partition.classes[compo_index].size; i++) {
                        free(submatrix[i]);
                    }
                    free(submatrix);
                    submatrix=NULL;
                }
                submatrix = subMatrix(adj_matrix, partition, compo_index);
                printf("\nSubmatrix for component %d extracted successfully.\n", compo_index);
                print_matrix(partition.classes[compo_index].size, submatrix);
                break;
            }
            case 11: {
                t_link_array links = build_links(&graph, &partition);
                removeTransitiveLinks(&links);
                print_hasse(&links);
                break;
            }
            case 12: { // Or whatever number is next
                if (graph.size == 0) {
                    printf("Load the adjacency list first.\n");
                    break;
                }
                generate_mermaid_file("graph.mmd", &graph);
                printf("Content ready. Open 'graph.mmd' and copy to https://www.mermaidchart.com/\n");
                break;
            }
            case 13: {
                if (graph.size == 0) {
                    printf("Load the adjacency list first.\n");
                    break;
                }

                printf("\nVerifying Markov property...\n");
                if (is_graph_markovian(&graph)) {
                    printf("Success: The graph is Markovian (all rows sum to 1.0).\n");
                } else {
                    printf("Failure: The graph is NOT Markovian.\n");
                }
                break;
            }
            case 14: {
                if (!adj_matrix) {
                    printf("Create the adjacency matrix first.\n");
                    break;
                }
                int n;
                printf("Enter the number of steps (n): ");
                scanf("%d", &n);

                float proba_vector[] = {
                        0.0, 0.2, 0.0, 0.0, 0.2,
                        0.0, 0.0, 0.0, 0.0, 0.0,  // Indices 5-9   (States 6-10)
                        0.0, 0.2, 0.0, 0.0, 0.0,  // Indices 10-14 (States 11-15)
                        0.0, 0.0, 0.0, 0.0, 0.0,  // Indices 15-19 (States 16-20)
                        0.2, 0.0, 0.0, 0.0, 0.2,  // Indices 20-24 (States 21-25)
                        0.0, 0.0                  // Indices 25-26 (States 26-27)
                };
                if (sizeof(proba_vector) / sizeof(proba_vector[0]) != graph.size) {
                    printf("Error: Hardcoded array size does not match the graph size.\n");
                    break;
                }
                compute_distribution(graph.size, adj_matrix, proba_vector, n);

                printf("\n--- Probability Distribution after %d steps ---\n", n);
                printf("[ ");
                for (int i = 0; i < graph.size; i++) {
                    printf("%.4f; ", proba_vector[i]);
                }
                printf("]\n");

                float max_p = -1.0f;
                int max_idx = -1;
                for (int i = 0; i < graph.size; i++) {
                    if (proba_vector[i] > max_p) {
                        max_p = proba_vector[i];
                        max_idx = i;
                    }
                }
                printf("Most likely location: Vertex %d (%.2f%%)\n", max_idx + 1, max_p * 100);

                break;
            }

            case 15:
                printf("Exiting program.\n");
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 9);

    if (path) free(path);
    if (vertices) free(vertices);
    if (partition.classes) free(partition.classes);
    if (adj_matrix) free(adj_matrix);
    if (empty_matrix) free(empty_matrix);
    if (multiplied_matrix) free(multiplied_matrix);
    if (multiplied_matrix_n) free(multiplied_matrix_n);
    if (diff_matrix) free(diff_matrix);
    if (submatrix) free(submatrix);
    return 0;
}