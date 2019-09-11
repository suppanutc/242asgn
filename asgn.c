#include <stdio.h>
#include <stdlib.h>
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
	char word[256];
	const char *optstring = "c:deps:t:h";
	char option;

	while ((option = getopt(argc, argv, optstring)) != EOF) {
		switch (option) {
			case 'c':
			case 'd':
			case 'e':
			case 'p':
			case 's':
			case 't':
			case 'h':
			default:
		}
	}
	/*do stuff*/
	h = htable_new(capacity, LINEAR_P);

	while (getword(word, sizeof word, stdin) != EOF){
			htable_insert(h, word);
	}
	/* not working, just reminder */
	htable_print(h, (print_info(h->frequencies, word)));

	htable_free(h);
	return EXIT_SUCCESS;
}
