#include <stdio.h>

FILE * open_file(const char * filename, const char * mode);
void close_file(FILE * ptr);
unsigned int get_len(FILE* file);
char* get_extension(const char* filename);
