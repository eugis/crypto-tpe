#include "hider.h"

void put_LSB1(BYTE * out, int index, int size_of_each_sample, BYTE data);
void get_LSB1(BYTE * out, int index, BYTE * data);

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
	BYTE a;
	unsigned int size;
	for (i = 0; i < sizeof(unsigned int); i++) {
		get_LSB1(data, i*size_of_each_sample, &a);
		size |= a<<(sizeof(int) - i);
	}

	printf("%ul\n", size);

	BYTE * message = malloc(sizeof(char) * size);
	for (j = i; j < size; j++) {
		for(k = 0; k < 8; k++) {
			get_LSB1(data, k*size_of_each_sample, message[j]);	
		}
	}

	char * b = malloc(sizeof(char) * 20);
	l = 0;
	while (a != '\0') {
		get_LSB1(data, (j+l)*size_of_each_sample, &a);
		b[l] = a;
		l++;
	}
	printf("%s\n", b);
	char * full_filename = malloc(strlen(filename) + strlen(b));
	strcat(full_filename, filename);
	strcat(full_filename, b);

	FILE * ptr = fopen(full_filename, "wb");
	fwrite(message, size, 1, ptr);				
}

// Change this method
void get_LSB1(BYTE * out, int index, BYTE * data) {
	int index_in_array = index/8;
	int index_in_byte = index%8;
	int shift = 7 - index_in_byte;
	out[index_in_array]|=((*data)&0x01)<<shift;
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