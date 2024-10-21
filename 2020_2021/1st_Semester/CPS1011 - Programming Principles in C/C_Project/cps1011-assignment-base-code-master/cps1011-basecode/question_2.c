#include "question_2.h"

#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

sparse_set_ptr ss_init(sparse_set_index_t capacity,sparse_set_elem_t max_value){
    if(capacity >= (max_value+1)){
        printf("Capacity is larger than the maximum value +1\n");
        return false;
    }
    sparse_set_ptr my_sparse_set = malloc(sizeof(sparse_set_t));
    my_sparse_set->ss_capacity = capacity;
    my_sparse_set->max_value= max_value;
    my_sparse_set->sparse_array = calloc(capacity, sizeof(sparse_set_index_t));
    my_sparse_set->dense_array = calloc((max_value+1), sizeof(sparse_set_elem_t));
    if(my_sparse_set == NULL){
        printf("malloc has failed\n");
        return false;
    }
    else{
        return my_sparse_set;
    }
}

bool ss_destroy(sparse_set_ptr sparse_set){
    sparse_set->max_value=0;
    sparse_set->current_no_elements=0;
    sparse_set->ss_capacity=0;
    free(sparse_set->sparse_array);
    free(sparse_set->dense_array);

    sparse_set->sparse_array = NULL;
    sparse_set->dense_array = NULL;
    if(sparse_set->dense_array == NULL && sparse_set->sparse_array == NULL) {
        return true;
    } else{
        return false;
    }
}

bool ss_contained(sparse_set_ptr sparse_set,sparse_set_elem_t element){
    bool found = false;
    if(sparse_set->current_no_elements !=0){
        for(int i=0; i <sparse_set->current_no_elements; i++){
            if(sparse_set->dense_array[i] == element){
                found = true;
            }
        }
        if(found){
            return true;
        } else {
            return false;
        }
    } else{
        return false;
    }
}

bool ss_add(sparse_set_ptr sparse_set,sparse_set_elem_t element){
    if(element > sparse_set->max_value || element < 0){
        printf("Element entered is not eligible for the set\n");
        return false;
    }
    else{
        if(ss_contained(sparse_set, element) == true){
            return true;
        }
        else{
            sparse_set->dense_array[sparse_set->current_no_elements] = element;
            sparse_set->sparse_array[element] = sparse_set->current_no_elements;
            sparse_set->current_no_elements++;//increment count of elements in set
            return true;
        }
    }
}

bool ss_remove(sparse_set_ptr sparse_set,sparse_set_elem_t element){
    if(sparse_set->current_no_elements !=0){
        if(ss_contained(sparse_set,element) == true){
            sparse_set->dense_array[sparse_set->sparse_array[element]] = sparse_set->dense_array[sparse_set->current_no_elements - 1];
            sparse_set->sparse_array[sparse_set->dense_array[sparse_set->current_no_elements - 1]] = sparse_set->sparse_array[element];
            sparse_set->current_no_elements--;
            return true;
        }else{
            return false;
        }
    } else {
        return false;
    }
}

bool ss_sort(sparse_set_ptr sparse_set){
    if(sparse_set->current_no_elements !=0) {
        int temporary_value;
        for (int i = 0; i < sparse_set->current_no_elements; i++) {
            for (int j = i + 1; j < sparse_set->current_no_elements; j++) {
                if (sparse_set->dense_array[i] > sparse_set->dense_array[j]) {
                    temporary_value = sparse_set->dense_array[i];
                    sparse_set->dense_array[i] = sparse_set->dense_array[j];
                    sparse_set->dense_array[j] = temporary_value;
                    sparse_set->sparse_array[i] = i;
                    sparse_set->sparse_array[j] = j;
                }
                return true;
            }
        }
    } else {
        return false;
    }
}

ssize_t ss_size(sparse_set_ptr sparse_set){
    if(sparse_set->current_no_elements != 0) {
        return sparse_set->current_no_elements;
    } else {
        return -1;
    }
}

bool ss_get_elem(sparse_set_ptr sparse_set,sparse_set_index_t sparse_index,sparse_set_elem_ptr dest){
    *dest=sparse_set->sparse_array[sparse_index];
    if(dest == NULL){
        return false;
    } else{
        return true;
    }
}

bool ss_print_status(sparse_set_ptr sparse_set,bool print_as_chars){
    if(print_as_chars != true){
        printf("Dense Array\n");
        for(int i=0;i<sparse_set->current_no_elements;i++){
            printf("%u",sparse_set->dense_array[i]);
        }
        printf("\n");
        printf("Sparse Array\n");
        for(int i=0;i<sparse_set->current_no_elements;i++){
            printf("%u", sparse_set->sparse_array[sparse_set->dense_array[i]]);
        }
        printf("\n");
    }else{
        printf("Dense Array\n");
        for(int i=0;i<sparse_set->current_no_elements;i++){
            printf("%c",sparse_set->dense_array[i]);
        }
        printf("\n");
        printf("Sparse Array\n");
        for(int i=0;i<sparse_set->current_no_elements;i++){
            printf("%u", sparse_set->sparse_array[sparse_set->dense_array[i]]);
        }
        printf("\n");
    }
}

//Q2 (d)
bool ss_union(sparse_set_ptr a,sparse_set_ptr b,sparse_set_ptr result){
    bool flag = false;
   for(int i=0; i< a->current_no_elements; i++){
       if(a->dense_array[i] <= result->max_value){
           ss_add(result, a->dense_array[i]);
       }
   }

   for(int i=0; i< b->current_no_elements; i++) {
       if (b->dense_array[i] <= result->max_value) {
           if (ss_contained(result, b->dense_array[i]) == false) {
               ss_add(result, b->dense_array[i]);
           }
       }
       flag = true;
   }
    if(result->dense_array == NULL){
           flag = false;
       }
       if(flag == true){
           return true;
       }else{
           return false;
       }
}

bool ss_intersection(sparse_set_ptr a,sparse_set_ptr b,sparse_set_ptr result){
    bool flag = false;
    for(int i=0;i<a->current_no_elements;i++){
        for(int j=0; j<b->current_no_elements;j++){
            if(a->dense_array[i] == b->dense_array[i]) {
                ss_add(result, a->dense_array[i]);
                break;
            }
        }
        flag = true;
    }
    if(result->dense_array == NULL){
        flag = false;
    }
    if(flag==true){
        return true;
    }else{
        return false;
    }
}

bool ss_difference(sparse_set_ptr a,sparse_set_ptr b,sparse_set_ptr result){
    bool flag = false;
    for(int i=0;i< a->current_no_elements;i++){
        if(ss_contained(b, a->dense_array[i])==false){
            ss_add(result, a->dense_array[i]);
        }
        flag = true;
    }

    if(flag == true){
        return true;
    }else{
        return false;
    }
}

