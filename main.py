import numpy as np


def algorithm(k, input_file, output_file, max_iter=200):
    epsilon = 0.001
    data_points = reading_from_file(input_file)
    centroids = init_centroids(data_points, k)
    N = len(data_points)
    while max_iter > 0:
        clusters = [[] for _ in range(k)]
        for i in range(N):
            x_i = data_points[i]
            index = find_closest_cluster(x_i, centroids)
            clusters[index].append(x_i)
        new_centroids = [[] for i in range(k)]
        for i in range(k):
            cluster = clusters[i]
            len_cluster = len(clusters[i])
            current_centroid = [0 for _ in range(len(cluster[0]))]
            for j in range(len_cluster):
                data = cluster[j]
                for m in range(len(data)):
                    current_centroid[m] += data[m]
            new_centroids[i] = [current_centroid[i] / len_cluster for i in range(len(current_centroid))]
        max_iter -= 1
        diff_centroids = [[lst1[i] - lst2[i] for i in range(len(lst1))] for lst1, lst2 in zip(centroids, new_centroids)]
        diff_centroids = [sum([num**2 for num in diff_centroids[i]])**0.5 for i in range(len(diff_centroids))]
        if max(diff_centroids) < epsilon:
            break
        centroids = new_centroids
    writing_to_output_file(output_file, new_centroids)


def writing_to_output_file(output_file, centroids):
    f = open(output_file, "a")
    f.write(centroids)
    f.close()
    #’%.4f’

def find_closest_cluster(data_point, mu_array):
    difference_array = [sum([(a - b)**2 for a, b in zip(data_point, mu_array[i])]) for i in range(len(mu_array))]
    min_sum = difference_array[0]
    index = 0
    for i in range(len(difference_array)):
        if difference_array[i] < min_sum:
            min_sum = difference_array[i]
            index = i
    return index

def reading_from_file(file):
    lines = tuple(open(file, 'r'))
    vectors = [line.split(",") for line in lines]
    for i in range(len(vectors)):
        for j in range(len(vectors[i])):
            vectors[i][j] = float(vectors[i][j])

    return vectors


def init_centroids(vectors_array, k):
    mu_array = [vectors_array[i] for i in range(k)]
    return mu_array


if __name__ == '__main__':
    algorithm(7, 'input_2.txt', 'output_check')

