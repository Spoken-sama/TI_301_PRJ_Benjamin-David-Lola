#include "utils.h"
#include "types.h"
#include <stdlib.h>
#include <stdio.h>
#include "math.h"

float **create_matrix(t_adjacency_list *graph) {
    int size = graph->size;

    float **matrix = (float **)malloc(size * sizeof(float *));
    for (int i = 0; i < size; i++) {
        matrix[i] = (float *)calloc(size, sizeof(float));
    }

    for (int i = 0; i < size; i++) {
        cell *current = graph->lists[i].head;
        while (current) {
            matrix[i][current->vertex] = current->probability;
            current = current->next;
        }
    }

    return matrix;
}

void print_matrix(int size, float **matrix) {
    for (int i = 0; i < size; i++) {
        printf("｜");
        for (int j = 1; j < size+1; j++) {
            printf(" %.2f ", matrix[i][j]);
        }
        printf("｜\n");
    }
}

float **create_empty_matrix(int size) {
    float **matrix = malloc(size * sizeof(float *));
    if (!matrix) {
        perror("Error allocating memory for matrix rows");
        return NULL;
    }
    for (int i = 0; i < size; i++) {
        matrix[i] = calloc(size, sizeof(float));
        if (!matrix[i]) {
            perror("Error allocating memory for matrix columns");
            for (int j = 0; j < i; j++) {
                free(matrix[j]);
            }
            free(matrix);
            return NULL;
        }
    }
    return matrix;
}

float **fill_matrix_from_matrix(int size, float **source_matrix, float **dest_matrix) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size+1; j++) {
            dest_matrix[i][j] = source_matrix[i][j];
        }
    }
    return dest_matrix;
}

float **multiply_matrices(int size, float **matrix_a, float **matrix_b) {
    float **result_matrix = create_empty_matrix(size);
    if (!result_matrix) {
        return NULL;
    }
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size+1; j++) {
            result_matrix[i][j] = 0.0f;
            for (int k = 0; k < size; k++) {
                result_matrix[i][j] += matrix_a[i][k] * matrix_b[k][j];
            }
        }
    }
    return result_matrix;
}

float **diff(float **matrix_a, float **matrix_b) {
    int size = sizeof(matrix_a);
    float **result_matrix = create_empty_matrix(size);
    float total_diff = 0.0f;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size+1; j++) {
            total_diff = fabs(matrix_a[i][j] - matrix_b[i][j]);
            result_matrix[i][j] = total_diff;
        }
    }
    return result_matrix;
}

float **multiply_n_times(int n, int size, float **matrix_a, float **matrix_b) {
    float **temp_matrix = create_empty_matrix(size);
    if (!temp_matrix) {
        return NULL;
    }

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            temp_matrix[i][j] = matrix_b[i][j];
        }
    }

    for (int count = 0; count < n; count++) {
        float **new_result = multiply_matrices(size, matrix_a, temp_matrix);
        if (!new_result) {
            perror("Error allocating memory for matrix multiplication");
            for (int i = 0; i < size; i++) {
                free(temp_matrix[i]);
            }
            free(temp_matrix);
            return NULL;
        }

        for (int i = 0; i < size; i++) {
            free(temp_matrix[i]);
        }
        free(temp_matrix);
        temp_matrix = new_result;
    }

    return temp_matrix;
}

t_matrix subMatrix(t_matrix matrix, t_partition part, int compo_index) {
    if (compo_index < 0 || compo_index >= part.size) {
        fprintf(stderr, "Invalid component index.\n");
        exit(EXIT_FAILURE);
    }

    t_class component = part.classes[compo_index];
    int sub_size = component.size;

    t_matrix submatrix = (t_matrix)malloc(sub_size * sizeof(float *));
    if (!submatrix) {
        perror("Error allocating memory for submatrix");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < sub_size; i++) {
        submatrix[i] = (float *)malloc(sub_size * sizeof(float));
        if (!submatrix[i]) {
            perror("Error allocating memory for submatrix rows");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < sub_size; i++) {
        int original_row = component.vertices[i]->id;
        for (int j = 0; j < sub_size; j++) {
            int original_col = component.vertices[j]->id;
            submatrix[i][j] = matrix[original_row][original_col];
        }
    }

    return submatrix;
}

