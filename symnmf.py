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

def load_points_to_matrix(filename):
    try:
        df = pd.read_csv(filename, header=None)
    except Exception:
        return None

    # Check for NaNs (missing values)
    if df.isnull().values.any():
        return None

    return df.to_numpy(dtype=float)

def validate_arguments(args):
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

def squared_frobenius_norm(matrix):
    return np.sum(matrix ** 2)

def symnmf(data_points, k):
    def initialize_H(W, k):
        m = np.mean(W)
        upper = 2 * np.sqrt(m / k)
        n = W.shape[0]
        H = np.random.uniform(0.0, upper, size=(n, k))
        return H
    
    def symnmf_update(W, H):
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

import numpy as np

def derive_clustering(H):
    labels = np.argmax(H, axis=1)
    return labels

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