#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "htable.h"
#include "mylib.h"


int main(int argc, char **argv){
    int capacity = 101;
    char *p; /*variable required for strtol function, unutilised */
    htable h;
    char word[256];
    if(argc > 1){
        if ((capacity = strtol(argv[1], &p, 10)) == 0){
            fprintf(stderr, "arg 1 should be a number");
            return EXIT_FAILURE;
        }
    }
        
        
    h = htable_new(capacity, LINEAR_P);

    while (getword(word, sizeof word, stdin) != EOF){
        htable_insert(h, word);
    }
    
    htable_print_stats(h, stdout, 1);
    htable_free(h);
    return EXIT_SUCCESS;
}
