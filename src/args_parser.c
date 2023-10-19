#include "mysync.h"
#include <sys/stat.h>

/* DECLARE GLOBAL VARIABLES */
Config CONFIG;									/* Struct to store option flag values */

/* 
 * Parses option flags and sets the corresponding values in the CONFIG struct.
 *
 * @param argc The number of arguments passed to the program.
 * @param argv An array of strings containing the arguments passed to the program.
 */
void parse_opts(int argc, char* argv[]) {

int opt;										/* Stores the current option flag */								

	while ((opt = getopt(argc, argv, "ai:no:prv")) != -1) {
		switch (opt) {
			case 'a':							/* Corresponds to -a: sync all files */
				CONFIG.all_files = true;
				break;
			case 'i':							/* Corresponds to -i: "include" pattern*/							
				if (CONFIG.include_pattern) {
					fprintf(stderr, "Multiple -i options are not allowed.\n");
					usage();
					exit(EXIT_FAILURE);
				}
				CONFIG.include_pattern = optarg;
				break;
			case 'n':							/* Corresponds to -n: dry run */
				CONFIG.dry_run = true;
				CONFIG.verbose = true;  
				break;
			case 'o':							/* Corresponds to -o: "only" pattern */
				if (CONFIG.only_pattern) {
					fprintf(stderr, "Multiple -o options are not allowed.\n");
					usage();
					exit(EXIT_FAILURE);
				}
				CONFIG.only_pattern = optarg;
				break;
			case 'p':							/* Corresponds to -p: preserve file metadata */
				CONFIG.preserve = true;
				break;
			case 'r':							/* Corresponds to -r: sync directories recursively */
				CONFIG.recursive = true;
				break;
			case 'v':							/* Corresponds to -v: verbose output */
				CONFIG.verbose = true;
				break;
			default:
				/* Invalid option */
				fprintf(stderr, "Invalid option: -%c\n", opt);
				usage();
				exit(EXIT_FAILURE);
		}
	}
}

/*
 * Parses command line arguments and returns a struct containing the parsed values.
 *
 * @param argc The number of arguments passed to the program.
 * @param argv An array of strings containing the arguments passed to the program.
 */
void parse_args(int argc, char* argv[]) {
	
	parse_opts(argc, argv);						/* Parse option flags */
	
	/* Check for correct number of command-line arguments */
	if ((argc - optind) < 2) {
		printf("%d\n", optind);
		printf("%d\n", argc);
		fprintf(stderr, "Incorrect number of arguments.\n");					
		usage();
		exit(EXIT_FAILURE);
	}

	/* Ensure arguments are readable directory paths */
	struct stat path_stat;
	for (int i = optind; i < argc; i++) {
		if (stat(argv[i], &path_stat) != 0 || !S_ISDIR(path_stat.st_mode) || access(argv[i], R_OK) != 0) {
			perror("Error while parsing command-line arguments");
			usage();
			exit(EXIT_FAILURE);
		}
	}
}

