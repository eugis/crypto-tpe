#include "encryptLib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/aes.h>
#include <openssl/evp.h>

// const static BYTE aes_key[]={0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xAA,0xBB,0xCC,0xDD,0xEE,0xFF};

void print_data(const char *tittle, const void* data, int len);

BYTE* encrypt_aes128(const char *password, const BYTE* data, int len, BYTE* ans, encrypt_method method) {
	// OPENSSL_cleanse(0, 0); //This is only to fix openssl bug
	// /* Init vector */
	// // TODO: el iv tiene que estar hecho de alguna forma
	// BYTE iv[AES_BLOCK_SIZE];
	// memset(iv, 0x00, AES_BLOCK_SIZE);
	
	// /* Buffers for Encryption and Decryption */
	// BYTE enc_out[len*sizeof(char)];
	
	//  AES-128 bit CBC Encryption 
	// AES_KEY enc_key;
	// AES_set_encrypt_key(aes_key, sizeof(aes_key)*8, &enc_key);
	// AES_cbc_encrypt(data, ans, len*sizeof(char), &enc_key, iv, AES_ENCRYPT);
	
	// /* Printing and Verifying */
	// print_data("Original ",data, len*sizeof(char)); // you can not print data as a string, because after Encryption its not ASCII
	// printf("%s\n", data);
	// print_data("Encrypted",ans, len*sizeof(BYTE));
	// return enc_out;
	return 0;
}

int encrypt_aes192(const BYTE *password, const BYTE* data, int len, BYTE* ans, encrypt_method method) {
	EVP_CIPHER_CTX ctx;
	unsigned int outl, templ;
	char out[MAX_ENCR_LENGTH];
	BYTE key[TAM_CLAVE];
	BYTE iv[TAM_CLAVE];
	
	/* Getting keys and iv */ 
	// Salt is setting in NULL
	EVP_BytesToKey(EVP_aes_192_cbc(), EVP_md5(), NULL, password, strlen(password),1, key, iv);

    /* Initialize context */
    EVP_CIPHER_CTX_init(&ctx);
	EVP_EncryptInit_ex(&ctx, EVP_aes_192_cbc(), NULL, key, iv); 
	EVP_EncryptUpdate(&ctx, out, &outl, data, len); 
	EVP_EncryptFinal_ex(&ctx, out + outl, &templ);
	outl +=templ;
	memcpy(ans, out, outl);

	/* Testing */
	print_data("Original ", data, len*sizeof(char)); // you can not print data as a string, because after Encryption its not ASCII
	printf("%s\n", data);
	print_data("Encrypted", ans, outl*sizeof(BYTE));
	
	/* Clean context struct */ 
	EVP_CIPHER_CTX_cleanup(&ctx);
	return outl;
}

void* encrypt_aes256(const char *password, const void* data, int len, encrypt_method method) {
	return 0;
}

void* encrypt_des(const char *password, const void* data, int len, encrypt_method method) {
	return 0;
}

BYTE* decrypt_aes128(const char *password, const BYTE* data, int len, encrypt_method method) {
	// BYTE iv[AES_BLOCK_SIZE];
	// memset(iv, 0x00, AES_BLOCK_SIZE);

	// BYTE dec_out[len*sizeof(char)];

	// AES_KEY dec_key;
	// /* AES-128 bit CBC Decryption */
	// memset(iv, 0x00, AES_BLOCK_SIZE); // don't forget to set iv vector again, else you can't decrypt data properly
	// AES_set_decrypt_key(aes_key, sizeof(aes_key)*8, &dec_key); // Size of key is in bits
	// AES_cbc_encrypt(data, dec_out, len*sizeof(char), &dec_key, iv, AES_DECRYPT);
	
	// print_data("Encrypted",data, len*sizeof(char));
	// printf("%s\n", dec_out );
	// print_data("Decrypted",dec_out, sizeof(dec_out));
	// return dec_out;
	return 0;
}

int decrypt_aes192(const BYTE *password, const BYTE* data, int len, BYTE* ans, encrypt_method method) {
	EVP_CIPHER_CTX ctx;
	BYTE out[MAX_ENCR_LENGTH]; 
	int outl, templ;
    BYTE key[TAM_CLAVE];
    BYTE iv[TAM_CLAVE];

	/* Getting keys and iv */ 
	// Salt is setting in NULL
    EVP_BytesToKey(EVP_aes_192_cbc(), EVP_md5(), NULL, password, strlen(password),1, key, iv);

	/* Initialize context */
	EVP_CIPHER_CTX_init(&ctx);
	EVP_DecryptInit_ex(&ctx, EVP_aes_192_cbc(), NULL, key, iv); 
	EVP_DecryptUpdate(&ctx, out, &outl, data, len); 
	EVP_DecryptFinal_ex(&ctx, out + outl, &templ);
	outl +=templ;

	/* Testing */
	memcpy(ans, out, outl);
	print_data("Encrypted", data, len*sizeof(char));
	printf("%s\n", out );
	print_data("Decrypted", out, outl*sizeof(char));
	
	/* Clean context struct */ 
	EVP_CIPHER_CTX_cleanup(&ctx);
	return outl;
}

void* decrypt_aes256(const char *password, const void* data, int len, encrypt_method method) {
	return 0;
}

void* decrypt_des(const char *password, const void* data, int len, encrypt_method method) {
	return 0;
}

void print_data(const char *tittle, const void* data, int len) {
	printf("%s : ",tittle);
	const BYTE * p = (const BYTE*)data;
	int i = 0;
	
	for (; i<len; ++i)
		printf("%02X ", *p++);
	
	printf("\n");
}
