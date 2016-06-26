#ifndef ENCRYPT_LIB
#define ENCRYPT_LIB 

#include <openssl/evp.h>

typedef enum { 
	ECB, 
	CFB, 
	OFB,
	CBC 
} encrypt_method;

typedef enum { 
	AES128, 
	AES192, 
	AES256,
	DES 
} encrypt_mode;

typedef unsigned char BYTE;
// It should be used to make "generic" functions for encryption and decryption
typedef const EVP_CIPHER*(*encrypt_function)();

int encrypt_aes128(const BYTE *password, const BYTE* data, int len, BYTE* ans, encrypt_method method);
int encrypt_aes192(const BYTE *password, const BYTE* data, int len, BYTE* ans, encrypt_method method);
int encrypt_aes256(const BYTE *password, const BYTE* data, int len, BYTE* ans, encrypt_method method);
int encrypt_des(const BYTE *password, const BYTE* data, int len, BYTE* ans, encrypt_method method);
int encrypt_with_mode(const BYTE *password, const BYTE* data, int len, BYTE* ans, encrypt_mode mode, encrypt_method method);

int decrypt_aes128(const BYTE *password, const BYTE* data, int len, BYTE* ans, encrypt_method method);
int decrypt_aes192(const BYTE *password, const BYTE* data, int len, BYTE* ans, encrypt_method method);
int decrypt_aes256(const BYTE *password, const BYTE* data, int len, BYTE* ans, encrypt_method method);
int decrypt_des(const BYTE *password, const BYTE* data, int len, BYTE* ans, encrypt_method method);
int decrypt_with_mode(const BYTE *password, const BYTE* data, int len, BYTE* ans, encrypt_mode mode, encrypt_method method);

#endif