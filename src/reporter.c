#include "mysync.h"

/*
 * Prints usage instructions.
 */
void usage() {
	printf("Usage: ./mysync [options] directory1 directory2 ...\n");
	printf("Options:\n");
	printf("  -a\t\t\t\tSync all files, including hidden files\n");
	printf("  -i <include_pattern>\t\tOnly sync files matching the pattern\n");
	printf("  -n\t\t\t\tDry run, print files that would be copied\n");
	printf("  -o <only_pattern>\t\tOnly sync files matching the pattern\n");
	printf("  -p\t\t\t\tPreserve file metadata\n");
	printf("  -r\t\t\t\tSync directories recursively\n");
	printf("  -v\t\t\t\tVerbose output\n");
}

/*
 * Prints to stdout if verbose flag is set.
 *
 * @param format The format string to be printed.
 * @param ... The arguments to be printed.
 */
void verbose(char *format, ...) {
	if (!CONFIG.verbose) {
		return;
	}

	/* Format the message*/
	char formatted_message[MAXPATHLEN];
	va_list args;
	va_start(args, format);
	vsnprintf(formatted_message, sizeof(formatted_message), format, args);
	va_end(args);

	/* Print the formatted message to stdout */
	printf("%s\n", formatted_message);
}

	

