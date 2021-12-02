#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>


#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2
double ** data_frame(const char *file);
void find_vectors_len(FILE *fp);
void find_d_of_vector(FILE *fp);
char* concat(const char *s1, const char *s2);
int read_file(char* input, int k);
void write_to_output_file(char *input);
int find_closets_cluster();
void init_centroids();
int algorithm();
void validate(int condition);
int is_number(char* num);
int isdigit(char digit);
void find_rows_and_columns();
void init_data_points();

int k, max_iter,num_rows,d=1,num_in_row,vector_len;
char *input_file, *output_file;
double **data_points,centroids;

/*
 * write func that creates data frame of vectors(array[]][])
 */
double ** data_frame(const char *file){
    FILE *ifp = NULL;
    ifp = fopen("C:\\Users\\aviva\\CLionProjects\\tec_project_1\\c\\input1.txt" ,"r");
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
    char *result = malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator
    // in real code you would check for errors in malloc here
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}


int main(int argc, char *argv[]) {
    data_frame(argv[3]);

//    printf("%d", is_number("123"));
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

    while(max_iter > 0){



        max_iter--;

    }

    return 1;
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
