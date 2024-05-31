import numpy as np
import matplotlib.pyplot as plt
from scipy.spatial import cKDTree

# 乱数を初期化
np.random.seed(123)

# 球体内にランダムアトラクターを生成
def generate_attractors(num_attractors, radius=1):
    attractors = []
    for _ in range(num_attractors):
        theta = np.random.uniform(0, np.pi)
        phi = np.random.uniform(0, 2 * np.pi)
        r = radius * np.cbrt(np.random.uniform(0, 1))
        x = r * np.sin(theta) * np.cos(phi)
        y = r * np.sin(theta) * np.sin(phi)
        z = r * np.cos(theta)
        attractors.append([x, y, z])
    return np.array(attractors)

# 枝の成長
def grow_branches(attractors, initial_branches, iterations, attraction_distance, kill_distance):
    branches = initial_branches.copy().tolist()
    total_iterations = iterations  # 総イテレーション数を保存
    for i in range(iterations):
        if len(attractors) == 0:
            break
        new_branches = []
        tree = cKDTree(attractors)
        for branch in branches:
            # 近隣のアトラクターを探索
            distances, indices = tree.query(branch, k=len(attractors), distance_upper_bound=attraction_distance)
            valid_indices = indices[distances < attraction_distance]
            if len(valid_indices) > 0:
                valid_attractors = attractors[valid_indices]
                mean_direction = np.mean(valid_attractors - branch, axis=0)
                mean_direction /= np.linalg.norm(mean_direction)
                new_branch = branch + mean_direction * 0.1  # 枝の成長量
                new_branches.append(new_branch)
                
                # 近すぎるアトラクターを削除
                close_indices = valid_indices[np.linalg.norm(valid_attractors - branch, axis=1) < kill_distance]
                attractors = np.delete(attractors, close_indices, axis=0)
                tree = cKDTree(attractors)  # ツリーを更新
        branches.extend(new_branches)
        
        # 進行状況の表示
        progress = (i + 1) / total_iterations * 100  # 進行状況をパーセンテージで計算
        print(f"\rProgress: {progress:.2f}%", end='')  # 進行状況を表示
        
    return np.array(branches)

# パラメータ
num_attractors = 500
initial_branches = np.array([[0, 0, -1]])  # 初期枝の位置
iterations = 100
attraction_distance = 0.3
kill_distance = 0.3

# アトラクターの生成と枝の成長
attractors = generate_attractors(num_attractors)
branches = grow_branches(attractors, initial_branches, iterations, attraction_distance, kill_distance)

# プロット
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
ax.scatter(attractors[:, 0], attractors[:, 1], attractors[:, 2], color='red', label='Attractors')
ax.plot(branches[:, 0], branches[:, 1], branches[:, 2], color='green', label='Branches')

ax.set_xlabel('X')
ax.set_ylabel('Y')
ax.set_zlabel('Z')
ax.set_title('Space Colonization Algorithm with cKDTree in Sphere')
ax.legend()
plt.show()
