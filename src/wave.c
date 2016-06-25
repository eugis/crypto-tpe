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