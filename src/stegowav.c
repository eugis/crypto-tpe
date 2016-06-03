#include <stdio.h>
#include <stdlib.h>
#include "encryptLib.h"
#include <string.h>

int main() {
	printf("Bienvenido a stegowav\n");
	printf("Hola mundo les da la bienvenida\n");
	char data[] = "hola cómo estás"; 
	size_t len = strlen(data);
	BYTE* ans = malloc(MAX_ENCR_LENGTH*sizeof(char));
	BYTE* ans_d = malloc(MAX_ENCR_LENGTH*sizeof(char));
	int i = encrypt_aes192("12345678", data, len, ans, CBC);
	//size_t len2 = strlen(enc_data);
	print_data("Encrypted", ans, i);
	int j = decrypt_aes192("12345678", ans, i, ans_d, CBC);
	print_data("Decrypted", ans_d, j);

	free(ans);
	return 0;
}	