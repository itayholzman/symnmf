#ifndef MATRIX_UTILS_H
#define MATRIX_UTILS_H

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

double **generate_matrix(unsigned int rows, unsigned int columns);

double **generate_zero_initialized_matrix(unsigned int rows, unsigned int columns);

void free_matrix(double** matrix);

void matrix_multiply(double **destination, double **matrix1, double **matrix2, unsigned int rows1, unsigned int columns1, unsigned int rows2, unsigned int columns2);

void print_matrix(double **matrix, unsigned int rows, unsigned int columns);

#endif