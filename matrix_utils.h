/**
 * @file matrix_utils.h
 * @brief Simple matrix utility functions: allocation, zero-initialization, multiplication, printing, and deallocation.
 */

#ifndef MATRIX_UTILS_H
#define MATRIX_UTILS_H

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

/**
 * @brief Allocate a matrix of doubles with given dimensions.
 * 
 * @param rows Number of rows.
 * @param columns Number of columns.
 * @return Pointer to the allocated matrix, or NULL on failure. 
 */
double **generate_matrix(unsigned int rows, unsigned int columns);

/**
 * @brief Allocate a zero-initialized matrix of doubles with given dimensions.
 * 
 * @param rows Number of rows.
 * @param columns Number of columns.
 * @return Pointer to the allocated zero-initialized matrix, or NULL on failure.
 */
double **generate_zero_initialized_matrix(unsigned int rows, unsigned int columns);

/**
 * @brief Free a previously allocated matrix.
 * 
 * @param matrix Pointer to the matrix to free.
 */
void free_matrix(double** matrix);

/**
 * @brief Multiply two matrices and store the result in a destination matrix.
 * 
 * @param destination Pre-allocated matrix to store the result.
 * @param matrix1 First input matrix.
 * @param matrix2 Second input matrix.
 * @param rows1 Number of rows in matrix1.
 * @param columns1 Number of columns in matrix1.
 * @param rows2 Number of rows in matrix2.
 * @param columns2 Number of columns in matrix2.
 */
void matrix_multiply(double **destination, double **matrix1, double **matrix2, unsigned int rows1, unsigned int columns1, unsigned int rows2, unsigned int columns2);

/**
 * @brief Print a matrix to standard output with 4 decimal places.
 * 
 * @param matrix Matrix to print.
 * @param rows Number of rows.
 * @param columns Number of columns.
 */
void print_matrix(double **matrix, unsigned int rows, unsigned int columns);

#endif