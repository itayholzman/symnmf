import symnmf
import kmeans
import sys
from sklearn.metrics import silhouette_score
import numpy as np

def main():
    if (len(sys.argv) != 3):
        print("An Error Has Occurred")
        sys.exit(1)

    k = int(sys.argv[1])
    data_points = symnmf.load_points_to_matrix(sys.argv[2])

    H = symnmf.symnmf(data_points, k)
    symnmf_labels = symnmf.derive_clustering(H)
    symnmf_score = silhouette_score(data_points, symnmf_labels)

    centroids = kmeans.kmeans(data_points.tolist(), k, symnmf.MAX_ITER, symnmf.EPSILON)
    kmeans_labels = kmeans.derive_clustering(data_points.tolist(), centroids)
    kmeans_score = silhouette_score(data_points, kmeans_labels)

    print(f"nmf: {symnmf_score}")
    print(f"kmeans: {kmeans_score}")
    

if __name__ == '__main__':
    main()