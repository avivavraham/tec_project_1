#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

#define SQ(x) x * x

//TODO: all int outside the for
//initialize only at the start of every function
//add handle errors
//check division by zero
//check that all memory is free at the end


void init_data_frame(const char *file);
void validate(int condition);
int is_number(char* num);
int isdigit_help(char digit);
void find_vectors_len(FILE *fp);
void find_d_of_vector(FILE *fp);
char* concat(const char *s1, const char *s2);
void write_to_output_file();
int find_closets_cluster();
int algorithm();
void init_centroids();
void init_clusters();
void free_2d_array(double **array, int rows);
void free_3d_array(double ***array,int a,int b);
void set_equal_2d_array(double **new,double **current,int rows,int columns);
void zero_3d_array(double ***array,int x,int y,int z);
void zero_2d_array(double **array,int rows,int columns);

void normal_centroids(double **diff_centroids, double *sum_diff_centroids);
void substruct_centroids(double ** diff_centroids);
void calculate_new_centroids(int *num_elemnts_in_cluster);
double** allocate_2d_array(int rows,int columns);
double get_squared_distance(double *v1, double *v2);
void error(int line);

int k, max_iter,num_rows,d=1;
char *input_file, *output_file;
double **data_points,**centroids,**new_centroids,***clusters;

int main(int argc, char *argv[]) {
    printf("start\n");

    validate(argc == 4 || argc == 5);

    sscanf(argv[1],"%d",&k);

    if (is_number(argv[2])){
        sscanf(argv[2],"%d",&max_iter);
    } else{
        max_iter = 200;
    }
    input_file = argv[argc-2];
    output_file = argv[argc-1];

    init_data_frame(input_file);

    return algorithm();
}

/*
 * write func that creates data frame of vectors(array[]][])
 */
void init_data_frame(const char *file){
    // validate(5);
    FILE *ifp = NULL;
    ifp = fopen(input_file ,"r");
    int i=0,j=0,r=0;

    if (ifp != NULL){
        find_vectors_len(ifp);
        find_d_of_vector(ifp);
        
        double vectors[num_rows][d];
        char line[1024];
        char* temp_vector= "";
        while(fgets(line, sizeof line, ifp) != NULL) {
            while (line[r] != '\n'){
                if (line[r] != ','){
                    char *temp_char_to_string = malloc(2 * sizeof(char));
                    temp_char_to_string[0] = line[r];
                    temp_char_to_string[1] = '\0';
                    temp_vector = concat(temp_vector, temp_char_to_string);
                } else{
                    double ftemp = atof(temp_vector);
                    vectors[i][j] = ftemp;
                    j++;
                    temp_vector ="";
                }
                r++;
            }
            double ftemp = atof(temp_vector);
            vectors[i][j] = ftemp;
            j++;
            temp_vector ="";
            r = 0;
            i++;
            j=0;
        }
        printf("%d", i);
        fclose(ifp);

        data_points = calloc(num_rows, sizeof(double *));
        for(int i=0 ; i<num_rows ; i++ ){
            data_points[i] = calloc(d, sizeof(double));
        }
        for(int i=0 ; i<num_rows ; i++ ){
            for(j=0 ; j<d ; j++ ){
                data_points[i][j] = vectors[i][j];
            }
        }
    } else{
        error(113);
    }
}

void error(int line){
    printf("Error Has Occurred in line %d \n",line);
    exit(1);
}

void find_vectors_len(FILE *fp){
    char line[1024];
    while(fgets(line, sizeof line, fp) != NULL) {
        num_rows++;
    }
    fseek(fp,0,SEEK_SET);
}


void find_d_of_vector(FILE *fp){
    char line[1024];
    int i=0;
    if (fgets(line, sizeof line, fp) != NULL) {
        line[i] = line[0];
        while (line[i] != '\n'){
            if (line[i] == ','){
                d++;
            }
            i++;
        }
    }
    fseek(fp,0,SEEK_SET);
}

char* concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator
    // in real code you would check for errors in malloc here
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

int find_closets_cluster(double *data_point){
    double *difference;
    difference = calloc(k, sizeof(int *));
    int i=0;
    for(i=0 ; i<k ; i++){
        double *current_mu = centroids[i];
        double sum=0;
        // for(int j=0;j<d;j++){
        //     sum += (data_point[j]-current_mu[j]);
        // }
        // sum = SQ(sum);
        sum = get_squared_distance(current_mu,data_point);
        difference[i] = sum;
    }
    double min_sum = difference[0];
    int index=0;
    for (int i=0;i<k;i++){
        if(difference[i] < min_sum){
            min_sum = difference[i];
            index = i;
        }
    }
    return index;

    /*
    difference_array = [sum([(a - b)**2 for a, b in zip(data_point, mu_array[i])]) for i in range(len(mu_array))]
    min_sum = difference_array[0]
    index = 0
    for i in range(len(difference_array)):
        if difference_array[i] < min_sum:
            min_sum = difference_array[i]
            index = i
    return index
    */
}

void set_clusters(int *num_elemnts_in_cluster){
    for(int i=0;i<k;i++){
        num_elemnts_in_cluster[i] = 0;
    }
    for (int i=0;i<num_rows;i++){
        double *x_i = data_points[i];
        int index = find_closets_cluster(x_i);
        int elements_in_index = num_elemnts_in_cluster[index];
        for (int a=0;a<d;a++){
            clusters[index][elements_in_index][a] = x_i[a];
        }
        num_elemnts_in_cluster[index]++;
    }
}

void calculate_new_centroids(int *num_elemnts_in_cluster){
    for(int i=0;i<k;i++){
        double **cluster = clusters[i];
        int len_cluster = num_elemnts_in_cluster[i];
        double *current_centroid = calloc(len_cluster,sizeof(double));
        for (int j=0;j<len_cluster;j++){
            double *data = cluster[j];
            for(int m=0;m<d;m++){
                current_centroid[m] += data[m];
            }
        }
        //new_centroids[i] = [current_centroid[i] / len_cluster for i in range(len(current_centroid))]
        for(int j=0;j<d;j++){
            new_centroids[i][j] = current_centroid[j] / len_cluster;
        }
    }
}

void substruct_centroids(double ** diff_centroids){
    for(int i=0;i<k;i++){
        double *lst1 = centroids[i];
        double *lst2 = new_centroids[i];
        double *diff_lst = calloc(d,sizeof(double));
        for(int j=0;j<d;j++){
            diff_lst[j] = lst1[j] - lst2[j];
        }
        for(int j=0;j<k;j++){
            diff_centroids[i][j] = diff_lst[j];
        }
    }
}

void normal_centroids(double **diff_centroids, double *sum_diff_centroids){
     for(int i=0;i<k;i++){
        double *data_point = diff_centroids[i];
        double sum=0;
        for(int j=0;j<k;j++){
            sum += SQ(data_point[j]);
        }
        double normal = sqrt(sum);
        sum_diff_centroids[i] = normal;
    }
}

double get_squared_distance(double *v1, double *v2) {
    double dist = 0;
    int i;

    for (i = 0; i < d; i++)
        dist += pow(v1[i] - v2[i], 2);

    return dist;
}

int algorithm(){
    double epsilon = 0.001;
    init_centroids();
    init_clusters();
    new_centroids = allocate_2d_array(k,d);
    while(max_iter > 0){
        zero_3d_array(clusters,k,num_rows,d);
        zero_2d_array(new_centroids,k,d);

        int *num_elemnts_in_cluster = calloc(k,sizeof(int));

        set_clusters(num_elemnts_in_cluster);
        calculate_new_centroids(num_elemnts_in_cluster);

        // free(num_elemnts_in_cluster);
        max_iter--;
        //diff_centroids = [[lst1[i] - lst2[i] for i in range(len(lst1))] for lst1, lst2 in zip(centroids, new_centroids)]
        double ** diff_centroids;
        diff_centroids = allocate_2d_array(k,d);
        // substruct_centroids(diff_centroids);
        
        // //diff_centroids = [sum([num**2 for num in diff_centroids[i]])**0.5 for i in range(len(diff_centroids))]
        double *sum_diff_centroids = calloc(k,sizeof(double));
        // normal_centroids(diff_centroids,sum_diff_centroids);

        for(int i=0;i<k;i++){
            double diff = get_squared_distance(centroids[i],new_centroids[i]);
            double sq_diff = pow(diff,0.5);
            sum_diff_centroids[i] = sq_diff;
        }
        double max=sum_diff_centroids[0];
        for(int i=0;i<k;i++){
            if(sum_diff_centroids[i]>max){
                max = sum_diff_centroids[i];
            }
        }
        // free_2d_array(diff_centroids,k);
        // free(sum_diff_centroids);
        if(max<epsilon){
            set_equal_2d_array(centroids,new_centroids,k,d);
            write_to_output_file();
            free_2d_array(centroids,k);
            free_2d_array(new_centroids,k);
            free_2d_array(data_points,num_rows);
            free_3d_array(clusters,k,num_rows);
            return 1;
        }
        set_equal_2d_array(centroids,new_centroids,k,d);
    }
    write_to_output_file();
    
    return 1;

    /*
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

    */
}

double** allocate_2d_array(int rows,int columns){
    double ** array;
    array = calloc(rows,sizeof(double *));
    for (int i=0;i<rows;i++){
        array[i] = calloc(columns,sizeof(double));
    }
    return array;
}

void init_centroids(){
    centroids = calloc(k,sizeof(double *));

    for(int i=0;i<k;i++){
        centroids[i] = calloc(d,sizeof(double));
        for(int j=0;j<d;j++){
            centroids[i][j] = data_points[i][j];
        }
    }

}

void init_clusters(){
    clusters = calloc(k,sizeof(double **));
    for(int i=0;i<k;i++){
        clusters[i] = calloc(num_rows,sizeof(double *));
        for(int j=0;j<num_rows;j++){
            clusters[i][j] = calloc(d,sizeof(double));
        }
    }
}

int is_number(char s[])
{
    for (int i = 0; s[i]!= '\0'; i++)
    {
        if (isdigit_help(s[i]) == 0)
            return 0;
    }
    return 1;
}

int isdigit_help(char digit){
    if (digit < '0' || digit > '9')
        return 0;
    return 1;
}

void validate(int condition){
    if(!condition){
        printf("invalid input\n");
        exit(1);
    }
}

void write_to_output_file(){}

void free_2d_array(double **array, int rows) {
    for (int row = 0; row < rows; row++){
        free(array[row]);
    }
    free(array);
}

void free_3d_array(double ***array, int a,int b) {
    for (int row1 = 0; row1 < a; row1++){
        for(int row2 = 0;row2 < b; row2++){
            free(array[row1][row2]);
        }
        free(array[row1]);
    }
    free(array);
}

void set_equal_2d_array(double **new,double **current,int rows,int columns){
    for(int i=0;i<rows;i++){
        for(int j=0;j<columns;j++){
            new[i][j] = current[i][j];
        }
    }
}

void zero_2d_array(double **array,int rows,int columns){
    for(int i=0;i<rows;i++){
        for(int j=0;j<columns;j++){
            new_centroids[i][j] = 0;
        }
    }
}

void zero_3d_array(double ***array,int x,int y,int z){
    for(int i=0;i<x;i++){
        for(int j=0;j<y;j++){
            for(int m=0;j<y;j++){
                array[i][j][m] = 0;
            }
        }
    }
}
        