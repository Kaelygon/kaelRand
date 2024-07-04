//kaelMan.h
//Print manual and obtain arguments
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Structure to hold option information
typedef struct {
	unsigned char 		flag_decimal;
	unsigned char 		flag_debug;

	unsigned char 		flag_generate;
	unsigned long long 	generate_value;

	unsigned char 		flag_seed;
	unsigned long long  seed_value;

	unsigned char 		flag_stateSize;
	size_t			    stateSize_value;
} Options;

// Function to print usage information
void print_usage() {
	printf("Usage: kaelRand [options]\n");
	printf("Options:\n");
	printf("  -h,             Show this message\n");
	printf("  -g [amount]     Generate number of uint64\n");
    printf("                  0 = Generate indefinitely\n\n");
	printf("  -d              Output as decimal\n");
	printf("  -b              Output as binary\n");
	printf("  --debug         Debug information\n");
	printf("  --seed [seed]   Starting seed\n");
	printf("  --states [size] State array size in uint64\n");
}

// Function to parse command-line arguments and populate options
void parse_arguments(int argc, char *argv[], Options *opts) {
	opts->flag_decimal		= 0;
	opts->flag_debug		= 0;
	opts->flag_generate		= 0;
	opts->generate_value	= 0;
	opts->flag_seed			= 1;
	opts->seed_value		= 13238717;
	opts->flag_stateSize	= 1;
	opts->stateSize_value	= 16;

	for (int i = 1; i < argc; ++i) {
		if (strcmp(argv[i], "-h") == 0) {
			print_usage();
			exit(EXIT_SUCCESS);
		} else if (strcmp(argv[i], "-d") == 0) {
			opts->flag_decimal = 1;
		} else if (strcmp(argv[i], "--debug") == 0) {
			opts->flag_debug = 1;
		} else if (strcmp(argv[i], "-g") == 0) {
			if (i + 1 < argc) {
				opts->flag_generate = 1;
				opts->generate_value = atoi(argv[i + 1]);
				i++;  // skip the next argument
			} else {
				printf("Error: Missing value for %s\n", argv[i]);
				print_usage();
				exit(EXIT_FAILURE);
			}
		}else if (strcmp(argv[i], "--seed") == 0) {
			if (i + 1 < argc) {
				opts->flag_seed = 1;
				opts->seed_value = atoi(argv[i + 1]);
				i++;  // skip the next argument
			} else {
				printf("Error: Missing value for %s\n", argv[i]);
				print_usage();
				exit(EXIT_FAILURE);
			}
		}else if (strcmp(argv[i], "--states") == 0) {
			if (i + 1 < argc) {
				opts->flag_stateSize = 1;
				opts->stateSize_value = atoi(argv[i + 1]);
				i++;  // skip the next argument
			} else {
				printf("Error: Missing value for %s\n", argv[i]);
				print_usage();
				exit(EXIT_FAILURE);
			}
		}
		 else if (strcmp(argv[i], "-b") == 0) {
			//binary as default
		} else {
			printf("Unknown option or parameter: %s\n", argv[i]);
			print_usage();
			exit(EXIT_FAILURE);
		}
	}
}