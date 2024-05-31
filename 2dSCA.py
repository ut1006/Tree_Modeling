import numpy as np
import matplotlib.pyplot as plt
from scipy.spatial import cKDTree

class AttractionPoint:
    def __init__(self, x, y):
        self.pos = np.array([x, y])

class TreeNode:
    def __init__(self, x, y):
        self.pos = np.array([x, y])
        self.x = x
        self.y = y

class Tree:
    def __init__(self, root):
        self.root = root
        self.nodes = [self.root]
        self.transition_map = {}

    def add_child(self, parent, child):
        if child in self.nodes:
            raise ValueError("Child already exists in the tree")
        if parent in self.nodes:
            self.transition_map[child] = parent
            self.nodes.append(child)
        else:
            raise ValueError("Parent does not exist in the tree")

    def is_leaf(self, node):
        if node not in self.nodes:
            raise ValueError("Node does not exist in the tree")
        return node not in self.transition_map.values()

    def num_children(self, node):
        if node not in self.nodes:
            raise ValueError("Node does not exist in the tree")
        return list(self.transition_map.values()).count(node)

    def get_children(self, parent):
        if parent not in self.nodes:
            raise ValueError("Parent does not exist in the tree")
        return [child for child in self.transition_map if self.transition_map[child] == parent]

    def get_level(self, node):
        if node not in self.nodes:
            raise ValueError("Node does not exist in the tree")
        if node == self.root:
            return 0
        x = self.transition_map[node]
        level = 1
        while x != self.root:
            x = self.transition_map[x]
            if self.num_children(x) > 1:
                level += 1
        return level

class Simulation:
    def __init__(self, crown_attraction_points, radius_of_influence, kill_distance, D, branch_min_width):
        self.d_i = radius_of_influence
        self.d_k = kill_distance
        self.D = D
        self.iter_num = 0
        x, y = crown_attraction_points
        self.attraction_pts = [AttractionPoint(i, j) for i, j in zip(x, y)]
        self.nodes = []
        root = TreeNode(0.5, 0)
        self.nodes.append(root)
        self.closest_node = {attr_pt: None for attr_pt in self.attraction_pts}
        self.closest_dist = {attr_pt: np.inf for attr_pt in self.attraction_pts}
        self._update_closest_node(self.nodes[0])
        self.branches = []
        self.branching_tree = Tree(root)
        self.branch_min_width = branch_min_width
        self.branch_width = {}

    def _update_closest_node(self, node):
        kill_candidates = []
        for attr_pt in self.closest_node:
            old_smallest = self.closest_dist[attr_pt]
            dist = np.linalg.norm(attr_pt.pos - node.pos)
            if dist < self.d_k:
                kill_candidates.append(attr_pt)
                continue
            if dist < self.d_i and dist < old_smallest:
                self.closest_node[attr_pt] = node
                self.closest_dist[attr_pt] = dist
        for attr_pt in kill_candidates:
            del self.closest_node[attr_pt]
            del self.closest_dist[attr_pt]

    def _report(self):
        print(f'\tREPORT FOR ITERATION {self.iter_num}')
        print('Number of nodes:', len(self.nodes))
        print('Number of attraction points:', len(self.closest_node))
        print()
        x = [node.x for node in self.nodes]
        y = [node.y for node in self.nodes]
        plt.scatter(x, y, s=2, c='slategray')
        plt.xlim(-3, 3)
        plt.ylim(0, 8)
        plt.title(f'Skeleton progress for iteration {self.iter_num}')
        plt.axis('off')
        plt.show()
        print()

def generate_attractors(num_attractors, radius=1):
    attractors = []
    for _ in range(num_attractors):
        theta = np.random.uniform(0, 2 * np.pi)
        r = radius * np.sqrt(np.random.uniform(0, 1))
        x = r * np.cos(theta)
        y = r * np.sin(theta)
        attractors.append([x, y])
    return np.array(attractors)

def grow_branches(attractors, initial_branches, iterations, attraction_distance, kill_distance):
    branches = initial_branches.copy()
    for i in range(iterations):
        if len(attractors) == 0:
            break
        branch_tree = cKDTree(branches)
        attractor_indices_to_delete = []
        branch_growth_vectors = {tuple(branch.astype(float)): np.array([0.0, 0.0]) for branch in branches}

        for attractor_index, attractor in enumerate(attractors):
            distance, branch_index = branch_tree.query(attractor, distance_upper_bound=attraction_distance)
            if distance < attraction_distance:
                branch = branches[branch_index]
                branch_growth_vectors[tuple(branch.astype(float))] += attractor - branch
                if distance < kill_distance:
                    attractor_indices_to_delete.append(attractor_index)

        for branch, growth_vector in branch_growth_vectors.items():
            if np.linalg.norm(growth_vector) > 0:
                new_branch = np.array(branch) + growth_vector / np.linalg.norm(growth_vector) * 0.1
                branches = np.vstack([branches, new_branch])

        attractors = np.delete(attractors, attractor_indices_to_delete, axis=0)

        print(f"\rProgress: {(i + 1) / iterations * 100:.2f}%", end='')

    return branches

# Parameters
num_attractors = 100
initial_branches = np.array([[0, 0]])  # Initial branch position
iterations = 200
attraction_distance = 0.5
kill_distance = 0.48

# Generate attractors and grow the branches
attractors = generate_attractors(num_attractors)
branches = grow_branches(attractors, initial_branches, iterations, attraction_distance, kill_distance)

# Plot
plt.figure(figsize=(8, 8))
plt.scatter(attractors[:, 0], attractors[:, 1], color='red', label='Attractors')
plt.plot(branches[:, 0], branches[:, 1], color='green', marker='o', label='Branches')
plt.xlabel('X')
plt.ylabel('Y')
plt.title('Space Colonization Algorithm in 2D')
plt.legend()
plt.show()
