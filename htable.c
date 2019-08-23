#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "htable.h"
#include "mylib.h"

static unsigned int htable_word_to_int(char *word){
    unsigned int result = 0;

    while(*word != '\0') {
        result = (*word++ + 31 *result);
    }
    return result;
}

struct htablerec {
    int num_keys;
    int capacity;
    char **keys;
    int *frequencies;
};

htable htable_new(int capacity){
    htable result = emalloc(sizeof *result);
    result->capacity = capacity;
    result->num_keys = 0;
    result->frequencies = emalloc(capacity * sizeof result->frequencies[0]);
    result->keys = emalloc(capacity * sizeof result->keys[0]);
    return result;
}

void htable_free(htable h){
    int i;
    for (i = 0; i < h->capacity; i++){
        if (h->keys[i] != NULL){
            free(h->keys[i]);
        }
    }
    free(h->keys);
    free(h->frequencies);
    free(h);
}

int htable_insert(htable h, char *str){
    volatile unsigned int wordint = htable_word_to_int(str);
    volatile int insert_pos = wordint % h->capacity;
    int i = 0;

    while(NULL != h->keys[insert_pos] && strcmp(h->keys[insert_pos], str) != 0 && i < h->capacity){
        insert_pos = (insert_pos+1) % h->capacity;
        i++;
    }
    if(NULL == h->keys[insert_pos]){
        h->keys[insert_pos] = emalloc(sizeof str);
        strcpy(h->keys[insert_pos], str);
        h->frequencies[insert_pos] = 1;
        h->num_keys++;
        return 1;
    }else if (strcmp(h->keys[insert_pos], str) == 0){
        h->frequencies[insert_pos]++;
        return h->frequencies[insert_pos];
    }else{
        return 0;
    }
}

void htable_print(htable h, FILE *stream){
    int i, j = 0;

    for(i = 0; i < h->capacity; i++){
        j = 0;
        if(NULL != h->keys[i]){
            fprintf(stream, "%d ", h->frequencies[i]);
            while(h->keys[i][j] != '\0'){
                fprintf(stream, "%c", h->keys[i][j]);
                j++;
            }
            fprintf(stream, "\n");
        }
    }
}

int htable_search(htable h, char *key){
    int collisions = 0;
    unsigned int keyint = htable_word_to_int(key);
    int search_pos = keyint % h->capacity;

    while (h->keys[search_pos] != NULL && strcmp(h->keys[search_pos], key) != 0 && collisions < h->capacity){
        search_pos = (search_pos + 1) % h->capacity;
        collisions++;
    }
    if (collisions == h->capacity){
        return 0;
    }else{
        return h->frequencies[search_pos];
    }
}













