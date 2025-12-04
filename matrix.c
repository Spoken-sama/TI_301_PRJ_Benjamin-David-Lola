#include "utils.h"
#include "types.h"
#include <stdlib.h>
#include <stdio.h>
#include "math.h"

void free_matrix(float **matrix, int size) {
    if (!matrix) return;
    for (int i = 0; i < size; i++) {
        if (matrix[i]) {
            free(matrix[i]);
        }
    }
    free(matrix);
}
// Correction : Taille exacte graph->size, pas +1
float **create_matrix(t_adjacency_list *graph) {
    int size = graph->size;

    float **matrix = (float **)malloc(size * sizeof(float *));
    for (int i = 0; i < size; i++) {
        matrix[i] = (float *)calloc(size, sizeof(float));
    }

    for (int i = 0; i < size; i++) {
        cell *current = graph->lists[i].head;
        while (current) {
            // current->vertex est maintenant 0-based grâce au correctif dans utils.c
            matrix[i][current->vertex] = current->probability;
            current = current->next;
        }
    }

    return matrix;
}

// Correction : Affichage de 0 à size-1
void print_matrix(int size, float **matrix) {
    if (!matrix) return;
    printf("    ");
    for(int j=0; j<size; j++) printf(" %02d ", j+1); // En-tête colonnes (affichage +1 pour l'humain)
    printf("\n");

    for (int i = 0; i < size; i++) {
        printf("%02d ｜", i+1); // En-tête lignes
        for (int j = 0; j < size; j++) {
            printf(" %.2f ", matrix[i][j]);
        }
        printf("｜\n");
    }
}

// Correction : Taille exacte
float **create_empty_matrix(int size) {
    float **matrix = malloc(size * sizeof(float *));
    if (!matrix) {
        perror("Error allocating memory for matrix rows");
        return NULL;
    }
    for (int i = 0; i < size; i++) {
        matrix[i] = calloc(size, sizeof(float));
        // ... (gestion erreur inchangée)
    }
    return matrix;
}

// Correction : Boucles de 0 à size
float **fill_matrix_from_matrix(int size, float **source_matrix, float **dest_matrix) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) { // Correction ici
            dest_matrix[i][j] = source_matrix[i][j];
        }
    }
    return dest_matrix;
}

// Correction : Boucles de 0 à size
float **multiply_matrices(int size, float **matrix_a, float **matrix_b) {
    float **result_matrix = create_empty_matrix(size);
    if (!result_matrix) return NULL;

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) { // Correction ici
            result_matrix[i][j] = 0.0f;
            for (int k = 0; k < size; k++) {
                result_matrix[i][j] += matrix_a[i][k] * matrix_b[k][j];
            }
        }
    }
    return result_matrix;
}

// Correction : Boucles de 0 à size
float **diff(int size, float **matrix_a, float **matrix_b) {
    float **result_matrix = create_empty_matrix(size);
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) { // Correction ici
            result_matrix[i][j] = fabs(matrix_a[i][j] - matrix_b[i][j]);
        }
    }
    return result_matrix;
}

// ... (multiply_n_times reste globalement le même, s'assurer que size est bien passé)

// La version corrigée de subMatrix mentionnée plus haut
t_matrix subMatrix(t_matrix matrix, t_partition part, int compo_index) {
    if (compo_index < 0 || compo_index >= part.size) {
        fprintf(stderr, "Invalid component index.\n");
        exit(EXIT_FAILURE);
    }

    t_class component = part.classes[compo_index];
    int sub_size = component.size;

    float **submatrix = (float **)malloc(sub_size * sizeof(float *)); // Cast explicite t_matrix -> float**
    // ... verification allocation ...

    for (int i = 0; i < sub_size; i++) {
        submatrix[i] = (float *)malloc(sub_size * sizeof(float));
        // ... verification allocation ...
    }

    for (int i = 0; i < sub_size; i++) {
        int original_row = component.vertices[i]->id; // RETRAIT DU -1
        for (int j = 0; j < sub_size; j++) {
            int original_col = component.vertices[j]->id; // RETRAIT DU -1
            submatrix[i][j] = matrix[original_row][original_col];
        }
    }

    return submatrix;
}
float **multiply_n_times(int n, int size, float **matrix_b) {
    // Créer une copie initiale de la matrice (C'est A^1)
    float **result = create_empty_matrix(size);
    fill_matrix_from_matrix(size, matrix_b, result);

    // Si n=1, on retourne juste la copie
    if (n <= 1) return result;

    // On multiplie n-1 fois supplémentaire
    // Ex: n=2 -> on fait 1 multiplication (A * A)
    for (int i = 0; i < n - 1; i++) {
        float **temp = multiply_matrices(size, matrix_b, result);

        // On libère l'ancien résultat intermédiaire
        free_matrix(result, size);

        // On met à jour
        result = temp;
    }


    return result;
}

void compute_distribution(int size, float **matrix, float *current_distribution, int n) {
    float *new_distribution = (float *)malloc(size * sizeof(float));
    if (!new_distribution) {
        perror("Error allocating memory for distribution");
        return;
    }

    for (int step = 0; step < n; step++) {
        for (int j = 0; j < size; j++) {
            new_distribution[j] = 0.0f;
            for (int i = 0; i < size; i++) {
                new_distribution[j] += current_distribution[i] * matrix[i][j];
            }
        }
        for (int i = 0; i < size; i++) {
            current_distribution[i] = new_distribution[i];
        }
    }

    free(new_distribution);
}