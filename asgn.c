#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <getopt.h>
#include "mylib.h"
#include "htable.h"

static void print_info(int freq, char *word) {
	printf("%-4d %s\n", freq, word);
}

int main(int argc, char **argv){
	int capacity = 113;
	char *p; /*variable required for strtol function, unutilised */
	htable h;
	FILE *infile, *outfile;
	clock_t start_f, start_s, end_f, end_s;
	double fill_t, search_t;
	char word[256];
	const char *optstring = "c:deps:t:h";
	char option, *new_capacity;
	hashing_t method = LINEAR_P;
	int print_entire_table, print_stats, snapshots, snapshots_num,
	 unknownCount, default_case, spellcheck = 0;
	outfile = NULL;
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
			case 'e': //do this call print_entire_table
				print_entire_table = 1;
				break;
			case 'p': //do this
				print_stats = 1; // no break because, it is used with s.
				if (atoi(argv[4]) == 's') {
					snapshots = 1;
					snapshots_num = atoi(argv[5]);
				}
				break;
			case 't':
				new_capacity = optarg;
				capacity = atoi(new_capacity);
				break;
			case 'h':
				printf("Help");
				break;
			default :
				printf("Default");
				default_case = 1;
		}

		h = htable_new(capacity, method); //fill table up with "method" way.
		while (getword(word, sizeof word, stdin) != EOF){
				htable_insert(h, word);
		}

		if (print_stats == 1 && snapshots == 1) {
			htable_print_stats(h, stdout, snapshots_num);
		} else {
			htable_print_stats(h, stdout, 1); //standard case, if -s wasn't passed.
		}
		if (print_entire_table == 1) {
			htable_print_entire_table(); //haven't fill argument.
		}
		if(spellcheck == 1){
        start_s = clock();
        while(getword(word, sizeof word, infile) != EOF) {
            if(htable_search(h, word) != 1){ //pretty much a copy, only changed
                unknownCount++; 						//it to htable_search.
                fprintf(stdout, "%s\n", word);
            }
        }
        end_s = clock();

        fill_t = ((end_f - start_f)/(double)CLOCKS_PER_SEC);
        search_t = ((end_s - start_s)/(double)CLOCKS_PER_SEC);
        /** Print timing information and unknown word count. */
        fprintf(stderr, "\nStats:\n");
        fprintf(stderr, "Fill time     : %2.5f\n", fill_t);
        fprintf(stderr, "Search time   : %2.5f\n", search_t);
        fprintf(stderr, "Unknown words : %d\n", unknownCount);
    }
		if(default_case == 1) { //print normally if no option passed.
			htable_print(h, print_info);
		}
	}
	/*do stuff*/

	htable_free(h);

	return EXIT_SUCCESS;
}
