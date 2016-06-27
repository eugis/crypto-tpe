#include "encryptLib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/aes.h>

void print_data(const char *tittle, const void* data, uint32_t len);
uint32_t decrypt(const BYTE *password, const BYTE* data, uint32_t len, BYTE* ans, encrypt_function function);
uint32_t encrypt(const BYTE *password, const BYTE* data, uint32_t len, BYTE* ans, encrypt_function function);

uint32_t encrypt_with_mode(const BYTE *password, const BYTE* data, uint32_t len, BYTE* ans, encrypt_mode mode, encrypt_method method) {
	uint32_t outl;
	switch (mode) {
		case AES128:
			outl = encrypt_aes128(password, data, len, ans, method);
			break;
		case AES192:
			outl = encrypt_aes192(password, data, len, ans, method);
			break;
		case AES256:
			outl = encrypt_aes256(password, data, len, ans, method);
			break;
		case DES:
			outl = encrypt_des(password, data, len, ans, method);
			break;
	}
	return outl;
}

uint32_t encrypt_aes128(const BYTE *password, const BYTE* data, uint32_t len, BYTE* ans, encrypt_method method) {
	uint32_t outl;
	switch (method) {
		case ECB:
			outl = encrypt(password, data, len, ans, EVP_aes_128_ecb);
			break;
		case CFB:
			outl = encrypt(password, data, len, ans, EVP_aes_128_cfb8);
			break;
		case OFB:
			outl = encrypt(password, data, len, ans, EVP_aes_128_ofb);
			break;
		case CBC:
			outl = encrypt(password, data, len, ans, EVP_aes_128_cbc);
			break;
	}
	return outl;
}

uint32_t encrypt_aes192(const BYTE *password, const BYTE* data, uint32_t len, BYTE* ans, encrypt_method method) {
	uint32_t outl;
	switch (method) {
		case ECB:
			outl = encrypt(password, data, len, ans, EVP_aes_192_ecb);
			break;
		case CFB:
			outl = encrypt(password, data, len, ans, EVP_aes_192_cfb8);
			break;
		case OFB:
			outl = encrypt(password, data, len, ans, EVP_aes_192_ofb);
			break;
		case CBC:
			outl = encrypt(password, data, len, ans, EVP_aes_192_cbc);
			break;
	}
	return outl;
}

uint32_t encrypt_aes256(const BYTE *password, const BYTE* data, uint32_t len, BYTE* ans, encrypt_method method) {
	uint32_t outl;
	switch (method) {
		case ECB:
			outl = encrypt(password, data, len, ans, EVP_aes_256_ecb);
			break;
		case CFB:
			outl = encrypt(password, data, len, ans, EVP_aes_256_cfb8);
			break;
		case OFB:
			outl = encrypt(password, data, len, ans, EVP_aes_256_ofb);
			break;
		case CBC:
			outl = encrypt(password, data, len, ans, EVP_aes_256_cbc);
			break;
	}
	return outl;
}

uint32_t encrypt_des(const BYTE *password, const BYTE* data, uint32_t len, BYTE* ans, encrypt_method method) {
	uint32_t outl;
	switch (method) {
		case ECB:
			outl = encrypt(password, data, len, ans, EVP_des_ede3_ecb);
			break;
		case CFB:
			outl = encrypt(password, data, len, ans, EVP_des_ede3_cfb8);
			break;
		case OFB:
			outl = encrypt(password, data, len, ans, EVP_des_ede3_ofb);
			break;
		case CBC:
			outl = encrypt(password, data, len, ans,  EVP_des_ede3_cbc);
			break;
	}
	return outl;
}

uint32_t decrypt_with_mode(const BYTE *password, const BYTE* data, uint32_t len, BYTE* ans, encrypt_mode mode, encrypt_method method) {
	uint32_t outl;
	switch (mode) {
		case AES128:
			outl = decrypt_aes128(password, data, len, ans, method);
			break;
		case AES192:
			outl = decrypt_aes192(password, data, len, ans, method);
			break;
		case AES256:
			outl = decrypt_aes256(password, data, len, ans, method);
			break;
		case DES:
			outl = decrypt_des(password, data, len, ans, method);
			break;
	}
	return outl;
}

uint32_t decrypt_aes128(const BYTE *password, const BYTE* data, uint32_t len, BYTE* ans, encrypt_method method) {
    uint32_t outl;
	switch (method) {
		case ECB:
			outl = decrypt(password, data, len, ans, EVP_aes_128_ecb);
			break;
		case CFB:
			outl = decrypt(password, data, len, ans, EVP_aes_128_cfb8);
			break;
		case OFB:
			outl = decrypt(password, data, len, ans, EVP_aes_128_ofb);
			break;
		case CBC:
			outl = decrypt(password, data, len, ans, EVP_aes_128_cbc);
			break;
	}
	return outl;
}

uint32_t decrypt_aes192(const BYTE *password, const BYTE* data, uint32_t len, BYTE* ans, encrypt_method method) {
	uint32_t outl;
	switch (method) {
		case ECB:
			outl = decrypt(password, data, len, ans, EVP_aes_192_ecb);
			break;
		case CFB:
			outl = decrypt(password, data, len, ans, EVP_aes_192_cfb8);
			break;
		case OFB:
			outl = decrypt(password, data, len, ans, EVP_aes_192_ofb);
			break;
		case CBC:
			outl = decrypt(password, data, len, ans, EVP_aes_192_cbc);
			break;
	}
	return outl;
}

uint32_t decrypt_aes256(const BYTE *password, const BYTE* data, uint32_t len, BYTE* ans, encrypt_method method) {
	uint32_t outl;
	switch (method) {
		case ECB:
			outl = decrypt(password, data, len, ans, EVP_aes_256_ecb);
			break;
		case CFB:
			outl = decrypt(password, data, len, ans, EVP_aes_256_cfb8);
			break;
		case OFB:
			outl = decrypt(password, data, len, ans, EVP_aes_256_ofb);
			break;
		case CBC:
			outl = decrypt(password, data, len, ans, EVP_aes_256_cbc);
			break;
	}
	return outl;
}

uint32_t decrypt_des(const BYTE *password, const BYTE* data, uint32_t len, BYTE* ans, encrypt_method method) {
	uint32_t outl;
	switch (method) {
		case ECB:
			outl = decrypt(password, data, len, ans, EVP_des_ede3_ecb);
			break;
		case CFB:
			outl = decrypt(password, data, len, ans, EVP_des_ede3_cfb8);
			break;
		case OFB:
			outl = decrypt(password, data, len, ans, EVP_des_ede3_ofb);
			break;
		case CBC:
			outl = decrypt(password, data, len, ans,  EVP_des_ede3_cbc);
			break;
	}
	return outl;
}

void print_data(const char *tittle, const void* data, uint32_t len) {
	printf("%s : ",tittle);
	const BYTE * p = (const BYTE*)data;
	uint32_t i = 0;
	
	for (; i<len; ++i)
		printf("%02X ", *p++);
	
	printf("\n");
}

uint32_t encrypt(const BYTE *password, const BYTE* data, uint32_t len, BYTE* ans, encrypt_function function) {
	EVP_CIPHER_CTX ctx;
	uint32_t keyl = 0, ivl = 0;
	uint32_t outl = 0, templ = 0;
	char *out = calloc(len + EVP_MAX_BLOCK_LENGTH - 1, sizeof(char));
	keyl = EVP_CIPHER_key_length(function());
	ivl = EVP_CIPHER_iv_length(function());
	BYTE key[keyl];
	BYTE iv[ivl];
	
	/* Getting keys and iv */ 
	// Salt is setting in NULL
	EVP_BytesToKey(function(), EVP_md5(), NULL, password, strlen(password),1, key, iv);

    /* Initialize context */
    EVP_CIPHER_CTX_init(&ctx);
	EVP_EncryptInit_ex(&ctx, function(), NULL, key, iv); 
	EVP_EncryptUpdate(&ctx, out, &outl, data, len); 
	EVP_EncryptFinal_ex(&ctx, out + outl, &templ);
	outl +=templ;
	memcpy(ans, out, outl);
	
	/* Clean context struct */ 
	EVP_CIPHER_CTX_cleanup(&ctx);
	free(out);
	return outl;
}

uint32_t decrypt(const BYTE *password, const BYTE* data, uint32_t len, BYTE* ans, encrypt_function function) {
	EVP_CIPHER_CTX ctx;
	uint32_t keyl = 0, ivl = 0;
	uint32_t outl = 0, templ = 0;
	char *out = calloc(len, sizeof(char));
	keyl = EVP_CIPHER_key_length(function());
	ivl = EVP_CIPHER_iv_length(function());
    BYTE key[keyl];
    BYTE iv[ivl];

	/* Getting keys and iv */ 
	// Salt is setting in NULL
    EVP_BytesToKey(function(), EVP_md5(), NULL, password, strlen(password), 1, key, iv);

	/* Initialize context */
	EVP_CIPHER_CTX_init(&ctx);
	EVP_DecryptInit_ex(&ctx, function(), NULL, key, iv); 
	EVP_DecryptUpdate(&ctx, out, &outl, data, len); 
	EVP_DecryptFinal_ex(&ctx, out + outl, &templ);
	outl +=templ;
	memcpy(ans, out, outl);
	
	/* Clean context struct */ 
	EVP_CIPHER_CTX_cleanup(&ctx);
	free(out);
	return outl;
}
