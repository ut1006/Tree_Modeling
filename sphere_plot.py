import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

# Function to generate random attractors inside a sphere
def generate_attractors(num_attractors, radius=1):
    attractors = []
    rng = np.random.RandomState(123)
    for _ in range(num_attractors):
        theta = rng.uniform(-1, 1)
        phi = rng.uniform(0, 2 * np.pi)
        r = rng.uniform(0, 1)
        x = r**(1/3) * (1 - theta**2) * np.cos(phi)
        y = r**(1/3) * (1 - theta**2) * np.sin(phi)
        z = r**(1/3) * theta
        attractors.append((x, y, z))
    return np.array(attractors)

# Function to grow branches towards the attractors
def grow_branches(attractors, initial_branches, iterations, attraction_distance, kill_distance):
    branches = initial_branches.copy()
    for _ in range(iterations):
        new_branches = []
        for branch in branches:
            closest_attractor = None
            min_distance = attraction_distance
            for attractor in attractors:
                distance = np.linalg.norm(np.array(attractor) - np.array(branch))
                if distance < min_distance:
                    closest_attractor = attractor
                    min_distance = distance
            if closest_attractor is not None:
                direction = np.array(closest_attractor) - np.array(branch)
                direction /= np.linalg.norm(direction)
                new_branch = np.array(branch) + direction * 0.1  # Branch growth amount
                new_branches.append(new_branch.tolist())
                if min_distance < kill_distance:
                    attractors = np.delete(attractors, np.where(np.all(attractors == closest_attractor, axis=1)), axis=0)
        branches.extend(new_branches)
    return np.array(branches)

# Parameters
num_attractors = 1000
initial_branches = [[0, 0, 0]]  # Initial branch position
iterations = 100
attraction_distance = 0.5
kill_distance = 0.5

# Generate attractors and grow branches
attractors = generate_attractors(num_attractors)
branches = grow_branches(attractors, initial_branches, iterations, attraction_distance, kill_distance)

# Plotting
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
ax.scatter(attractors[:, 0], attractors[:, 1], attractors[:, 2], color='red', label='Attractors')
ax.plot(branches[:, 0], branches[:, 1], branches[:, 2], color='green', label='Branches')

ax.set_xlabel('X')
ax.set_ylabel('Y')
ax.set_zlabel('Z')
ax.set_title('Space Colonization Algorithm in Sphere')
ax.legend()
plt.show()
