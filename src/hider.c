#include "hider.h"

void put_LSB1(BYTE * out, int index, int size_of_each_sample, BYTE data);
void get_LSB1(BYTE * out, int index, BYTE data);
void put_LSB4(BYTE * out, int index, int size_of_each_sample, BYTE data);
void get_LSB4(BYTE * out, int index, BYTE data);

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

void get_from_LSB4(const BYTE * data, const char * filename, int size_of_each_sample) {
	int i, j, k, l;
	BYTE* size_buffer = calloc(4, sizeof(BYTE));
	unsigned int size = 0;
	for (i = 0; i < 4*sizeof(BYTE)*2; i++) {
		get_LSB4(size_buffer, i, data[(i+1)*size_of_each_sample-1]);
	}
	size = size_buffer[0]<<24 
		   | size_buffer[1] << 16 
		   | size_buffer[2] << 8 
		   | size_buffer[3];
	
	printf("%u\n", size);
	BYTE * message = malloc(size);
	for (j = 0; j < size*2; j++) {
		get_LSB4(message, j, data[(j+i+1)*size_of_each_sample-1]);	
	}

	char * b = malloc(sizeof(char) * 20);
	l = 0;
	while (l == 0 || l % 2 != 0 || b[l/2-1] != '\0') { 
		get_LSB4(b, l, data[(1+i+j+l)*size_of_each_sample-1]);
		l++;
	}
	char * full_filename = malloc(strlen(filename) + strlen(b));
	strcat(full_filename, filename);
	strcat(full_filename, b);

	FILE * ptr = fopen(full_filename, "wb");
	fwrite(message, size, 1, ptr);				
}

// TODO: Change this method
void get_LSB4(BYTE * out, int index, BYTE data) {
	int index_in_array = index/(8/4); //4 for lsb4
	int index_in_byte = index%2;
	int shift = 4 - index_in_byte*4;
	out[index_in_array]|=(data&0x0F)<<shift;
}

void get_from_LSBE(const BYTE * data, const char * filename, int size_of_each_sample) {
	int i, l;
	BYTE* size_buffer = calloc(4, sizeof(BYTE));
	unsigned int size = 0;
	int bytes_read = 0;
	i = 0;
	while(bytes_read < 4 * sizeof(BYTE)* 8) {
		if (data[i] == 0xFF || data[i] == 0xFE) {
			get_LSB1(size_buffer, bytes_read, data[i]);
			printf("%d", data[i]-254);
			bytes_read++;
		}
		i++;
	}
	size = size_buffer[0]<<24 | size_buffer[1] << 16 | size_buffer[2] << 8 | size_buffer[3];

	printf("%u\n", size);
	printf("%d\n", bytes_read);
	bytes_read = 0;
	BYTE * message = malloc(size * sizeof(BYTE));
	while(bytes_read < size*8) {
		if (data[i] == 0xFF || data[i] == 0xFE) {
			get_LSB1(message, bytes_read, data[i]);
			bytes_read++;
		}
		i++;
	}

	char * extension_buffer = malloc(sizeof(char) * 20);
	l = 0;
	while (l == 0 || l % 8 != 0 || extension_buffer[l/8-1] != '\0') { 
		if (data[i] == 0xFF || data[i] == 0xFE) {
			get_LSB1(extension_buffer, l, data[i]);
			l++;
		}
		i++;
	}
	char * full_filename = malloc(strlen(filename) + strlen(extension_buffer));
	strcat(full_filename, filename);
	strcat(full_filename, extension_buffer);

	FILE * ptr = fopen(full_filename, "wb");
	fwrite(message, size, 1, ptr);				
}

// index: first index to write in out 
void put_LSB1(BYTE * out, int index, int size_of_each_sample, BYTE data) {
	int i;
	for(i=0; i<8; i++){
		out[index+i*size_of_each_sample]|=(data>>(i))&1;
	}
}
