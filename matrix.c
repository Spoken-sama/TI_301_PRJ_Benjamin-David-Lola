#include "utils.h"
#include "types.h"
#include <stdlib.h>
#include <stdio.h>


float **create_matrix(t_adjacency_list *graph) {
    int size = graph->size;
    float **matrix = malloc(size * sizeof(float *));
    if (!matrix) {
        perror("Error allocating memory for matrix rows");
    }

    for (int i = 0; i < size; i++) {
        matrix[i] = calloc(size, sizeof(float));
        if (!matrix[i]) {
            perror("Error allocating memory for matrix columns");
            for (int j = 0; j < i; j++) {
                free(matrix[j]);
            }
            free(matrix);
        }
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
        for (int j = 0; j < size; j++) {
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
        for (int j = 0; j < size; j++) {
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
        for (int j = 0; j < size; j++) {
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
        for (int j = 0; j < size; j++) {
            total_diff = abs(matrix_a[i][j] - matrix_b[i][j]);
            result_matrix[i][j] = total_diff;
        }
    }
    return result_matrix;
}

float **multiply_n_times(int n,float **matrix_a, float **matrix_b) {
    int size = sizeof(matrix_a);
    float **result_matrix = create_empty_matrix(size);
    float **temp_matrix;
    temp_matrix =matrix_b;
    for (int count = 0; count < n ; count++){
        result_matrix = multiply_matrices(size, matrix_a, temp_matrix);
        temp_matrix = result_matrix;

    }
    return result_matrix;
}