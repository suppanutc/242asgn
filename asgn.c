#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <getopt.h>
#include "mylib.h"
#include "htable.h"

/**
 * A printing function to pass to htable_print.
 *
 * @param freq - the frequancy of the word.
 * @param word - the word to print.
 */
static void print_info(int freq, char *word) {
	printf("%-4d %s\n", freq, word);
}

/**
 * Creates a hash table and fills it with input. Outputs information
 * about the table based on the option input.
 */
int main(int argc, char **argv){
	int capacity = 113; /* capacity defaults to 113 */
	htable h;
	FILE *infile;
	clock_t start_f, start_s, end_f, end_s;
	double fill_t = 0, search_t = 0;
	char word[256], option;
	const char *optstring = "c:deps:t:h";
	hashing_t method = LINEAR_P;
	int print_entire_table = 0, print_stats = 0, snapshots = 0, 
	unknownCount = 0, spellcheck = 0; /* all to track options input */
	infile = NULL;

	while ((option = getopt(argc, argv, optstring)) != EOF) {
		switch (option) {
			case 'c':
				spellcheck = 1;
				if(NULL == (infile = fopen(optarg, "r"))){
					fprintf(stderr, "%s: can't find file %s\n", argv[0], argv[optind]);
					return EXIT_FAILURE;
				}
				break;
			case 'd':
				method = DOUBLE_H;
				break;
			case 'e': 
				print_entire_table = 1;
				break;
			case 'p': 
				print_stats = 1; 
				break;
			case 's':
				snapshots = atoi(optarg);
				break;
			case 't':
				capacity = next_prime(atoi(optarg));
				break;
			case 'h':
				fprintf(stderr, "Usage: ./asgn [OPTION]... <STDIN>\n\n");
				fprintf(stderr, "Perform various operations using a hash table.  By default, words are\n");
				fprintf(stderr, "read from stdin and added to the hash table, before being printed out\n");
				fprintf(stderr, "alongside their frequencies to stdout.\n\n");
				fprintf(stderr, " -c FILENAME  Check spelling of words in FILENAME using words\n");
				fprintf(stderr, "              from stdin as dictionary.  Print unknown words to\n");
				fprintf(stderr, "              stdout, timing info & count to stderr (ignore -p)\n");
				fprintf(stderr, " -d           Use double hashing (linear probing is the default)\n");
				fprintf(stderr, " -e           Display entire contents of hash table on stderr\n");
				fprintf(stderr, " -p           Print stats info instead of frequencies & words\n");
				fprintf(stderr, " -s SNAPSHOTS Show SNAPSHOTS stats snapshots (if -p is used)\n");
				fprintf(stderr, " -t TABLESIZE Use the first prime >= TABLESIZE as htable size\n\n");
				fprintf(stderr, " -h           Display this message\n");
				return EXIT_SUCCESS;
			default :
				fprintf(stderr, "Usage: ./asgn [OPTION]... <STDIN>\n\n");
				fprintf(stderr, "Perform various operations using a hash table.  By default, words are\n");
				fprintf(stderr, "read from stdin and added to the hash table, before being printed out\n");
				fprintf(stderr, "alongside their frequencies to stdout.\n\n");
				fprintf(stderr, " -c FILENAME  Check spelling of words in FILENAME using words\n");
				fprintf(stderr, "              from stdin as dictionary.  Print unknown words to\n");
				fprintf(stderr, "              stdout, timing info & count to stderr (ignore -p)\n");
				fprintf(stderr, " -d           Use double hashing (linear probing is the default)\n");
				fprintf(stderr, " -e           Display entire contents of hash table on stderr\n");
				fprintf(stderr, " -p           Print stats info instead of frequencies & words\n");
				fprintf(stderr, " -s SNAPSHOTS Show SNAPSHOTS stats snapshots (if -p is used)\n");
				fprintf(stderr, " -t TABLESIZE Use the first prime >= TABLESIZE as htable size\n\n");
				fprintf(stderr, " -h           Display this message\n");

				return EXIT_FAILURE;	
		}
	}

	h = htable_new(capacity, method); 
	while (getword(word, sizeof word, stdin) != EOF){
		start_f = clock();
		htable_insert(h, word);
		end_f = clock();
		fill_t += ((end_f - start_f)/(double)CLOCKS_PER_SEC);
	}

	

	if (print_entire_table == 1) {
		htable_print_entire_table(h, stderr); 
	}

	if(spellcheck == 1){
		start_s = clock();
		while(getword(word, sizeof word, infile) != EOF) {
			if(htable_search(h, word) == 0){ 
				unknownCount++; 						
				fprintf(stdout, "%s\n", word);
			}
		}
       		end_s = clock();
	
	        search_t = ((end_s - start_s)/(double)CLOCKS_PER_SEC);
        	/** Print timing information and unknown word count. */
        	fprintf(stderr, "Fill time     : %2.6f\n", fill_t);
        	fprintf(stderr, "Search time   : %2.6f\n", search_t);
        	fprintf(stderr, "Unknown words = %d\n", unknownCount);
    	} else if (print_stats == 1 && snapshots > 0) {
		htable_print_stats(h, stdout, snapshots);
	} else if (print_stats == 1){
		htable_print_stats(h, stdout, 10); 
	}

	if (print_stats == 0 && spellcheck == 0){
		htable_print(h, print_info);
	}
	

	htable_free(h);

	return EXIT_SUCCESS;
}
