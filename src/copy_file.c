#include "mysync.h"

void copy_file(char *src_path, char *dest_path) {
    if (CONFIG.dry_run) {
        return;
    }

    struct stat src_stat;
    // if (stat(src_path, &src_stat) != 0) {
    //     perror("Error: could not get source file metadata");
    //     exit(EXIT_FAILURE);
    // }
    time_t src_mtime = src_stat.st_mtime;
    time_t src_atime = src_stat.st_atime;

    // struct stat dest_stat;
    // if (stat(dest_path, &dest_stat) != 0) {
    //     perror("Error: could not get destination file metadata");
    //     exit(EXIT_FAILURE);
    // }
    // time_t dest_mtime = dest_stat.st_mtime;



    FILE *src_file = fopen(src_path, "rb");
    if (src_file == NULL) {
        perror("Error: could not open source file");
        exit(EXIT_FAILURE);
    }

    FILE *dest_file = fopen(dest_path, "wb");
    if (dest_file == NULL) {
        perror("Error: could not open destination file");
        exit(EXIT_FAILURE);
    }

    char buffer[BUFSIZ];
    size_t n;

    while ((n = fread(buffer, 1, sizeof buffer, src_file)) > 0) {
        if (fwrite(buffer, 1, n, dest_file) != n) {
            perror("Error: could not write to destination file");
            exit(EXIT_FAILURE);
        }
    }

    if (ferror(src_file)) {
        perror("Error: could not read from source file");
        exit(EXIT_FAILURE);
    }

    if (fclose(src_file) != 0) {
        perror("Error: could not close source file");
        exit(EXIT_FAILURE);
    }

    if (CONFIG.preserve) {
 
        if (chmod(dest_path, src_stat.st_mode) != 0) {
            perror("Error: could not set destination file permissions");
            exit(EXIT_FAILURE);
        }
        struct timeval times[2];
        times[0].tv_sec = src_atime;
        times[0].tv_usec = 0;
        times[1].tv_sec = src_mtime;
        times[1].tv_usec = 0;
        if (utimes(dest_path, times) != 0) {
            perror("Error: could not set destination file timestamps");
            exit(EXIT_FAILURE);
        }
    }

    if (fclose(dest_file) != 0) {
        perror("Error: could not close destination file");
        exit(EXIT_FAILURE);
    }


    // // PRINT PREVIOUS MODIFIED TIMES
    // printf("Previous modified times:\n");
    // printf("Source: %ld\n", src_mtime);
    // printf("Destination: %ld\n", dest_mtime);

    // // PRINT NEW MODIFIED TIMES
    // stat(src_path, &src_stat);
    // stat(dest_path, &dest_stat);
    // printf("New modified times:\n");
    // printf("Source: %ld\n", src_stat.st_mtime);
    // printf("Destination: %ld\n", dest_stat.st_mtime);

}
