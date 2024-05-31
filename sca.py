import numpy as np
import matplotlib.pyplot as plt
from scipy.spatial import cKDTree

np.random.seed(123)

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

def grow_branches(attractors, initial_branches, iterations, attraction_distance, kill_distance):
    branches = initial_branches.copy()
    for i in range(iterations):
        if len(attractors) == 0:
            break
        branch_tree = cKDTree(branches)
        attractor_indices_to_delete = []
        branch_growth_vectors = {tuple(branch.astype(float)): np.array([0.0, 0.0, 0.0]) for branch in branches}

        # Find the nearest node from each attractor
        for attractor_index, attractor in enumerate(attractors):
            distance, branch_index = branch_tree.query(attractor, distance_upper_bound=attraction_distance)
            if distance < attraction_distance:
                # Record the nearby attractors
                branch = branches[branch_index]
                branch_growth_vectors[tuple(branch.astype(float))] += attractor - branch#tentative growth vectors
                if distance < kill_distance:
                    # Add the attractor to the deletion list
                    attractor_indices_to_delete.append(attractor_index)

        # Grow the branches using the average vector
        for branch, growth_vector in branch_growth_vectors.items():
            if np.linalg.norm(growth_vector) > 0:
                new_branch = np.array(branch) + growth_vector / np.linalg.norm(growth_vector) * 0.2
                branches = np.vstack([branches, new_branch])

        # Remove the attractors that are too close
        attractors = np.delete(attractors, attractor_indices_to_delete, axis=0)

        # Display the progress
        progress = (i + 1) / iterations * 100
        print(f"\rProgress: {progress:.2f}%", end='')

    return branches

# Parameters
num_attractors = 1000

initial_branches = np.array([[0, -0.8, -0.3]])  # Initial branch position
iterations = 200
attraction_distance = 0.5
kill_distance = 0.5

# Generate attractors and grow the branches
attractors = generate_attractors(num_attractors)
branches = grow_branches(attractors, initial_branches, iterations, attraction_distance, kill_distance)

# Plot
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
