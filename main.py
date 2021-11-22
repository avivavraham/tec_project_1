import numpy as np


def algorithm(k, input_file, output_file, max_iter=200):
    epsilon = 0.001
    data_points = reading_from_file(input_file)
    centroids = init_centroids(data_points, k)
    N = len(data_points)
    while max_iter > 0:
        clusters = [[] for i in range(k)]
        for i in range(N):
            x_i = data_points[i]
            index = find_closest_cluster(x_i, centroids)
            clusters[index].append(x_i)
        new_centroids = np.array([np.sum(clusters[i], axis=0) / len(clusters[i]) for i in range(k)])
        max_iter -= 1
        diff_centroids = centroids - new_centroids
        diff_centroids = np.array([np.sqrt(np.sum(np.square(diff_centroids[i]))) for i in range(len(diff_centroids))])
        if np.max(diff_centroids) < epsilon:
            break
        centroids = new_centroids
    writing_to_output_file(output_file, new_centroids)


def writing_to_output_file(output_file, centroids):
    f = open(output_file, "a")
    f.write(str(np.round(centroids, 4)))
    f.close()


def find_closest_cluster(data_point, mu_array):
    difference_array = np.array([np.sum(np.square(data_point - mu_array[i])) for i in range(len(mu_array))])
    return np.argmin(difference_array)


def reading_from_file(file):
    lines = tuple(open(file, 'r'))
    vectors = np.array([np.array(line.split(",")) for line in lines]).astype(np.float)
    return vectors


def init_centroids(vectors_array, k):
    mu_array = np.array([vectors_array[i] for i in range(k)])
    return mu_array


if __name__ == '__main__':
    algorithm(7, 'input_2.txt', 'output_check')

