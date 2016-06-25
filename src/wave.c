/**
 * Read and parse a wave file
 *
 **/
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "wave.h"
#include "fileLibrary.h"
#include "hider.h"

#define TRUE 1 
#define FALSE 0

// WAVE header structure

unsigned char buffer4[4];
unsigned char buffer2[2];

FILE *ptr;
char *filename;
struct HEADER header;

int main(int argc, char **argv) {

	// Get parameters is needed. 
	// It should validates parameters from main :)
 int read = 0;
 filename = (char*) malloc(sizeof(char) * 1024);
 if (filename == NULL) {
   printf("Error in malloc\n");
   exit(1);
 }

 FILE * ansPtr = fopen("ans.wav", "wb");
 // get file path
 char cwd[1024];
 if (getcwd(cwd, sizeof(cwd)) != NULL) {
   
	strcpy(filename, cwd);

	// get filename from command line
	if (argc < 2) {
	 printf("No wave file specified\n");
	 return 0;
	}
	
	strcat(filename, "/");
	strcat(filename, argv[1]);
 }

 ptr = openFile(filename);
 read = read_headers(&header, ptr, ansPtr);

 // calculate no.of samples
 long size_of_each_sample = (header.channels * header.bits_per_sample) / 8;
 // read each sample from data chunk if PCM
 printf("%s\n", "no es pcm?");
 if (header.format_type == 1) { // PCM
	long i =0;
	char* data_buffer=malloc(header.data_size); // this trows segmentation
	int  size_is_correct = TRUE;
 	printf("%s\n", "voy a verificar el formato");
	if (size_is_correct) { 				
		read = fread(data_buffer, header.data_size, 1, ptr);
		printf("%s\n", "intento leer");
		if (read == 1) {
			char cwd[1024];
			free(filename);
			filename = (char*) malloc(sizeof(char) * 1024);
 			if (getcwd(cwd, sizeof(cwd)) != NULL) {
				strcpy(filename, cwd);
				strcat(filename, "/../data/a.txt");
			}
			apply_LSB1(data_buffer, ansPtr, filename, size_of_each_sample, header.data_size);
			// get_from_LSB1(data_buffer, "out", size_of_each_sample);
			// fwrite(data_buffer, sizeof(data_buffer), 1, ansPtr);		
			closeFile(ansPtr);		
		} else {
			printf("Error reading file. %d bytes\n", read);
		}
	} 
 } //  if (header.format_type == 1) { 

 // cleanup before quitting
 closeFile(ptr);
 free(filename);
 return 0;
}

int read_headers(struct HEADER * header, FILE * ptr, FILE * ansPtr) {
	int read = 0;
 
	// read header parts

	read = fread(header -> riff, sizeof(header -> riff), 1, ptr);
	fwrite(header -> riff, sizeof(header -> riff), 1, ansPtr);
	read = fread(buffer4, sizeof(buffer4), 1, ptr);
	fwrite(buffer4, sizeof(buffer4), 1, ansPtr);
	
	// convert little endian to big endian 4 byte int
	header -> overall_size  = buffer4[0] | 
							(buffer4[1]<<8) | 
							(buffer4[2]<<16) | 
							(buffer4[3]<<24);

	read = fread(header -> wave, sizeof(header -> wave), 1, ptr);
	fwrite(header -> wave, sizeof(header -> wave), 1, ansPtr);

	read = fread(header -> fmt_chunk_marker, sizeof(header -> fmt_chunk_marker), 1, ptr);
	fwrite(header -> fmt_chunk_marker, sizeof(header -> fmt_chunk_marker), 1, ansPtr);

	read = fread(buffer4, sizeof(buffer4), 1, ptr);
	fwrite(buffer4, sizeof(buffer4), 1, ansPtr);

	// convert little endian to big endian 4 byte integer
	header -> length_of_fmt = buffer4[0] |
								(buffer4[1] << 8) |
								(buffer4[2] << 16) |
								(buffer4[3] << 24);

	read = fread(buffer2, sizeof(buffer2), 1, ptr); 
	fwrite(buffer2, sizeof(buffer2), 1, ansPtr);
	
	header -> format_type = buffer2[0] | (buffer2[1] << 8);
	
	// Getting format type
	char format_name[10] = "";
	// TODO: change this for switch
	if (header -> format_type == 1) {
		strcpy(format_name,"PCM"); 	
	} else if (header -> format_type == 6) {
		strcpy(format_name, "A-law");	
	} else if (header -> format_type == 7) {
		strcpy(format_name, "Mu-law");	
	}

	read = fread(buffer2, sizeof(buffer2), 1, ptr);
	fwrite(buffer2, sizeof(buffer2), 1, ansPtr);

	header -> channels = buffer2[0] | (buffer2[1] << 8);

	read = fread(buffer4, sizeof(buffer4), 1, ptr);
	fwrite(buffer4, sizeof(buffer4), 1, ansPtr);

	header -> sample_rate = buffer4[0] |
							(buffer4[1] << 8) |
							(buffer4[2] << 16) |
							(buffer4[3] << 24);

	read = fread(buffer4, sizeof(buffer4), 1, ptr);
	fwrite(buffer4, sizeof(buffer4), 1, ansPtr);

	header -> byterate  = buffer4[0] |
							(buffer4[1] << 8) |
							(buffer4[2] << 16) |
							(buffer4[3] << 24);

	read = fread(buffer2, sizeof(buffer2), 1, ptr);
	fwrite(buffer2, sizeof(buffer2), 1, ansPtr);

	header -> block_align = buffer2[0] | (buffer2[1] << 8);

	read = fread(buffer2, sizeof(buffer2), 1, ptr);
	fwrite(buffer2, sizeof(buffer2), 1, ansPtr);
	header -> bits_per_sample = buffer2[0] | (buffer2[1] << 8);
	read = fread(header -> data_chunk_header, sizeof(header -> data_chunk_header), 1, ptr);
	fwrite(header -> data_chunk_header, sizeof(header -> data_chunk_header), 1, ansPtr);
	read = fread(buffer4, sizeof(buffer4), 1, ptr);
	fwrite(buffer4, sizeof(buffer4), 1, ansPtr);

	header -> data_size = buffer4[0] |
					(buffer4[1] << 8) |
					(buffer4[2] << 16) | 
					(buffer4[3] << 24 );
	

	return read;
}