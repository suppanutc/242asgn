#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "htable.h"
#include "mylib.h"

struct htablerec {
    int num_keys;
    int capacity;
    char **keys;
    int *frequencies;
    int *stats;
    hashing_t method;
};

static unsigned int htable_word_to_int(char *word){
    unsigned int result = 0;

    while(*word != '\0') {
        result = (*word++ + 31 *result);
    }
    return result;
}

static unsigned int htable_step(htable h, unsigned int i_key) {
    return 1 + (i_key % (h->capacity - 1));
}

htable htable_new(int capacity, hashing_t method){
    htable result = emalloc(sizeof *result);
    result->capacity = capacity;
    result->num_keys = 0;
    result->frequencies = emalloc(capacity * sizeof result->frequencies[0]);
    result->keys = emalloc(capacity * sizeof result->keys[0]);
    result->method = method;
    result->stats = malloc(capacity * sizeof result->stats[0]); 
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
    unsigned int wordint = htable_word_to_int(str);
    int insert_pos = wordint % h->capacity;
    int i = 0;
    unsigned int step = htable_step(h, wordint);
    while(NULL != h->keys[insert_pos] && strcmp(h->keys[insert_pos], str) != 0 && i < h->capacity){
        if ( h->method == LINEAR_P ){
            insert_pos = (insert_pos+1) % h->capacity;
        }else {
            insert_pos = (insert_pos + step) % h->capacity;
        }
        i++;
    }

    h->stats[h->num_keys] = i;

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
            fprintf(stream, "%-4d ", h->frequencies[i]);
            while(h->keys[i][j] != '\0'){
                fprintf(stream, "%c", h->keys[i][j]);
                j++;
            }
            fprintf(stream, "\n");
        }
    }
}

void htable_print_entire_table(htable h, FILE *stream){
	int i = 0;
	for (i = 0; i < h->capacity; i++){
	     if(NULL != h->keys[i]){
 		fprintf(stream, "%5d %5d %5d   %s\n", i, h->frequencies[i], h->stats[i], h->keys[i]);
	     }else {
	        fprintf(stream, "%5d %5d %5d    \n", i, h->frequencies[i], h->stats[i]);
	     }
	}
}


int htable_search(htable h, char *key){
    int collisions = 0;
    unsigned int keyint = htable_word_to_int(key);
    int search_pos = keyint % h->capacity;
    unsigned int step = htable_step(h, keyint);

    while (h->keys[search_pos] != NULL && strcmp(h->keys[search_pos], key) != 0 && collisions < h->capacity){
        if(h->method == LINEAR_P){
            search_pos = (search_pos + 1) % h->capacity;
        } else {
            search_pos = (search_pos + step) % h->capacity;
        }
        collisions++;
    }
    if (collisions == h->capacity){
        return 0;
    }else{
        return h->frequencies[search_pos];
    }
}

/**
 * Prints out a line of data from the hash table to reflect the state
 * the table was in when it was a certain percentage full.
 * Note: If the hashtable is less full than percent_full then no data
 * will be printed.
 *
 * @param h - the hash table.
 * @param stream - a stream to print the data to.
 * @param percent_full - the point at which to show the data from.
 */
static void print_stats_line(htable h, FILE *stream, int percent_full) {
   int current_entries = h->capacity * percent_full / 100;
   double average_collisions = 0.0;
   int at_home = 0;
   int max_collisions = 0;
   int i = 0;

   if (current_entries > 0 && current_entries <= h->num_keys) {
      for (i = 0; i < current_entries; i++) {
         if (h->stats[i] == 0) {
            at_home++;
         } 
         if (h->stats[i] > max_collisions) {
            max_collisions = h->stats[i];
         }
         average_collisions += h->stats[i];
      }
    
      fprintf(stream, "%4d %10d %11.1f %10.2f %11d\n", percent_full, 
              current_entries, at_home * 100.0 / current_entries,
              average_collisions / current_entries, max_collisions);
   }
}

/**
 * Prints out a table showing what the following attributes were like
 * at regular intervals (as determined by num_stats) while the
 * hashtable was being built.
 *
 * @li Percent At Home - how many keys were placed without a collision
 * occurring.
 * @li Average Collisions - how many collisions have occurred on
 *  average while placing all of the keys so far.
 * @li Maximum Collisions - the most collisions that have occurred
 * while placing a key.
 *
 * @param h the hashtable to print statistics summary from.
 * @param stream the stream to send output to.
 * @param num_stats the maximum number of statistical snapshots to print.
 */
void htable_print_stats(htable h, FILE *stream, int num_stats) {
   int i;

   fprintf(stream, "\n%s\n\n", 
           h->method == LINEAR_P ? "Linear Probing" : "Double Hashing"); 
   fprintf(stream, "Percent   Current    Percent    Average      Maximum\n");
   fprintf(stream, " Full     Entries    At Home   Collisions   Collisions\n");
   fprintf(stream, "------------------------------------------------------\n");
   for (i = 1; i <= num_stats; i++) {
      print_stats_line(h, stream, 100 * i / num_stats);
   }
   fprintf(stream, "------------------------------------------------------\n\n");
}












