#include <stdio.h>

FILE * openFile(const char * filename);
void closeFile(FILE * ptr);
unsigned int getLen(FILE* file);
char* get_extension(const char* filename);
