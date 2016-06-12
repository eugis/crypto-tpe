/**
 * Read and parse a wave file
 *
 **/
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "wave.h"
#include "fileLibrary.h"

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
 long num_samples = (8 * header.data_size) / (header.channels * header.bits_per_sample);
 long size_of_each_sample = (header.channels * header.bits_per_sample) / 8;

 // read each sample from data chunk if PCM
 if (header.format_type == 1) { // PCM
    printf("Dump sample data? Y/N?");
	char c = 'n';
	scanf("%c", &c); 
	if (c == 'Y' || c == 'y') { 
		long i =0;
		char data_buffer[size_of_each_sample];
		int  size_is_correct = TRUE;

		// make sure that the bytes-per-sample is completely divisible by num.of channels
		long bytes_in_each_channel = (size_of_each_sample / header.channels);
		if ((bytes_in_each_channel  * header.channels) != size_of_each_sample) {
			printf("Error: %ld x %ud <> %ld\n", bytes_in_each_channel, header.channels, size_of_each_sample);
			size_is_correct = FALSE;
		}
 
		if (size_is_correct) { 
			// the valid amplitude range for values based on the bits per sample
			long low_limit = 0l;
			long high_limit = 0l;

			switch (header.bits_per_sample) {
				case 8:
					low_limit = -128;
					high_limit = 127;
					break;
				case 16:
					low_limit = -32768;
					high_limit = 32767;
					break;
				case 32:
					low_limit = -2147483648;
					high_limit = 2147483647;
					break;
			}					

			for (i =1; i <= num_samples; i++) {
				read = fread(data_buffer, sizeof(data_buffer), 1, ptr);

				if (read == 1) {
					fwrite(data_buffer, sizeof(data_buffer), 1, ansPtr);				
					// dump the data read
					unsigned int  xchannels = 0;
					int data_in_channel = 0;

					for (xchannels = 0; xchannels < header.channels; xchannels ++ ) {
						// convert data from little endian to big endian based on bytes in each channel sample
						if (bytes_in_each_channel == 4) {
							data_in_channel =	data_buffer[0] | 
												(data_buffer[1]<<8) | 
												(data_buffer[2]<<16) | 
												(data_buffer[3]<<24);
						}
						else if (bytes_in_each_channel == 2) {
							data_in_channel = data_buffer[0] |
												(data_buffer[1] << 8);
						}
						else if (bytes_in_each_channel == 1) {
							data_in_channel = data_buffer[0];
						}

						// check if value was in range
						if (data_in_channel < low_limit || data_in_channel > high_limit) {
							printf("**value out of range\n");
						}
					}
				}
				else {
					printf("Error reading file. %d bytes\n", read);
					break;
				}

			} // 	for (i =1; i <= num_samples; i++) {

		} // 	if (size_is_correct) { 

	} // if (c == 'Y' || c == 'y') { 
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