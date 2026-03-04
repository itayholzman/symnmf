"""
K-Means Clustering Module.

This module provides a complete implementation of the K-means clustering
algorithm, including:

    - Cluster assignment
    - Centroid recomputation
    - Convergence detection
    - Input parsing and validation utilities

Input data points are expected as comma-separated floating-point values
provided via standard input.

Any malformed input causes the program to print "An Error Has Occurred" and terminate.
"""

import sys
import numpy as np


def derive_clustering(data_points: list[list[float]], centroids: list[list[float]]) -> list[int]:
    """
    Compute cluster labels for each data point.

    Each data point is assigned to the centroid with the smallest Euclidean distance.

    Parameters
    ----------
    data_points : list[list[float]]
        Dataset of N data points in R^d.
    centroids : list[list[float]]
        List of k centroid vectors in R^d.

    Returns
    -------
    list[int]
        A list of integers where each entry represents the index of
        the closest centroid for the corresponding data point.
    """
    labels = []
    for point in data_points:
        min_dist = float('inf')
        label = 0
        for i, centroid in enumerate(centroids):
            dist = np.linalg.norm(np.array(point) - np.array(centroid))
            if dist < min_dist:
                min_dist = dist
                label = i
        labels.append(label)
    return labels


def euclidean_distance(point1: list[float], point2: list[float]) -> float:
    """
    Compute the Euclidean distance between two vectors.

    Parameters
    ----------
    point1 : list[float]
        First vector.
    point2 : list[float]
        Second vector.
    
    Returns
    -------
    float
        Euclidean distance between point1 and point2.
    """
    distance_squared = 0.0
    for i in range(len(point1)):
        distance_squared += (point1[i] - point2[i]) ** 2
    return distance_squared ** 0.5


def assign_to_clusters(data_points: list[list[float]], centroids: list[list[float]]) -> list[list[list[float]]]:
    """
    Assign each data point to its nearest centroid.

    Parameters
    ----------
    data_points : list[list[float]]
        Dataset of N data points.
    centroids : list[list[float]]
        Current centroid positions.

    Returns
    -------
    list[list[list[float]]]
        A list of k clusters, where each cluster contains
        the data points assigned to its centroid.
    """
    clusters = [[] for _ in range(len(centroids))]
    
    for point in data_points:
        min_distance = float('inf')
        closest_cluster = 0
        
        for i in range(len(centroids)):
            distance = euclidean_distance(point, centroids[i])
            if distance < min_distance:
                min_distance = distance
                closest_cluster = i
        
        clusters[closest_cluster].append(point)
    
    return clusters


def update_centroids(clusters: list[list[list[float]]], dimension: int) -> list[list[float]]:
    """
    Recompute centroids as the arithmetic mean of cluster members.

    Empty clusters are assigned a zero vector of the given dimension.

    Parameters
    ----------
    clusters : list[list[list[float]]]
        Current cluster assignments.
    dimension : int
        Dimensionality of each data point.

    Returns
    -------
    list[list[float]]
        Updated centroid vectors.
    """
    new_centroids = []
    
    for cluster in clusters:
        if len(cluster) == 0:
            new_centroids.append([0.0] * dimension)
        else:
            centroid = [0.0] * dimension
            for point in cluster:
                for i in range(dimension):
                    centroid[i] += point[i]
            
            for i in range(dimension):
                centroid[i] /= len(cluster)
            
            new_centroids.append(centroid)
    
    return new_centroids

def has_converged(old_centroids: list[list[float]], new_centroids: list[list[float]], epsilon: float) -> bool:
    """
    Determine whether centroid updates fall below the convergence threshold.

    Convergence is achieved when the Euclidean distance between each
    old centroid and its updated counterpart is strictly less than epsilon.

    Parameters
    ----------
    old_centroids : list[list[float]]
        Centroids from the previous iteration.
    new_centroids : list[list[float]]
        Centroids from the current iteration.
    epsilon : float
        Convergence tolerance.

    Returns
    -------
    bool
        True if convergence is reached; otherwise False.
    """
    for i in range(len(old_centroids)):
        distance = euclidean_distance(old_centroids[i], new_centroids[i])
        if distance >= epsilon:
            return False
    return True

def kmeans(data_points: list[list[float]], k: int, max_iter: int, epsilon: float) -> list[list[float]]:
    """
    Execute the K-means clustering algorithm.

    The first k data points are selected as initial centroids.
    The algorithm iteratively:

        1. Assigns data points to the nearest centroid.
        2. Recomputes centroids as cluster means.
        3. Checks for convergence.

    Iteration stops when convergence is achieved or when
    max_iter iterations have been performed.

    Parameters
    ----------
    data_points : list[list[float]]
        Dataset of N data points in R^d.
    k : int
        Number of clusters.
    max_iter : int
        Maximum number of iterations.
    epsilon : float
        Convergence tolerance.

    Returns
    -------
    list[list[float]]
        Final centroid positions.
    """
    dimension = len(data_points[0])
    
    centroids = [data_points[i][:] for i in range(k)]
    
    for _ in range(max_iter):
        clusters = assign_to_clusters(data_points, centroids)
        new_centroids = update_centroids(clusters, dimension)
        
        if has_converged(centroids, new_centroids, epsilon):
            centroids = new_centroids
            break
        
        centroids = new_centroids
    
    return centroids

def read_input() -> list[list[float]]:
    """
    Read and validate data points from standard input.

    Each input line must contain comma-separated floating-point values.
    All data points must share identical dimensionality.

    On validation failure, prints:
        "An Error Has Occurred"
    and terminates the program.

    Returns
    -------
    list[list[float]]
        Validated dataset.
    """
    data_points = []
    dimension = None
    
    for line in sys.stdin:
        line = line.strip()
        if not line:
            continue
        
        try:
            values = line.split(',')
            point = []
            
            for val_str in values:
                val_str = val_str.strip()
                if not val_str:  # Empty value
                    print("An Error Has Occurred")
                    sys.exit(1)
                
                try:
                    point.append(float(val_str))
                except ValueError:
                    print("An Error Has Occurred")
                    sys.exit(1)
            
            # Check dimension consistency
            if dimension is None:
                dimension = len(point)
            elif len(point) != dimension:
                print("An Error Has Occurred")
                sys.exit(1)
            
            data_points.append(point)
        except Exception:
            print("An Error Has Occurred")
            sys.exit(1)
    
    return data_points

def print_centroids(centroids: list[list[float]]) -> None:
    """
    Print centroid vectors formatted to four decimal places.

    Parameters
    ----------
    centroids : list[list[float]]
        Centroids to output.
    """
    for centroid in centroids:
        formatted = ','.join(['%.4f' % val for val in centroid])
        print(formatted)

def parse_int_strict(s: str) -> int | None:
    """
    Strictly parse an integer string.

    Parameters
    ----------
    s : str
        Input string.

    Returns
    -------
    int | None
        Parsed integer if valid; otherwise None.
    """
    if not s:
        return None
    
    try:
        num = int(s)
        if str(num) == s:
            return num
        else:
            return None
    except ValueError:
        return None
