#include "fileLibrary.h"
#include <stdlib.h>

// open file
FILE * open_file(const char * filename, const char * mode) {
	FILE *ptr;
	ptr = fopen(filename, mode);
	if (ptr == NULL) {
		printf("Error opening file\n");
		exit(1);
 	}
 	return ptr;
}

// close file
void close_file(FILE * ptr) {
	fclose(ptr);
}

unsigned int get_len(FILE* file) {
	unsigned int len;
	fseek(file, 0L, SEEK_END);
	len = ftell(file);
	rewind(file);
	return len;
}

char* get_extension(const char* filename) {
	const char *dot = strrchr(filename, '.');
    if (!dot || dot == filename) return "";
    return dot + 1;
}
