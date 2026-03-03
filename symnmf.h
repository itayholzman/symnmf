#ifndef SYMNMF
#define SYMNMF

#define EPSILON 1e-4

double squared_euclidean_distance(double *a, double *b, unsigned int dim);

double RBF_kernel(double *a, double *b, unsigned int dim);

double **generate_similarity_matrix(double **matrix, unsigned int dim, unsigned int num_of_points);

double **generate_diagonal_degree_matrix(double **matrix, unsigned int num_of_points);

void inverse_square_root_diagonal_matrix(double **matrix, unsigned int n);

double **generate_normalised_adjacency_matrix(double **A, unsigned int num_of_points);

double **ddg(double **matrix, unsigned int dim, unsigned int num_of_points);

double **norm(double **matrix, unsigned int dim, unsigned int num_of_points);

#endif