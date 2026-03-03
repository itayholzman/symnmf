import numpy as np
from sklearn.datasets import make_blobs
from sklearn.metrics import adjusted_rand_score
from symnmf import sym_nmf  # your actual function here

def test_dataset(n_samples, true_k, full_sweep=False):
    print(f"\n--- Dataset: {n_samples} points, true k={true_k} ---")
    X, y_true = make_blobs(n_samples=n_samples, centers=true_k, n_features=true_k, random_state=42)

    # k range: 2..reasonable upper limit
    if full_sweep:
        k_range = range(2, n_samples)
    else:
        k_range = range(2, min(n_samples, true_k*2 + 1))

    best_ari = -1
    best_k = None
    for k in k_range:
        H = sym_nmf(X, k=k)
        labels = np.argmax(H, axis=1)
        ari = adjusted_rand_score(y_true, labels)
        status = "✅" if ari >= 0.9 else "❌"
        print(f"k={k}: H shape={H.shape}, ARI={ari:.3f} {status}")
        if ari > best_ari:
            best_ari = ari
            best_k = k

    print(f"→ Best k: {best_k}, ARI={best_ari:.3f}")

# Example datasets
datasets = [
    (50, 3),
    (100, 4),
    (150, 5),
    (200, 4),
    (300, 5),
]

for n, k_true in datasets:
    test_dataset(n, k_true)