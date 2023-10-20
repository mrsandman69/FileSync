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
			printf("File: %s\t\t", directory_array[i].files[j].filename);
			printf("Modified: %s\n", ctime(&directory_array[i].files[j].mtime));
		}
	}
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
		/* Expand directory_array */
		directory_array = realloc(directory_array, (n_directories + 1) * sizeof(directory_info_t));
		CHECK_ALLOC(directory_array);
		/* Add directory to directory_array */
		directory_array[n_directories].dir_path = strdup(directories[i]);
		CHECK_ALLOC(directory_array[n_directories].dir_path);
		directory_array[n_directories].n_files = 0;
		++n_directories;

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

				/* Update corresponding directory struct */
				directory_array[i].files = realloc(directory_array[i].files, (directory_array[i].n_files + 1) * sizeof(file_info_t));
				CHECK_ALLOC(directory_array[i].files);
				directory_array[i].files[directory_array[i].n_files] = files[n_files];
				CHECK_ALLOC(directory_array[i].files[directory_array[i].n_files].filename);
				++directory_array[i].n_files;

				/* Update global file count */
				++n_files;
			}
		}
		closedir(dirp);

	}
	print_directory_array();
	compare_directories();
	compare_modified_time();
}



void compare_directories() {
	/* Iterate over directories array */
	for (int i = 0; i < n_directories; i++) {
		printf("Checking directory %s for missing files...\n", directory_array[i].dir_path);

		/* Iterate over files in current directory */
		for (int j = 0; j < directory_array[i].n_files; j++) {
			char *filename = directory_array[i].files[j].filename;
			int file_missing = 1;   /* Flag to indicate if file is missing in any other directory */

			/* Iterate over other directories */
			for (int k = 0; k < n_directories; k++) {
				/* Skip current directory */
				if (k == i) {
					continue;
				}
				/* Check if file exists in other directory */
				for (int l = 0; l < directory_array[k].n_files; l++) {
					if (strcmp(filename, directory_array[k].files[l].filename) == 0) {
						file_missing = 0;
						break;
					}
				}
				/* If file is missing, perform copy operation from directory to target directory */
				if (file_missing) {
					if (!CONFIG.dry_run) {
						/* Construct full path to file */
						char src_path[1024];
						char dest_path[1024];
						sprintf(src_path, "%s/%s", directory_array[i].dir_path, filename);
						sprintf(dest_path, "%s/%s", directory_array[k].dir_path, filename);
						/* Copy file */
						copy_file(src_path, dest_path);
					}
					printf("Copy from %s to %s\n", directory_array[i].dir_path, directory_array[k].dir_path);
				}
			}
		}
	}
}

/*
* In future, we can still use this function for recursive mode
* because the directories array will contain all subdirectories
* of the original directories.
*/
void compare_modified_time() {
	/* Iterate over files array */
	for (int i = 0; i < n_files; i++) {
		char *filename = files[i].filename;
		time_t most_recent_mtime = files[i].mtime;
		char *most_recent_directory = files[i].directory;

		/* Iterate over other files */
		for (int j = 0; j < n_files; j++) {
			/* Skip current file */
			if (j == i) {
				continue;
			}
			/* Check if file has same name */
			if (strcmp(filename, files[j].filename) == 0) {
				/* Check if file has more recent modified time */
				if (files[j].mtime > most_recent_mtime) {
					most_recent_mtime = files[j].mtime;
					most_recent_directory = files[j].directory;
				}
			}
		}
		/* Copy most recent version of file to other directories */
		for (int k = 0; k < n_directories; k++) {
			/* Skip directory where most recent version of file is located */
			if (strcmp(most_recent_directory, directory_array[k].dir_path) == 0) {
				continue;
			}
			printf("Copy file %s from %s to %s\n", filename, most_recent_directory, directory_array[k].dir_path);
			if (CONFIG.dry_run) {
				continue;
			}
			/* Construct full path to file */
			char src_path[1024];
			char dest_path[1024];
			sprintf(src_path, "%s/%s", most_recent_directory, filename);
			sprintf(dest_path, "%s/%s", directory_array[k].dir_path, filename);
			/* Copy file */
			copy_file(src_path, dest_path);
		}
	}
}
