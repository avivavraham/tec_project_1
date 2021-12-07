#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define SEEK_SET 0

void init_data_frame();

void input_valid(int condition);

int is_number(char s[]);

int isdigit_help(char digit);

void find_vectors_len(FILE *fp);

void find_d_of_vector(FILE *fp);

void write_to_output_file();

int find_closets_cluster(double *data_point);

void algorithm();

void init_centroids();

void free_array_2d(double **arr, int r);

void set_equal_array_2d(double **new, double **current, int r, int c);

void zero_array_2d(double **arr, int r, int c);

void calculate_new_centroids();

double **allocate_array_2d(int r, int c);

double calculate_distance_squared(double *a, double *b);

void error_occured(int condition);

void set_clusters();

int k, max_iter, num_rows, d = 1;
int *num_elements_in_cluster;
char *input_file, *output_file;
double **data_points, **centroids, **new_centroids, **clusters;

int main(int argc, char **argv) {
    input_valid(argc >= 4 && argc <= 5);

    input_valid(is_number(argv[1]));


    input_valid(sscanf(argv[1], "%d", &k) != EOF);
    input_valid(k > 1);

    if (is_number(argv[2])) {
        sscanf(argv[2], "%d", &max_iter);
    } else {
        input_valid(argc == 4);
        max_iter = 200;
    }
    input_file = argv[argc - 2];
    output_file = argv[argc - 1];
    input_valid(!is_number(input_file));
    input_valid(!is_number(output_file));

    init_data_frame();

    algorithm();
    return 0;
}

void init_data_frame() {
    FILE *ifp = NULL;
    int i = 0, j;
    char line[1024];

    ifp = fopen(input_file, "r");
    error_occured(ifp == NULL);

    find_vectors_len(ifp);
    find_d_of_vector(ifp);

    data_points = allocate_array_2d(num_rows, d);
    error_occured(data_points == NULL);

    while (fgets(line, sizeof line, ifp) != NULL) {
        line[strlen(line) - 1] = '\0';
        for (j = 0; j < d; j++) {
            if (j == 0) {
                data_points[i][j] = atof(strtok(line, ","));
            } else {
                data_points[i][j] = atof(strtok(NULL, ","));
            }
        }
        i++;
    }
    fclose(ifp);

}

void error_occured(int condition) {
    if (condition == 1) {
        printf("An Error Has Occurred\n");
        exit(1);
    }
}

void find_vectors_len(FILE *fp) {
    char line[1024];
    while (fgets(line, sizeof line, fp) != NULL) {
        num_rows++;
    }
    fseek(fp, 0, SEEK_SET);

    input_valid(k < num_rows);
}

void find_d_of_vector(FILE *fp) {
    char line[1024];
    int i = 0;
    fseek(fp, 0, SEEK_SET);
    if (fgets(line, sizeof line, fp) != NULL) {
        line[i] = line[0];
        while (line[i] != '\n') {
            if (line[i] == ',') {
                d++;
            }
            i++;
        }
    }
    fseek(fp, 0, SEEK_SET);
}

int find_closets_cluster(double *data_point) {
    double *difference, *current_mu, sum, min_sum;
    int index = 0, i;

    difference = calloc(k, sizeof(int *));
    error_occured(difference == NULL);
    for (i = 0; i < k; i++) {
        current_mu = centroids[i];
        sum = calculate_distance_squared(current_mu, data_point);
        difference[i] = sum;
    }
    min_sum = difference[0];
    for (i = 0; i < k; i++) {
        if (difference[i] < min_sum) {
            min_sum = difference[i];
            index = i;
        }
    }
    free(difference);
    return index;
}

void set_clusters() {
    int index, i, a;

    for (i = 0; i < k; i++) {
        num_elements_in_cluster[i] = 0;
    }
    for (i = 0; i < num_rows; i++) {
        index = find_closets_cluster(data_points[i]);
        for (a = 0; a < d; a++) {
            clusters[index][a] += data_points[i][a];
        }
        num_elements_in_cluster[index]++;
    }
}

void calculate_new_centroids() {
    double *cluster;
    int len_cluster, i, j;

    for (i = 0; i < k; i++) {
        cluster = clusters[i];
        len_cluster = num_elements_in_cluster[i];
        for (j = 0; j < d; j++) {
            new_centroids[i][j] = cluster[j] / len_cluster;
        }
    }
}

double calculate_distance_squared(double *a, double *b) {
    double distance = 0;
    int i = d;
    while (i--) {
        distance += pow(a[i] - b[i], 2);
    }
    return distance;
}

void algorithm() {
    double epsilon = 0.001;
    int i, j;
    double *sum_diff_centroids;
    double diff, sq_diff, max;
    num_elements_in_cluster = calloc(k, sizeof(int));

    centroids = allocate_array_2d(k, d);
    init_centroids();
    clusters = allocate_array_2d(k, d);
    new_centroids = allocate_array_2d(k, d);
    sum_diff_centroids = calloc(k, sizeof(double));
    while (max_iter > 0) {
        for (j = 0; j < k; j++) {
            sum_diff_centroids[j] = 0;
        }

        zero_array_2d(new_centroids, k, d);
        zero_array_2d(clusters, k, d);

        error_occured(num_elements_in_cluster == NULL);

        set_clusters();
        calculate_new_centroids();

        max_iter--;
        error_occured(sum_diff_centroids == NULL);

        for (i = 0; i < k; i++) {
            diff = calculate_distance_squared(centroids[i], new_centroids[i]);
            sq_diff = pow(diff, 0.5);
            sum_diff_centroids[i] = sq_diff;
        }
        max = sum_diff_centroids[0];
        for (i = 0; i < k; i++) {
            if (sum_diff_centroids[i] > max) {
                max = sum_diff_centroids[i];
            }
        }
        set_equal_array_2d(centroids, new_centroids, k, d);
        if (max <= epsilon) {
            max_iter = 0;
        }
    }
    write_to_output_file();
    free(sum_diff_centroids);
    free_array_2d(centroids, k);
    free_array_2d(new_centroids, k);
    free_array_2d(data_points, num_rows);
    free_array_2d(clusters, k);
    free(num_elements_in_cluster);
}

double **allocate_array_2d(int r, int c) {
    double **arr;
    arr = calloc(r, sizeof(double *));
    error_occured(arr == NULL);
    while (r--) {
        arr[r] = calloc(c, sizeof(double));
        error_occured(arr[r] == NULL);
    }
    return arr;
}

void init_centroids() {
    int i, j;
    for (i = 0; i < k; i++) {
        for (j = 0; j < d; j++) {
            centroids[i][j] = data_points[i][j];
        }
    }

}

int is_number(char s[]) {
    int i;
    for (i = 0; s[i] != '\0'; i++) {
        if (isdigit_help(s[i]) == 0)
            return 0;
    }
    return 1;
}

int isdigit_help(char digit) {
    if (digit < '0' || digit > '9')
        return 0;
    return 1;
}

void input_valid(int condition) {
    if (condition == 0) {
        printf("Invalid Input!\n");
        exit(1);
    }
}

void write_to_output_file() {
    FILE *fptr;
    int i, j;

    fptr = fopen(output_file, "w");
    error_occured(fptr == NULL);

    for (i = 0; i < k; ++i) {
        for (j = 0; j < d - 1; ++j) {
            fprintf(fptr, "%.4f", centroids[i][j]);
            fprintf(fptr, ",");
        }
        fprintf(fptr, "%.4f", centroids[i][d - 1]);
        fprintf(fptr, "\n");
    }
    fclose(fptr);
}

void free_array_2d(double **arr, int r) {
    while (r--) {
        free(arr[r]);
    }
    free(arr);
}

void set_equal_array_2d(double **new, double **current, int r, int c) {
    int i, j;

    for (i = 0; i < r; i++) {
        for (j = 0; j < c; j++) {
            new[i][j] = current[i][j];
        }
    }
}

void zero_array_2d(double **arr, int r, int c) {
    int i, j;

    for (i = 0; i < r; i++) {
        for (j = 0; j < c; j++) {
            arr[i][j] = 0;
        }
    }
}
