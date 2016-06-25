#include <stdio.h>
#include <stdlib.h>
#include "encryptLib.h"
// #include "hider.h"
#include "fileLibrary.h"
#include <string.h>

typedef unsigned char BYTE;

void put_LSB1(BYTE * out, int index, int size_of_each_sample, BYTE data) {
		int i;
		for(i=0; i<8; i++){
			// print_data("antes en put_LSB1:", out, 16);
			BYTE out_byte = out[index+(7-i)*size_of_each_sample];
			// print_data("out_byte: ", &out_byte, 1);
			BYTE mask = out_byte &0xfe;
			// print_data("mask: ", &mask, 1);
			mask |= ((data >> (i)&1));
			// print_data("modifyed mask: ", &mask, 1);
			// print_data("should modify byte: ", out+index+(7-i)*size_of_each_sample, 1);
			// printf("%x\n", *(out+index+(7-i)*size_of_each_sample) | mask);
			*(out+index+(7-i)*size_of_each_sample) &= mask; 
			// print_data("out after lsb1: ", out, 16);
			// printf("\n");
		}
}

int main() {
	printf("Bienvenido a stegowav\n");
	printf("Hola mundo les da la bienvenida\n");
	// char data[] = "hola cómo estás"; 
	// size_t len = strlen(data);
	// BYTE* ans = malloc(len*sizeof(char));
	// BYTE* ans_d = malloc(len*sizeof(char));
	// int i = encrypt_des("12345678", data, len, ans, CBC);
	// int j = decrypt_des("12345678", ans, i, ans_d, CBC);

	// free(ans);
	// free(ans_d);
	BYTE * out = malloc(16);
	strcpy(out, "hola como estas");
	print_data("antes:", out, 16);
	put_LSB1(out, 0, 1, 'c');
	print_data("despues:", out, 16);
	printf("%x\n", 'c');
	free(out);
	return 0;
}	