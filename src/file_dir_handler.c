#include "mysync.h"

file_info_t *files = NULL;                      /* Array of file_info_t structs */
directory_info_t *directory_array = NULL;       /* Array of directory_info_t structs */
int n_files = 0;                                /* Number of files in files array */
int n_directories = 0;                          /* Number of directories in directory_array */

/* Function to print all information on elements in directory_array */
void print_directory_array() {
	for (int i = 0; i < n_directories; i++) {
		printf("Directory: %s\n", directory_array[i].dir_path);
		for (int j = 0; j < directory_array[i].n_files; j++) {
			printf("File: %s\n", directory_array[i].files[j].filename);
		}
	}
}

uint8_t directory_already_exists(char *dir_path) {
	for (int i = 0; i < n_directories; i++) {
		if (strcmp(directory_array[i].dir_path, dir_path) == 0) {
			return true;
		}
	}
	return false;
}

void scan_directories(char **directories, int num_directories) {
	DIR *dirp;                                  /* Pointer to directory stream */
	struct dirent *dp;                          /* Pointer to directory entry */
	
	/* Iterate over directories array */
	for (int i = 0; i < num_directories; i++) {
		dirp = opendir(directories[i]);
		if (dirp == NULL) {
			fprintf(stderr, "Error opening directory %s\n", directories[i]);
			continue;
		}

		printf("Regular files in directory %s:\n", directories[i]);

		/* Iterate over directory entries */
		while ((dp = readdir(dirp)) != NULL) {
			struct stat file_stat;              /* Struct to store current file status */
			char filepath[1024];

			char *filename = dp->d_name;
			IGNORE_HIDDEN_OPT_A(dp->d_name);    /* Ignore hidden files or directories if -a is not set */

			sprintf(filepath, "%s/%s", directories[i], filename);   /* Construct full path to file */
			
			/* Get file attributes */
			if (stat(filepath, &file_stat) == -1) {
				fprintf(stderr, "Error getting file status for %s\n", filepath);
				continue;
			}

			/* Check if file is a regular file */
			if (S_ISREG(file_stat.st_mode)) {
				printf("%s\n", filename);
				/* Expand files array */
				files = realloc(files, (n_files + 1) * sizeof(file_info_t));
				CHECK_ALLOC(files);
				/* Add file to files array */
				files[n_files].filename = strdup(filename);
				CHECK_ALLOC(files[n_files].filename);
				files[n_files].directory = strdup(directories[i]);
				CHECK_ALLOC(files[n_files].directory);
				files[n_files].mtime = file_stat.st_mtime;

				if (directory_already_exists(files[n_files].directory) == false) {
					/* Expand directory_array */
					directory_array = realloc(directory_array, (n_directories + 1) * sizeof(directory_info_t));
					CHECK_ALLOC(directory_array);
					/* Add directory to directory_array */
					directory_array[n_directories].dir_path = strdup(files[n_files].directory);
					CHECK_ALLOC(directory_array[n_directories].dir_path);
					directory_array[n_directories].n_files = 0;
					directory_array[n_directories].files = NULL;
					++n_directories;
				} else {
					for (int i = 0; i < n_directories; i++) {
						if (strcmp(directory_array[i].dir_path, files[n_files].directory) == 0) {
							directory_array[i].files = realloc(directory_array[i].files, (directory_array[i].n_files + 1) * sizeof(file_info_t));
							CHECK_ALLOC(directory_array[i].files);
							directory_array[i].files[directory_array[i].n_files] = files[n_files];
							++directory_array[i].n_files;
						}
					}
				}
				++n_files;
				
			}
		}

		closedir(dirp);
	}
	print_directory_array();
}

