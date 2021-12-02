#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>


#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

#define SQ(x) x * x


int read_file(char* input, int k);
void write_to_output_file();
int find_closets_cluster();
void init_centroids();
int algorithm();
void validate(int condition);
int is_number(char* num);
int isdigit(char digit);
void find_rows_and_columns();
void init_data_points();

int k, max_iter,num_rows,d,num_in_row,vector_len;
char *input_file, *output_file;
double **data_points,**centroids,***clusters;


char* concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator
    // in real code you would check for errors in malloc here
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

int find_closets_cluster(int *data_point){
    double *difference;
    difference = calloc(k, sizeof(int *));
    for(int i=0 ; i<k ; i++){
        double *current_mu = centroids[i];
        double sum=0;
        for(int j=0;j<d;j++){
            sum += (data_point[j]-current_mu[j]);
        }
        sum = SQ(sum);
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



int main(int argc, char *argv[])  {

    printf("%d", is_number("123"));
    printf("start\n");

    validate(argc == 4 || argc == 5);

    sscanf(argv[1],"%d",&k);

    if (is_number(argv[2])){
        sscanf(argv[2],"%d",&max_iter);
    } else{
        max_iter = 200;
    }
    sscanf(argv[argc-2],"%s",input_file);
    sscanf(argv[argc-1],"%s",output_file);

    return algorithm();
}

int algorithm(){
    double epsilon = 0.001;
    read_file(input_file,k);

    int N = num_rows;
    while(max_iter > 0){
        clusters = calloc(k,sizeof(double **));
        for (int i=0;i<N;i++){
            double *x_i = data_points[i];
            int index = find_closets_cluster(x_i);
            // clusters[index].append(x_i);
        }
        double **new_centorits = calloc(k,sizeof(double *));
        for(int i=0;i<k;i++){
            double **cluster = clusters[i];
            int len_cluster = sizeof(cluster)/sizeof(cluster[0]);
            double *current_centroid = calloc(len_cluster,sizeof(double));
            for (int j=0;j<len_cluster;j++){
                double *data = cluster[j];
                 int len = sizeof(data)/sizeof(data[0]);
                for(int m=0;m<len;m++){
                    current_centroid[m] += data[m];
                }
            }
            //new_centroids[i] = [current_centroid[i] / len_cluster for i in range(len(current_centroid))]
            int len = sizeof(current_centroid)/sizeof(current_centroid[0]);
            for(int j=0;j<len;j++){
                for(int m=0;m<len;m++){
                    new_centorits[j][m] = current_centroid[m] / len_cluster;

                }
            }
        }
        max_iter--;
        //diff_centroids = [[lst1[i] - lst2[i] for i in range(len(lst1))] for lst1, lst2 in zip(centroids, new_centroids)]
        double ** diff_centroids = calloc(k,sizeof(double *));
        for(int i=0;i<k;i++){
            double *lst1 = centroids[i];
            double *lst2 = new_centorits[i];
            int len = sizeof(lst1)/sizeof(lst1[0]);
            double *diff_lst = calloc(len,sizeof(double));
            for(int j=0;j<len;j++){
                diff_lst[j] = lst1[j] - lst2[j];
            }
        }
        //diff_centroids = [sum([num**2 for num in diff_centroids[i]])**0.5 for i in range(len(diff_centroids))]
        int len = sizeof(diff_centroids)/sizeof(diff_centroids[0]);
        double *sum_diff_centroids = calloc(k,sizeof(double));
        for(int i=0;i<k;i++){
            double *data_point = diff_centroids[i];
            double sum=0;
            for(int j=0;j<len;j++){
                sum += SQ(data_point[j]);
            }
            double normal = sqrt(sum);
            sum_diff_centroids[i] = normal;
        }
        double max=sum_diff_centroids[0];
        for(int i=0;i<k;i++){
            if(sum_diff_centroids[i]>max){
                max = sum_diff_centroids[i];
            }
        }
        if(max<epsilon){
            return 1;
        }
        centroids = new_centorits;
        write_to_output_file();

    }

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

int is_number(char s[])
{
    for (int i = 0; s[i]!= '\0'; i++)
    {
        if (isdigit(s[i]) == 0)
            return 0;
    }
    return 1;
}

int isdigit(char digit){
    if (digit < '0' || digit > '9')
        return 0;
    return 1;
}

int read_file(char* input, int k){
    FILE *ifp = NULL;
    ifp = fopen(concat("../",input),"r");
    assert(ifp!=NULL);

    fseek(ifp, 0, SEEK_END); // seek to end of file
    long size = ftell(ifp); // get current file pointer
    fseek(ifp, 0, SEEK_SET);

    find_rows_and_columns();





    char buff[254];
//    fgets(buff, 255, (FILE*)ifp);
    while(1) {
        int c = fgetc(ifp);
        if(feof(ifp)) {
            break ;
        }
        printf("%c", c);
    }
//    printf("print : %s\n", buff);
    fclose( ifp );
}

void validate(int condition){
    if(!condition){
        printf("invalid input\n");
        exit(1);
    }
}

void find_rows_and_columns(){

}



