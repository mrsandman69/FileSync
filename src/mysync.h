/* C STANDARD LIBRARY HEADER FILES*/
#include <dirent.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <fnmatch.h>
#include <stdarg.h>

#define CHECK_ALLOC(x) 							if (x == NULL) {perror(__func__); exit(EXIT_FAILURE);}
#define IGNORE_HIDDEN_OPT_A(x) 					if (CONFIG.all_files == false && x[0] == '.') {continue;}

/* DEFINE CUSTOM STRUCTURES */

typedef struct {
	char *path;
	struct Node *next; 
} Node;

typedef struct {
	bool all_files;                        		/* Corresponds to -a */
	char *include_pattern;                     	/* Corresponds to -i */
	char *only_pattern;                        	/* Corresponds to -o */
	bool dry_run;                              	/* Corresponds to -n */
	bool preserve;                             	/* Corresponds to -p */
	bool recursive;                            	/* Corresponds to -r */
	bool verbose;                              	/* Corresponds to -v */
} Config;

typedef struct {
	char *filename;
	char *directory;
	time_t mtime;
	mode_t mode;
} file_info_t;

typedef struct {
	int n_files;
	char *dir_path;
	file_info_t *files;
} directory_info_t;


/* DECLARE GLOBAL VARIABLES */
extern Config CONFIG;

/* DECLARE FUNCTION PROTOTYPES */
extern void usage();							/* Prints usage instructions */
extern void verbose(char *, ...);				/* Prints to stdout if verbose flag is set */

extern void parse_args(int, char *[]);			/* Parses command line arguments */

extern void scan_directories(char **, int);		/* Lists regular files in directories */
extern void compare_directories();				/* Compares files in directories */
extern void compare_modified_time();			/* Compares modified time of files */

