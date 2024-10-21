#include <stdio.h>

#include "question_1.h"
#include "question_2.h"
#include "string.h"

#define BUFFER_SIZE 999

int main(void)
{
    //first sparse set for (i)
    char buffer1[BUFFER_SIZE] = {0};
    get_text_from_user(buffer1, BUFFER_SIZE);
    sparse_set_ptr sparse1 = ss_init(strlen(buffer1),127);

    //second sparse set for (i)
    char buffer2[BUFFER_SIZE] = {0};
    get_text_from_user(buffer2, BUFFER_SIZE);
    sparse_set_ptr sparse2 = ss_init(strlen(buffer2),127);

    for(int i=0; i<strlen(buffer1);i++){
        if(buffer1[i]>=0 && buffer1[i]<=127){
            ss_add(sparse1,buffer1[i]);
        }
    }

    for(int i=0; i<strlen(buffer2);i++){
        if(buffer2[i]>=0 && buffer2[i]<=127){
            ss_add(sparse2,buffer2[i]);
        }
    }

    ss_print_status(sparse1, true);
    ss_print_status(sparse2, true);

    //sparse set for (ii)
    sparse_set_ptr sparse3 = ss_init(128,127);

    ss_union(sparse1,sparse2,sparse3);
    ss_print_status(sparse3, true);

    //sparse set for (iii)
    sparse_set_ptr sparse4 = ss_init(128,127);

    ss_intersection(sparse1,sparse2,sparse4);
    ss_print_status(sparse4,true);

    //sparse set for (iv)
    sparse_set_ptr sparse5 = ss_init(128,127);

    ss_difference(sparse1, sparse2, sparse5);
    ss_print_status(sparse5, true);
}

bool ss_populate_from_file(const char *filename,sparse_set_ptr result){
    FILE *fp;
    char content[128];
    fp = fopen(filename, "r");
    if (fp ==NULL){
        printf("File could not be opened");
        return false;
    }
    int i=0;
    while(fscanf(fp,"%s", content)!=EOF){
        for(int j=0;j<strlen(content);j++){
            ss_add(result,content[j]);
            j++;
        }
        i++;
    }
    fclose(fp);
    return true;
}
