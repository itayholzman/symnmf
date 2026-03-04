"""
SymNMF Python Interface Script

This script provides a Python interface for performing symmetric non-negative 
matrix factorization (SymNMF) and related matrix operations using the C extension 
`symnmf_capi`. It supports computing similarity matrices, diagonal degree matrices, 
normalized adjacency matrices, and running the SymNMF algorithm on input data.

Usage:
    python3 ./symnmf.py <k> <goal> <input_file>

Arguments:
    k           Number of clusters (num_of_points > integer > 1)
    goal        Task to perform: one of ["symnmf", "sym", "ddg", "norm"]
    input_file  CSV file containing the dataset (no headers, comma-separated floats)
"""

import symnmf_capi
import numpy as np
import pandas as pd
import sys

ERROR_MESSAGE = "An Error Has Occurred"

EPSILON = 1e-4
MAX_ITER = 300
BETA = 0.5

VALID_GOALS = ["symnmf", "sym", "ddg", "norm"]

np.random.seed(1234)


def load_points_to_matrix(filename: str) -> np.ndarray | None:
    """
    Load data points from a CSV file into a NumPy matrix.

    Parameters
    ----------
    filename : str
        Path to the CSV input file.

    Returns
    -------
    np.ndarray | None
        A 2D NumPy array of floats if successful, None on error.
    """
    try:
        df = pd.read_csv(filename, header=None)
    except Exception:
        return None

    # Check for NaNs (missing values)
    if df.isnull().values.any():
        return None

    return df.to_numpy(dtype=float)


def validate_arguments(args: list[str]) -> tuple[int, str, np.ndarray]:
    """
    Validate command-line arguments and input data.

    Parameters
    ----------
    args : list[str]
        List of command-line arguments.

    Returns
    -------
    tuple[int, str, np.ndarray]
        k (number of clusters), goal (task), and data_points (NumPy array).
    
    Exits with ERROR_MESSAGE if validation fails.
    """
    if len(args) != 4:
        print(ERROR_MESSAGE)
        sys.exit(1)
    
    try:
        k = int(args[1])
    except ValueError:
        print(ERROR_MESSAGE)
        sys.exit(1)

    if k <= 1:
        print(ERROR_MESSAGE)
        sys.exit(1)

    goal = args[2]
    if goal not in VALID_GOALS:
        print(ERROR_MESSAGE)
        sys.exit(1)

    data_points = load_points_to_matrix(args[3])
    if data_points is None:
        print(ERROR_MESSAGE)
        sys.exit(1)

    num_of_points = data_points.shape[0]
    if num_of_points <= k:
        print(ERROR_MESSAGE)
        sys.exit(1)

    return k, goal, data_points


def squared_frobenius_norm(matrix: np.ndarray) -> float:
    """
    Compute the squared Frobenius norm of a matrix.

    Parameters
    ----------
    matrix : np.ndarray
        Input matrix.

    Returns
    -------
    float
        Squared Frobenius norm (sum of squares of all entries).
    """
    return np.sum(matrix ** 2)


def symnmf(data_points, k):
    """
    Perform Symmetric Non-negative Matrix Factorization (SymNMF) on a dataset.

    Parameters
    ----------
    data_points : np.ndarray
        Input data points as a 2D NumPy array.
    k : int
        Number of clusters.

    Returns
    -------
    np.ndarray
        Factor matrix H of shape (num_points, k).
    """
    def initialize_H(W, k):
        """Randomly initialize the factor matrix H based on similarity matrix W."""
        m = np.mean(W)
        upper = 2 * np.sqrt(m / k)
        n = W.shape[0]
        H = np.random.uniform(0.0, upper, size=(n, k))
        return H
    
    def symnmf_update(W, H):
        """Perform a single multiplicative update for H."""
        WH = W @ H
        denominator = H @ (H.T @ H)
        denominator = np.maximum(denominator, 1e-13)

        ratio = WH / denominator
        H_new = H * ((1 - BETA) + BETA * ratio)

        return H_new
    
    data_list = data_points.tolist()
    W = symnmf_capi.norm(data_list)
    W = np.array(W, dtype=float)

    H = initialize_H(W, k)

    for _ in range(MAX_ITER):
        H_new = symnmf_update(W, H)

        delta_H_squared_frobenius_norm = squared_frobenius_norm(H_new - H)
        H = H_new

        if delta_H_squared_frobenius_norm < EPSILON:
            break
    
    return H


def derive_clustering(H):
    """
    Derive cluster labels from the factor matrix H.

    Parameters
    ----------
    H : np.ndarray
        Factor matrix from SymNMF.

    Returns
    -------
    np.ndarray
        Array of cluster labels for each data point.
    """
    return np.argmax(H, axis=1)


def print_matrix(matrix):
    for row in matrix:
        print(",".join(f"{value:.4f}" for value in row))


def main():
    k, goal, data_points = validate_arguments(sys.argv)

    data_list = data_points.tolist()

    if goal == "sym":
        A = symnmf_capi.sym(data_list)
        print_matrix(A)
    elif goal == "ddg":
        D = symnmf_capi.ddg(data_list)
        print_matrix(D)
    elif goal == "norm":
        W = symnmf_capi.norm(data_list)
        print_matrix(W)
    elif goal == "symnmf":
        H = symnmf(data_points, k)
        print_matrix(H)

if __name__ == '__main__':
    main()