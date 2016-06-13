#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "fileLibrary.h"
#include "encryptLib.h"

typedef unsigned char BYTE;

void apply_LSB1(BYTE * data, const BYTE * message, int size_of_each_sample);
void get_from_LSB1(const BYTE * data, const char * filename, int size_of_each_sample);