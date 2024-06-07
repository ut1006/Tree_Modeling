import numpy as np
from scipy.sparse import csr_matrix

# 元の行列（例として、スパース行列を定義）
dense_matrix = np.array([
    [0, 1, 1, 0, 1, 0,0],
    [0, 0, 0, 0, 0, 0,0],
    [0, 0, 0, 1, 0, 0,0],
    [0, 0, 0, 0, 0, 0,0],
    [0, 0, 0, 0, 0, 1,1],
    [0, 0, 0, 0, 0, 0,0],
    [0, 0, 0, 0, 0, 0,0]
])

# CSR形式に変換
csr_matrix = csr_matrix(dense_matrix)

# CSR形式の各属性を出力
print("Data (非ゼロ要素):", csr_matrix.data)
print("Indices (列インデックス):", csr_matrix.indices)
print("Indptr (行ポインタ):", csr_matrix.indptr)
