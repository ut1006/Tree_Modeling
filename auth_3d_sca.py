import numpy as np
import matplotlib.pyplot as plt
import random
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
            raise ValueError

        if parent in self.nodes:
            self.transition_map[child] = parent  # childをkeyとしてparentを登録
            self.nodes.append(child)
        else:
            raise ValueError

    def is_leaf(self, node):
        if node not in self.nodes:
            raise ValueError
        if node not in self.transition_map.values():
            return True
        else:
            return False

    def num_children(self, node):
        if node not in self.nodes:
            raise ValueError
        return list(self.transition_map.values()).count(node)

    def get_children(self, parent):
        if parent not in self.nodes:
            raise ValueError
        return [child for child in self.transition_map if self.transition_map[child] == parent]

    def get_level(self, node):
        if node not in self.nodes:
            raise ValueError
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
    def __init__(self, crown_attraction_points, radius_of_influence, kill_distance, D, brach_min_width):
        self.d_i = radius_of_influence
        self.d_k = kill_distance
        self.D = D
        self.iter_num = 0

        # list of attraction points
        x, y, z = crown_attraction_points

        attraction_pts = []
        for i, j, k in list(zip(x, y, z)):
            attraction_pts.append(Attraction_point(i, j, k))  # 点ごとに(x,y,z)の組にして追加

        # nodes
        self.nodes = []
        root = Tree_node(0.5, 0, 0)  # rootの位置
        self.nodes.append(root)

        # closest node to each attraction pt
        self.closest_node = {attr_pt: None for attr_pt in attraction_pts}
        self.closest_dist = {attr_pt: np.inf for attr_pt in attraction_pts}  # distanceのリスト。無限大でinit

        self._update_closest_node(self.nodes[0])

        # branches
        self.branches = []
        self.branching_tree = Tree(root)
        self.brach_min_width = brach_min_width
        self.branch_width = {}

    def _update_closest_node(self, node):
        kill_candidates = []

        # internal method to update self.closest_node and self.closest_dist
        for attr_pt in self.closest_node:
            old_smallest = self.closest_dist[attr_pt]
            dist = np.linalg.norm(attr_pt.pos - node.pos)

            if dist < self.d_k:
                # attr_pt to be killed
                kill_candidates.append(attr_pt)
                continue

            if dist < self.d_i and dist < old_smallest:
                self.closest_node[attr_pt] = node
                self.closest_dist[attr_pt] = dist

        # kill attraction points with nodes too close to them
        for attr_pt in kill_candidates:
            del self.closest_node[attr_pt]
            del self.closest_dist[attr_pt]

    def _report(self):
        print(f'\tREPORT FOR ITERATION {self.iter_num}')
        print('Number of nodes:', len(self.nodes))
        print('Number of attraction points:', len(self.closest_node))
        print()

        data = np.stack([node.pos for node in self.nodes])

        x = []
        y = []
        z = []

        for node in self.nodes:
            x.append(node.x)
            y.append(node.y)
            z.append(node.z)

        fig = plt.figure()
        ax = fig.add_subplot(111, projection='3d')
        ax.scatter(x, y, z, s=2, c='slategray')
        ax.set_xlim(-3, 3)
        ax.set_ylim(0, 8)
        ax.set_zlim(-3, 3)
        ax.set_title(f'skeleton progress for iteration {self.iter_num}')
        ax.set_axis_off()
        plt.show()
        print()

    def branch_thickness(self, node):
        if node in self.branch_width:
            return self.branch_width[node]

        if self.branching_tree.is_leaf(node):
            self.branch_width[node] = self.brach_min_width
            return self.brach_min_width

        if self.branching_tree.num_children(node) == 1:
            w = self.branch_thickness(self.branching_tree.get_children(node)[0])
            self.branch_width[node] = w
            return w

        w = 0
        for child in self.branching_tree.get_children(node):
            w += np.power(self.branch_thickness(child),4)
        w = np.power(w,1/4)#default is w+=np.square and power(1/2)

        self.branch_width[node] = w
        return w

    def run(self, num_iteration):
        for i in range(num_iteration):
            self._iter()
            if len(self.closest_node) == 0:
                break
        self._report()
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

        ax.set_xlim(-3, 3)
        ax.set_ylim(0, 8)
        ax.set_zlim(-3, 3)
        ax.set_title('Final Result')
        ax.set_axis_off()
        plt.show()
        print()

        # 分岐点とその情報の表示
        for node in self.nodes:
            if self.branching_tree.num_children(node) > 1:  # 分岐点
                parent = self.branching_tree.transition_map[node]
                children = self.branching_tree.get_children(node)

                for child in children:
                    branch_length = np.linalg.norm(child.pos - node.pos)
                    print(f'Branch point at {node.pos}, Parent at {parent.pos}, Child at {child.pos}, Branch length: {branch_length}')
                    print()
            elif self.branching_tree.is_leaf(node):  # 先端
                parent = self.branching_tree.transition_map[node]
                branch_length = np.linalg.norm(node.pos - parent.pos)
                print(f'Leaf at {node.pos}, Parent at {parent.pos}, Branch length: {branch_length}')
                print()

    def _iter(self):#ここで成長予測をしてく
        self.iter_num += 1

        meta_nodes = []
        for node in self.nodes:#もしsetが空だったらスキップ。ただし今はDi=5なので必ずfilled.
            # find set of attraction pts affecting node
            S_v = {attr_pt for attr_pt in self.closest_node if self.closest_node[attr_pt] == node}

            # if set is not empty, add new node
            if len(S_v) != 0:
                # find new node pos
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
        # add newly added nodes
        self.nodes.extend(meta_nodes)

        # report iteration results
        if self.iter_num % 10 == 0:
            self._report()
    

    def find_branching_points(self):
        branching_points = []
        for node in self.branching_tree.nodes:
            if self.branching_tree.num_children(node) > 1:
                branching_points.append(node)
        return branching_points

    def display_branching_info(self):
        branching_points = self.find_branching_points()
        for node in branching_points:
            parent = self.branching_tree.transition_map[node]
            children = self.branching_tree.get_children(node)

            parent_vector = node.pos - parent.pos
            child_vectors = [child.pos - node.pos for child in children]

            print(f'Branching point: {node.pos}')
            print(f'  Parent vector: {parent_vector}')
            for i, vec in enumerate(child_vectors):
                print(f'  Child {i+1} vector: {vec}')
            print(f'  Branch thickness: {self.branch_thickness(node)}')
            print()

def run_experiment_ellipsoid_crown_1():
    mean = [0.3, 5.5, 0.3]#３次元ガウス分布の平均
    cov = [[1, 0, 0], [0.6, 2, 0], [0, 0, 1]]#共分散行列
    #ランダムに点を生成してそのうちCrown内にあるもののみを採用する
    x, y, z = np.random.multivariate_normal(mean, cov, 1000).T

    t = np.square(x) + np.square(y - 4) / 4 + np.square(z) <= 1
    #中心(0,4,0),半径１の球の中にあれば１を返すBoolリスト。
    x_crown = x[t]
    y_crown = y[t]
    z_crown = z[t]

    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')
    ax.plot(x_crown, y_crown, z_crown, 'o')
    ax.set_title('Crown Attraction Points')
    ax.set_axis_off()
    ax.set_xlim(-3, 3)
    ax.set_ylim(0, 8)
    ax.set_zlim(-3, 3)
    plt.show()

    sim = Simulation(crown_attraction_points=(x_crown, y_crown, z_crown), radius_of_influence=5, kill_distance=0.1, D=0.1, brach_min_width=1)
    sim.run(50)
    sim.display_branching_info()
    sim.render_results()

    del sim

run_experiment_ellipsoid_crown_1()