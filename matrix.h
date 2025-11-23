#ifndef INC_2526_TI301_PJT_MATRIX_H
#define INC_2526_TI301_PJT_MATRIX_H
#include "utils.h"
#include "types.h"

float **create_matrix(t_adjacency_list *graph);
float **create_empty_matrix(int size);
float **fill_matrix_from_matrix(int size, float **source_matrix, float **dest_matrix);
float **multiply_matrices(int size, float **matrix_a, float **matrix_b);
float **multiply_n_times(int n,float **matrix_a, float **matrix_b);
float **diff(float **matrix_a, float **matrix_b);
void print_matrix(int size, float **matrix);
t_matrix subMatrix(t_matrix matrix, t_partition part, int compo_index);


#endif //INC_2526_TI301_PJT_MATRIX_H
