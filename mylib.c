#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include "mylib.h"

/** 
 * Allocates memory and checks if the memory was allocated.
 *
 * @param s - the size of memory to allocate.
 */
void *emalloc(size_t s) {
    void *result = malloc(s);
    if (NULL == result) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    return result;
}

/** 
 * Reallocates memory and checks if the memory was reallocated. 
 *
 * @param p - a pointer to the memory to realloc.
 * @param s - the size of memory to realloc.
 */
void *erealloc(void *p, size_t s) {
    void *result = realloc(p, s);
    if (NULL == result) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    return result;
}

/** 
 * Gets a word from the given stream.
 *
 * @param s - a pointer to the memory to store the word in.
 * @param limit - the size of the memory given by s.
 * @stream - a stream to read from.
 *
 * @returns - EOF if the end of file is reached.
 */
int getword(char *s, int limit, FILE *stream) {
    int c;
    char *w = s;
    assert(limit > 0 && s != NULL && stream != NULL);

    /* skip to the start of the word */
    while (!isalnum(c = getc(stream)) && EOF != c)
        ;
    if (EOF == c) {
        return EOF;
    }else if (--limit > 0) { /* reduce limit by 1 to allow for the \0 */
        *w++ = tolower(c);
    }
    while (--limit > 0) {
        if (isalnum(c = getc(stream))) {
            *w++ = tolower(c);
        }else if ('\'' == c) {
            limit++;
        }else {
            break;
        }
    }
    *w = '\0';
    return w - s;
}

/** Returns the smallest prime number greater than x.
 *
 * @param x - the number to be greater than.
 * 
 * @returns - the prime number.
 */
int next_prime(int x) {
    int i, is_prime = 0;
    while(is_prime == 0){
	if (x % 2 == 0){
	    x++;
	}else{
	    is_prime = 1;
	    for(i = 3; i < x; i+=2){
		if (x % i == 0){
		    x++;
		    is_prime = 0;
		    break;
		}
	    } 
	}
    }
    return x;
}
