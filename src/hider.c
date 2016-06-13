#include "hider.h"

void put_LSB1(BYTE * out, int index, int size_of_each_sample, BYTE data);
void get_LSB1(BYTE * out, int index, BYTE data);

void apply_LSB1(BYTE * data, const BYTE * message, int size_of_each_sample) {
	int i;
	int size = sizeof(message);

	if (size > sizeof(data) / size_of_each_sample) {
		printf("Error: the message doesn't fit data\n");
		exit(1);
	}

	for (i = 0; i < size; i += 8*size_of_each_sample) {
		put_LSB1(data, i, size_of_each_sample, message);
	}
}

void get_from_LSB1(const BYTE * data, const char * filename, int size_of_each_sample) {
	int i, j, k, l;
	BYTE* a = calloc(4, sizeof(BYTE));
	unsigned int size = 0;
	for (i = 0; i < sizeof(unsigned int)*8; i++) {
		get_LSB1(a, i, data[(i+1)*size_of_each_sample-1]);
	}
	size = a[0]<<24 | a[1] << 16 | a[2] << 8 | a[3];
	printf("%u\n", size);
	BYTE * message = malloc(size);
	for (j = 0; j < size*8; j++) {
		get_LSB1(message, j, data[(j+i+1)*size_of_each_sample-1]);	
	}

	char * b = malloc(sizeof(char) * 20);
	l = 0;
	while (l == 0 || l % 8 != 0 || b[l/8-1] != '\0') { 
		printf("qqq %s\n", b);
		get_LSB1(b, l, data[(1+i+j+l)*size_of_each_sample-1]);
		l++;
	}
	char * full_filename = malloc(strlen(filename) + strlen(b));
	strcat(full_filename, filename);
	strcat(full_filename, b);

	FILE * ptr = fopen(full_filename, "wb");
	fwrite(message, size, 1, ptr);				
}

// Change this method
void get_LSB1(BYTE * out, int index, BYTE data) {
	int index_in_array = index/8;
	int index_in_byte = index%8;
	int shift = 7 - index_in_byte;
	out[index_in_array]|=(data&0x01)<<shift;
}

// index: first index to write in out 
void put_LSB1(BYTE * out, int index, int size_of_each_sample, BYTE data) {
	int i;
	for(i=0; i<8; i++){
		out[index+i*size_of_each_sample]|=(data>>(i))&1;
	}
}

// // podría guardarse la longitud en chars? y nos ahorramos esta función?
// int get_size(BYTE * data, int size_of_each_sample) {
// 	int size;
// 	int i;
// 	for (i = 0; i < sizeof(int); i++) {
// 		size |= data[i*size_of_each_sample]>>(7)&1; //Esto funciona?
// 	}
// 	return size;
// }