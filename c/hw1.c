#include <stdio.h>
#include <assert.h>

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

int read_file(char* input, int k){
    FILE *ifp = NULL;
    ifp = fopen(input,"r");
    assert(ifp!=NULL);

    fseek(ifp, 0, SEEK_END); // seek to end of file
    long size = ftell(ifp); // get current file pointer
    fseek(ifp, 0, SEEK_SET);

    char buff[254];
    fgets(buff, 255, (FILE*)ifp);
    printf("print : %s\n", buff);
    // fclose( ifp );
}

int main(int argc, char *argv[])  {
    int k;
    int max_iter;
    
    if (argc == 5){
        sscanf(argv[1],"%d",&k);
        sscanf(argv[2],"%d",&max_iter);
        void* input_file = argv[3];
        void* output_file = argv[4];
    }
    else if (argc == 4){
        sscanf(argv[1],"%d",&k);
        max_iter = 200;
        void* input_file = argv[3];
        void* output_file = argv[4];
        
    }
    else {
        return 0;
    }
    read_file(argv[3],k);

    return 1;
}

