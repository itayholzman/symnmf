#include "matrix_utils.h"

double **generate_matrix(unsigned int rows, unsigned int columns) {
    unsigned int i;
    double *data;
    double **matrix;

    data = (double *) malloc(rows * columns * sizeof(double));
    if (!data) return NULL;

    matrix = (double **) malloc(rows * sizeof(double *));
    if(!matrix) { free(data); return NULL; }

    for (i = 0; i < rows; i++) {
        matrix[i] = data + i * columns;
    }

    return matrix;
}

double **generate_zero_initialized_matrix(unsigned int rows, unsigned int columns) {
    unsigned int i;
    unsigned int j;
    double **matrix;
    
    matrix = generate_matrix(rows, columns);
    if (!matrix) return NULL;

    for (i = 0; i < rows; i++) {
        for (j = 0; j < columns; j++) {
            matrix[i][j] = 0;
        }
    }

    return matrix;
}

void free_matrix(double** matrix) {
    free(matrix[0]);
    free(matrix);
}

void matrix_multiply(double **destination, double **matrix1, double **matrix2, unsigned int rows1, unsigned int columns1, unsigned int rows2, unsigned int columns2) {
    unsigned int i;
    unsigned int j;
    unsigned int k;
    double product;

    if (columns1 != rows2) return;

    for (i = 0; i < rows1; i++) {
        for (j = 0; j < columns2; j++) {
            product = 0;
            for (k = 0; k < columns1; k++) {
                product += matrix1[i][k] * matrix2[k][j];
            }
            destination[i][j] = product;
        }
    }
}

void print_matrix(double **matrix, unsigned int rows, unsigned int columns) {
    unsigned int i;
    unsigned int j;

    for (i = 0; i < rows; i++) {
        for (j = 0; j < columns; j++) {
            printf("%.4f ", matrix[i][j]);
        }
        printf("\n");
    }
}