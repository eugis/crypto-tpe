#include "hider.h"

void lsb1(BYTE * data, const BYTE * message, int size_of_each_sample) {
	int i;
	int size = sizeof(message);

	if (size > sizeof(data) / size_of_each_sample) {
		printf("Error: the message doesn't fit data\n");
		exit(1);
	}

	for (i = 0; i < size; i += 8*size_of_each_sample) {
		putLSB1(data, i, size_of_each_sample, message);
	}
}

// void getLSB1(BYTE * out, int index, BYTE data) {
// 	int index_in_array = index/8;
// 	int index_in_byte = index%8;
// 	int shift = 7 - index_in_byte;
// 	out[index_in_array]|(data&0x01)<<shift;
// }

// index: first index to write in out 
void putLSB1(BYTE * out, int index, int size_of_each_sample, BYTE data) {
	int i;
	for(i=0; i<8; i++){
		out[index+i*size_of_each_sample]|=(data>>(i))&1;
	}
}