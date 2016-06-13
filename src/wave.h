#include <stdio.h>
#include <stdint.h>

// typedef uint8_t		BYTE;
// typedef uint16_t	WORD;
// typedef uint32_t	DWORD;

// WAVE file header format
struct HEADER {
	unsigned char riff[4];						// RIFF string
	unsigned int overall_size	;				// overall size of file in bytes
	unsigned char wave[4];						// WAVE string
	unsigned char fmt_chunk_marker[4];			// fmt string with trailing null char
	unsigned int length_of_fmt;					// length of the format data
	unsigned int format_type;					// format type. 1-PCM, 3- IEEE float, 6 - 8bit A law, 7 - 8bit mu law
	unsigned int channels;						// no.of channels
	unsigned int sample_rate;					// sampling rate (blocks per second)
	unsigned int byterate;						// SampleRate * NumChannels * BitsPerSample/8
	unsigned int block_align;					// NumChannels * BitsPerSample/8
	unsigned int bits_per_sample;				// bits per sample, 8- 8bits, 16- 16 bits etc
	unsigned char data_chunk_header [4];		// DATA string or FLLR string
	unsigned int data_size;						// NumSamples * NumChannels * BitsPerSample/8 - size of the next chunk that will be read
};

// typedef DWORD FOURCC; //Four-character code
// typedef FOURCC CKID; //Four-character-code chunk identifier 
// typedef DWORD CKSIZE; //32-bit unsigned size value

// typedef struct{
//     CKID    chunkID;    // 'RIFF'
//     CKSIZE  chunkSize;  // File Size
//     CKID    format;     // Format: 'WAVE'
// } RIFF_CK;

// typedef struct{
// 	CKID chunkID; //'fmt '
// 	CKSIZE chunkSize; // 16 para PCM.Size of rest of subchunk. /* Common fields */
// 	DWORD wFormatTag; // Format category,i.e.:PCM = 1 (no
// 				 	 //compres.)
// 	DWORD wChannels; // Number of channels:1, mono; 2, stereo DWORD dwSamplesPerSec; // Sampling rate: Mhz
// 	DWORD dwAvgBytesPerSec;
// 	WORD wBlockAlign;
// 	WORD wBitsPerSample; //8, 16, etc.
// 	WORD extraParamSize;// If PCM, doesn't exist BYTE *extraParams;//space for extra params
// } FMT_CK;

// typedef struct{
// 	CKID chunkID; // 'data'
// 	CKSIZE chunkSize; // Bytes of data BYTE *soundData; // Sound data.
// } DATA_CK;

// struct wavStr {
// 	RIFF_CK riff_desc; // MANDATORY
// 	FMT_CK fmt;
// 	//FACT_CK fact;
// 	//CUE_CK cue;
// 	//PLIST_CK plist;// Playlist Chunk OPTIONAL
// 	//LIST_CK list; // Associated data list Chunk OPTIONAL // more optional data...
// 	DATA_CK data; // Wave Data Chunk MANDATORY };
// };

int read_headers(struct HEADER * header, FILE * ptr, FILE * ansPtr);