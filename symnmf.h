/**
 * @file symnmf.h
 * @brief Symmetric Non-negative Matrix Factorization (SymNMF) utility functions.
 * 
 * Provides functions for distance calculations, similarity matrices,
 * degree matrices, normalization, and related operations.
 */

#ifndef SYMNMF
#define SYMNMF

#define EPSILON 1e-4

/**
 * @brief Compute the squared Euclidean distance between two vectors.
 * 
 * @param a Pointer to the first vector.
 * @param b Pointer to the second vector.
 * @param dim Dimensionality of the vectors.
 * @return Squared Euclidean distance.
 */
double squared_euclidean_distance(double *a, double *b, unsigned int dim);

/**
 * @brief Compute the Radial Basis Function (RBF) kernel between two vectors.
 * 
 * @param a Pointer to the first vector.
 * @param b Pointer to the second vector.
 * @param dim Dimensionality of the vectors.
 * @return RBF kernel value.
 */
double RBF_kernel(double *a, double *b, unsigned int dim);

/**
 * @brief Generate a similarity matrix using pairwise RBF kernel values.
 * 
 * @param matrix Input data matrix (num_of_points x dim).
 * @param dim Dimensionality of each data point.
 * @param num_of_points Number of data points.
 * @return Pointer to the generated similarity matrix.
 */
double **generate_similarity_matrix(double **matrix, unsigned int dim, unsigned int num_of_points);

/**
 * @brief Generate a diagonal degree matrix from a similarity matrix.
 * 
 * @param matrix Input similarity matrix (num_of_points x num_of_points).
 * @param num_of_points Number of points (matrix size).
 * @return Pointer to the diagonal degree matrix, or NULL on failure.
 */
double **generate_diagonal_degree_matrix(double **matrix, unsigned int num_of_points);

/**
 * @brief Replace each diagonal entry of a matrix with its inverse square root.
 * 
 * @param matrix Diagonal matrix to modify.
 * @param n Dimension of the square matrix.
 */
void inverse_square_root_diagonal_matrix(double **matrix, unsigned int n);

/**
 * @brief Generate a normalized adjacency matrix: D^(-1/2) * A * D^(-1/2).
 * 
 * @param A Input adjacency or similarity matrix (num_of_points x num_of_points).
 * @param num_of_points Number of points (matrix size).
 * @return Pointer to the normalized adjacency matrix, or NULL on failure.
 */
double **generate_normalised_adjacency_matrix(double **A, unsigned int num_of_points);

/**
 * @brief Compute the degree matrix D from a data matrix.
 *
 * @param matrix Data matrix (num_of_points x dim).
 * @param dim Dimensionality of each point.
 * @param num_of_points Number of points.
 * @return Pointer to allocated degree matrix, or NULL on failure.
 */
double **ddg(double **matrix, unsigned int dim, unsigned int num_of_points);

/**
 * @brief Compute the normalized adjacency matrix from a data matrix.
 *
 * @param matrix Data matrix (num_of_points x dim).
 * @param dim Dimensionality of each point.
 * @param num_of_points Number of points.
 * @return Pointer to allocated normalized adjacency matrix, or NULL on failure.
 */
double **norm(double **matrix, unsigned int dim, unsigned int num_of_points);

#endif