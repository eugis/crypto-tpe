#include "fileLibrary.h"
#include <stdlib.h>

// open file
FILE * openFile(const char * filename) {
	FILE *ptr;
	printf("Opening  file..\n");
	ptr = fopen(filename, "rb");
	if (ptr == NULL) {
		printf("Error opening file\n");
		exit(1);
 	}
 	return ptr;
}

// close file
void closeFile(FILE * ptr) {
	printf("Closing file..\n");
	fclose(ptr);
}

unsigned int getLen(FILE* file) {
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
