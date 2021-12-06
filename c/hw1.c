#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

#define SQ(x) x * x

//TODO: all int outside the for - done
//initialize only at the start of every function - done
//add handle errors - done
//check division by zero
//check that all memory is free at the end


void init_data_frame(const char *file);
void validate_input_input(int condition);
int is_number(char* num);
int isdigit_help(char digit);
void find_vectors_len(FILE *fp);
void find_d_of_vector(FILE *fp);
char* concat(const char *s1, const char *s2);
void write_to_output_file(double **array_of_centroids);
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
void error(int condition);

int k, max_iter,num_rows,d=1;
char *input_file, *output_file;
double **data_points,**centroids,**new_centroids,***clusters;

int main(int argc, char *argv[]) {
    validate_input_input(argc == 4 || argc == 5);

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

void init_data_frame(const char *file){
    FILE *ifp = NULL;
    int i=0,j=0,r=0;
    char line[1024];
    char *temp_vector= "";
    char *temp_char_to_string="";
    double ftemp;

    ifp = fopen(input_file ,"r");
    error(ifp == NULL);
  
    find_vectors_len(ifp);
    find_d_of_vector(ifp);

    double vectors[num_rows][d];

    while(fgets(line, sizeof line, ifp) != NULL) {
        while (line[r] != '\n'){
            if (line[r] != ','){
                temp_char_to_string = malloc(2 * sizeof(char));
                error(temp_char_to_string == NULL);
                temp_char_to_string[0] = line[r];
                temp_char_to_string[1] = '\0';
                temp_vector = concat(temp_vector, temp_char_to_string);
            } else{
                ftemp = atof(temp_vector);
                vectors[i][j] = ftemp;
                j++;
                temp_vector ="";
            }
            r++;
        }
        ftemp = atof(temp_vector);
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
    error(data_points == NULL);
    for(i=0 ; i<num_rows ; i++ ){
        data_points[i] = calloc(d, sizeof(double));
        error(data_points[i] == NULL);

    }
    for(i=0 ; i<num_rows ; i++ ){
        for(j=0 ; j<d ; j++ ){
            data_points[i][j] = vectors[i][j];
        }
    }
    
}

void error(int condition){
    if(condition){
        printf("Error Has Occurred in line\n");
        exit(1);
    }
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
    char *result = malloc(strlen(s1) + strlen(s2) + 1);
    error(result == NULL);
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

int find_closets_cluster(double *data_point){
    double *difference, *current_mu, sum,min_sum;
    int index=0;


    difference = calloc(k, sizeof(int *));
    error(difference == NULL);
    int i=0;
    for(i=0 ; i<k ; i++){
        current_mu = centroids[i];
        sum = get_squared_distance(current_mu,data_point);
        difference[i] = sum;
    }
    min_sum = difference[0];
    for (i=0;i<k;i++){
        if(difference[i] < min_sum){
            min_sum = difference[i];
            index = i;
        }
    }
    return index;
}

void set_clusters(int *num_elemnts_in_cluster){
    int index, elements_in_index,i,a;
    double *x_i; 


    for(i=0;i<k;i++){
        num_elemnts_in_cluster[i] = 0;
    }
    for (i=0;i<num_rows;i++){
        x_i = data_points[i];
        index = find_closets_cluster(x_i);
        elements_in_index = num_elemnts_in_cluster[index];
        for (a=0;a<d;a++){
            clusters[index][elements_in_index][a] = x_i[a];
        }
        num_elemnts_in_cluster[index]++;
    }
}

void calculate_new_centroids(int *num_elemnts_in_cluster){
    double **cluster,*current_centroid,*data;
    int len_cluster,i,j,m;

    for( i=0;i<k;i++){
        cluster = clusters[i];
        len_cluster = num_elemnts_in_cluster[i];
        current_centroid = calloc(len_cluster,sizeof(double));
        error(current_centroid == NULL);
        for ( j=0;j<len_cluster;j++){
            data = cluster[j];
            for( m=0;m<d;m++){
                current_centroid[m] += data[m];
            }
        }
        for( j=0;j<d;j++){
            new_centroids[i][j] = current_centroid[j] / len_cluster;
        }
    }
}

void substruct_centroids(double ** diff_centroids){
    double *lst1,*lst2,*diff_lst; 
    int i,j;

    for( i=0;i<k;i++){
        lst1 = centroids[i];
        lst2 = new_centroids[i];
        diff_lst = calloc(d,sizeof(double));
        error(diff_lst == NULL);

        for( j=0;j<d;j++){
            diff_lst[j] = lst1[j] - lst2[j];
        }
        for( j=0;j<k;j++){
            diff_centroids[i][j] = diff_lst[j];
        }
    }
}

void normal_centroids(double **diff_centroids, double *sum_diff_centroids){
    double *data_point,sum,normal;
    int i,j;
    for( i=0;i<k;i++){
        data_point = diff_centroids[i];
        sum=0;
        for( j=0;j<k;j++){
            sum += SQ(data_point[j]);
        }
        normal = sqrt(sum);
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
    int *num_elemnts_in_cluster,i;
    double **diff_centroids,*sum_diff_centroids;
    double diff,sq_diff,max;



    init_centroids();
    init_clusters();
    new_centroids = allocate_2d_array(k,d);
    while(max_iter > 0){
        zero_3d_array(clusters,k,num_rows,d);
        zero_2d_array(new_centroids,k,d);

        num_elemnts_in_cluster = calloc(k,sizeof(int));
        error(num_elemnts_in_cluster == NULL);

        set_clusters(num_elemnts_in_cluster);
        calculate_new_centroids(num_elemnts_in_cluster);

        max_iter--;
        diff_centroids = allocate_2d_array(k,d);
        
        sum_diff_centroids = calloc(k,sizeof(double));
        error(sum_diff_centroids == NULL);

        for( i=0;i<k;i++){
            diff = get_squared_distance(centroids[i],new_centroids[i]);
            sq_diff = pow(diff,0.5);
            sum_diff_centroids[i] = sq_diff;
        }
        max = sum_diff_centroids[0];
        for( i=0;i<k;i++){
            if(sum_diff_centroids[i]>max){
                max = sum_diff_centroids[i];
            }
        }
        if(max<epsilon){
            set_equal_2d_array(centroids,new_centroids,k,d);
            write_to_output_file(centroids);
            free_2d_array(centroids,k);
            free_2d_array(new_centroids,k);
            free_2d_array(data_points,num_rows);
            free_3d_array(clusters,k,num_rows);
            return 1;
        }
        set_equal_2d_array(centroids,new_centroids,k,d);
    }
    write_to_output_file(centroids);
    
    return 1;     
}

double** allocate_2d_array(int rows,int columns){
    double ** array;
    int i;

    array = calloc(rows,sizeof(double *));
    error(array == NULL);
    
    for ( i=0;i<rows;i++){
        array[i] = calloc(columns,sizeof(double));
        error(array[i] == NULL);
    }
    return array;
}

void init_centroids(){
    int i,j;

    centroids = calloc(k,sizeof(double *));
    error(centroids == NULL);

    for( i=0;i<k;i++){
        centroids[i] = calloc(d,sizeof(double));
        error(centroids[i] == NULL);
        for( j=0;j<d;j++){
            centroids[i][j] = data_points[i][j];
        }
    }

}

void init_clusters(){
    int i,j;

    clusters = calloc(k,sizeof(double **));
    error(clusters == NULL);
    for( i=0;i<k;i++){
        clusters[i] = calloc(num_rows,sizeof(double *));
        error(clusters[i] == NULL);
        for( j=0;j<num_rows;j++){
            clusters[i][j] = calloc(d,sizeof(double));
            error(clusters[i][j] == NULL);
        }
    }
}

int is_number(char s[]){
    int i;

    for ( i = 0; s[i]!= '\0'; i++)
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

void validate_input_input(int condition){
    if(!condition){
        printf("invalid input\n");
        exit(1);
    }
}

void write_to_output_file(double **array_of_centroids){
    FILE *fptr;
    int i,j;

    fptr = fopen(output_file,"w");
    error(fptr == NULL);

    for ( i = 0; i < k; ++i) {
        for ( j = 0; j < d-1; ++j) {
            fprintf(fptr, "%.4f",centroids[i][j]);
            fprintf(fptr, ",");
        }
        fprintf(fptr, "%.4f",centroids[i][d-1]);
        fprintf(fptr, "\n");
    }
    fclose(fptr);
}

void free_2d_array(double **array, int rows) {
    int row;

    for ( row = 0; row < rows; row++){
        free(array[row]);
    }
    free(array);
}

void free_3d_array(double ***array, int a,int b) {
    int row1,row2;

    for ( row1 = 0; row1 < a; row1++){
        for( row2 = 0;row2 < b; row2++){
            free(array[row1][row2]);
        }
        free(array[row1]);
    }
    free(array);
}

void set_equal_2d_array(double **new,double **current,int rows,int columns){
    int i,j;

    for( i=0;i<rows;i++){
        for( j=0;j<columns;j++){
            new[i][j] = current[i][j];
        }
    }
}

void zero_2d_array(double **array,int rows,int columns){
    int i,j;

    for( i=0;i<rows;i++){
        for( j=0;j<columns;j++){
            new_centroids[i][j] = 0;
        }
    }
}

void zero_3d_array(double ***array,int x,int y,int z){
    int i,j,m;

    for( i=0;i<x;i++){
        for( j=0;j<y;j++){
            for( m=0;j<y;j++){
                array[i][j][m] = 0;
            }
        }
    }
}
        