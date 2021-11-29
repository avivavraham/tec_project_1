#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>


#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

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

int k, max_iter,num_rows,num_columns,num_in_row,vector_len;
char *input_file, *output_file;
double **data_points,centroids;


char* concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator
    // in real code you would check for errors in malloc here
    strcpy(result, s1);
    strcat(result, s2);
    return result;
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



