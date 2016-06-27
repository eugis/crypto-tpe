#include <stdio.h>
#include <stdint.h>

FILE * open_file(const char * filename, const char * mode);
void close_file(FILE * ptr);
uint32_t get_len(FILE* file);
char* get_extension(const char* filename);
