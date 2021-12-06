from os import close
import sys

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
    f = open(output_file, "w")
    f.writelines(','.join(f'{num:.4f}' for num in centroid) + '\n' for centroid in centroids)
    f.close()

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
    try:
        lines = tuple(open(file, 'r'))
        vectors = [line.split(",") for line in lines]
        for i in range(len(vectors)):
            for j in range(len(vectors[i])):
                vectors[i][j] = float(vectors[i][j])
        return vectors
    except Exception:
        print('An Error Has Occurred')
        exit(1)


def init_centroids(vectors_array, k):
    mu_array = [vectors_array[i] for i in range(k)]
    return mu_array

def validate_input(condition):
    if not condition:
        print('Invalid Input!')
        exit(1)


if __name__ == '__main__':
    try:
        args = sys.argv[1:]

        validate_input(len(args) == 3 or len(args) == 4)

        validate_input(args[0].isdigit())
        K = int(args[0])
        validate_input(K > 0)

        if len(args) == 4:
            validate_input(args[1].isdigit())
            max_iter = int(args[1])
        else:
            max_iter = 200

        input_file = args[-2]
        output_file = args[-1]

        algorithm(K, input_file, output_file)
    except Exception:
        print('An Error Has Occurred')
        exit(1)


