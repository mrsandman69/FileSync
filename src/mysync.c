/*
 * CITS2002 Project 2 2023
 * Student 1:	23315115	Kathryn Burton
 * Student 2:	22979781	Gareth Mallinson
 */

#include "mysync.h"

int main(int argc, char *argv[]) {

	parse_args(argc, argv);						/* Parse command line arguments */
	scan_directories(&argv[optind], argc - optind);		/* List regular files in directories */
	return EXIT_SUCCESS;
}


