import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

class Tree_node:
    def __init__(self, pos_x, pos_y, pos_z):
        self.x = pos_x
        self.y = pos_y
        self.z = pos_z
        self.pos = np.array([self.x, self.y, self.z])

    def __repr__(self):
        return self.pos.__repr__()

class Attraction_point:
    def __init__(self, x, y, z):
        self.x = x
        self.y = y
        self.z = z
        self.pos = np.array([self.x, self.y, self.z])

    def __repr__(self):
        return self.pos.__repr__()

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

        # attraction points
        x, y, z = crown_attraction_points
        attraction_pts = [Attraction_point(i, j, k) for i, j, k in zip(x, y, z)]

        # nodes
        self.nodes = []
        root = Tree_node(0.5, 0, 0)
        self.nodes.append(root)

        # closest node to each attraction pt
        self.closest_node = {attr_pt: None for attr_pt in attraction_pts}
        self.closest_dist = {attr_pt: np.inf for attr_pt in attraction_pts}
        self._update_closest_node(self.nodes[0])

        # branches
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

    def branch_thickness(self, node):
        if node in self.branch_width:
            return self.branch_width[node]
        if self.branching_tree.is_leaf(node):
            self.branch_width[node] = self.branch_min_width
            return self.branch_min_width
        if self.branching_tree.num_children(node) == 1:
            w = self.branch_thickness(self.branching_tree.get_children(node)[0])
            self.branch_width[node] = w
            return w
        w = 0
        for child in self.branching_tree.get_children(node):
            w += np.square(self.branch_thickness(child))
        w = np.sqrt(w)
        self.branch_width[node] = w
        return w

    def run(self, num_iteration):
        for i in range(num_iteration):
            self._iter()
            if len(self.closest_node) == 0:
                break
        self.render_results()

    def render_results(self):
        fig = plt.figure()
        ax = fig.add_subplot(111, projection='3d')
        for branch in self.branches:
            start, end, node = branch
            lw = self.branch_thickness(node)
            x = [start[0], end[0]]
            y = [start[1], end[1]]
            z = [start[2], end[2]]
            ax.plot(x, y, z, c='forestgreen', linewidth=lw)
        ax.set_xlim([-3, 3])
        ax.set_ylim([0, 8])
        ax.set_zlim([0, 8])
        ax.set_title('Final Result')
        ax.axis('off')
        plt.show()
        print()

    def _iter(self):
        self.iter_num += 1
        meta_nodes = []
        for node in self.nodes:
            S_v = {attr_pt for attr_pt in self.closest_node if self.closest_node[attr_pt] == node}
            if len(S_v) != 0:
                n = np.array([0, 0, 0], dtype=float)
                for attr_pt in S_v:
                    n += (attr_pt.pos - node.pos) / np.linalg.norm(attr_pt.pos - node.pos)
                n = n / np.linalg.norm(n)
                new_pos = node.pos + n * self.D
                new_node = Tree_node(new_pos[0], new_pos[1], new_pos[2])
                self._update_closest_node(new_node)
                branch = (node.pos, new_pos, new_node)
                self.branches.append(branch)
                self.branching_tree.add_child(node, new_node)
                meta_nodes.append(new_node)
        self.nodes.extend(meta_nodes)

def run_experiment_ellipse_crown_1():
    mean = [0.3, 5.5, 4]
    cov = [[1, 0, 0], [0, 2, 0], [0, 0, 1]]
    x, y, z = np.random.multivariate_normal(mean, cov, 50000).T
    t = np.square(x) + np.square(y-4)/4 + np.square(z-4)/4 <= 1
    x_crown = x[t]
    y_crown = y[t]
    z_crown = z[t]

    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')
    ax.scatter(x_crown, y_crown, z_crown)
    ax.set_title('Crown Attraction Points')
    ax.axis('off')
    ax.set_xlim([-3, 3])
    ax.set_ylim([0, 8])
    ax.set_zlim([0, 8])
    plt.show()

    sim = Simulation(crown_attraction_points=(x_crown, y_crown, z_crown), radius_of_influence=5, kill_distance=0.1, D=0.1, branch_min_width=1)
    sim.run(50)
    del sim

run_experiment_ellipse_crown_1()
