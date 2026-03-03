#include "symnmf.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "points_list_utils.h"
#include "matrix_utils.h"

double squared_euclidean_distance(double *a, double *b, unsigned int dim) {
    unsigned int i;
    double sum = 0, diff;

    for (i = 0; i < dim; i++) { diff = a[i] - b[i]; sum += diff * diff; }
    return sum;
}

double RBF_kernel(double *a, double *b, unsigned int dim) {
    return exp(- squared_euclidean_distance(a, b, dim) / 2.0);
}

double **generate_similarity_matrix(double **matrix, unsigned int dim, unsigned int num_of_points) {
    unsigned int i;
    unsigned int j;
    double **A;
    
    A = generate_matrix(num_of_points, num_of_points);
    if (!A) return NULL;

    for (i = 0; i < num_of_points; i++) {
        for (j = i; j < num_of_points; j++) {
            A[i][j] = A[j][i] = (i == j) ? 0 : RBF_kernel(matrix[i], matrix[j], dim);
        }
    }
    return A;
}

double **generate_diagonal_degree_matrix(double **A, unsigned int num_of_points) {
    unsigned int i;
    unsigned int j;
    double sum_line;
    double **D;

    D = generate_zero_initialized_matrix(num_of_points, num_of_points);
    if (!D) return NULL;

    for (i = 0; i < num_of_points; i++) {
        sum_line = 0;
        for (j = 0; j < num_of_points; j++) {
            sum_line += A[i][j];
        }
        D[i][i] = sum_line;
    }

    return D;
}

void inverse_square_root_diagonal_matrix(double **D, unsigned int n) {
    unsigned int i;
    
    for (i = 0; i < n ; i++) {
        double D_ii;

        D_ii = sqrt(D[i][i]);
        if (D_ii > EPSILON) D[i][i] = 1 / D_ii;
        else D[i][i] = 0;
    }
}

double **generate_normalised_adjacency_matrix(double **A, unsigned int num_of_points) {
    double **D_inverse_square_root;
    double **intermediate_matrix;
    double **W;


    D_inverse_square_root = generate_diagonal_degree_matrix(A, num_of_points);
    if (!D_inverse_square_root) { return NULL; }
    inverse_square_root_diagonal_matrix(D_inverse_square_root, num_of_points);

    intermediate_matrix = generate_matrix(num_of_points, num_of_points);
    if (!intermediate_matrix) { free_matrix(D_inverse_square_root); return NULL; }
    W = generate_matrix(num_of_points, num_of_points);
    if (!W) { free_matrix(intermediate_matrix); free_matrix(D_inverse_square_root); return NULL; }

    matrix_multiply(intermediate_matrix, D_inverse_square_root, A, num_of_points, num_of_points, num_of_points, num_of_points);
    matrix_multiply(W, intermediate_matrix, D_inverse_square_root, num_of_points, num_of_points, num_of_points, num_of_points);

    free_matrix(intermediate_matrix);
    free_matrix(D_inverse_square_root);

    return W;
}

int throw_error() {
    fprintf(stderr,"An Error Has Occurred\n");
    return 1;
}

double **ddg(double **matrix, unsigned int dim, unsigned int num_of_points) {
    double **A;
    double **D;
    
    A = generate_similarity_matrix(matrix, dim, num_of_points);
    if (!A) { return NULL; }
    D = generate_diagonal_degree_matrix(A, num_of_points);
    free_matrix(A);
    return D;
}

double **norm(double **matrix, unsigned int dim, unsigned int num_of_points) {
    double **A;
    double **W;
    
    A = generate_similarity_matrix(matrix, dim, num_of_points);
    if (!A) { return NULL; }
    W = generate_normalised_adjacency_matrix(A, num_of_points);
    free_matrix(A);
    return W;
}

int main(int argc, char *argv[]) {
    unsigned int dim;
    FILE *ifp;
    points_list *plist;
    unsigned int num_of_points;
    double **matrix;
    int error;

    if (argc != 3) { return throw_error(); }

    ifp = fopen(argv[2], "r");
    if (!ifp) { return throw_error(); }

    plist = read_points(&dim, ifp);
    if (!plist) { fclose(ifp); return throw_error(); }
    num_of_points = plist->length;

    matrix = points_to_matrix(plist, dim);
    free_points_list(plist);
    fclose(ifp);
    if (!matrix) { return throw_error(); }

    error = 0;
    if (strcmp(argv[1], "sym") == 0) {
        double **A;
        
        A = generate_similarity_matrix(matrix, dim, num_of_points);
        if (A) {
            print_matrix(A, num_of_points, num_of_points);
            free_matrix(A);
        }
        else { error = 1; }
    }
    else if (strcmp(argv[1], "ddg") == 0) {
        double **D;
        
        D = ddg(matrix, dim, num_of_points);
        if (D) { 
            print_matrix(D, num_of_points, num_of_points);
            free_matrix(D);
        }
        else { error = 1; }
    }
    else if (strcmp(argv[1], "norm") == 0) {
        double **W;
        
        W = norm(matrix, dim, num_of_points);
        if (W) {
            print_matrix(W, num_of_points, num_of_points);
            free_matrix(W);
        }
        else { error = 1; }
    }
    else { error = 1; }
    free_matrix(matrix);

    return error ? throw_error() : 0;
}