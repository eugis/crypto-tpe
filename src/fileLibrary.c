#include "fileLibrary.h"
#include <stdlib.h>

// open file
FILE * openFile(char * filename) {
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
