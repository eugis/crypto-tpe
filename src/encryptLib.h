typedef enum { 
	ECB, 
	CFB, 
	OFB, 
	CBC 
} encrypt_method;

typedef unsigned char BYTE;
// It should be used to make "generic" functions for encryption and decryption
//typedef EVP_CIPHER(* encrypt_method)() encrypt_function; 

#define MAX_ENCR_LENGTH 1024
#define TAM_CLAVE 16

BYTE* encrypt_aes128(const char *password, const BYTE* data, int len, BYTE* ans, encrypt_method method);
int encrypt_aes192(const BYTE *password, const BYTE* data, int len, BYTE* ans, encrypt_method method);
void* encrypt_aes256(const char *password, const void* data, int len, encrypt_method method);
void* encrypt_des(const char *password, const void* data, int len, encrypt_method method);

BYTE* decrypt_aes128(const char *password, const BYTE* data, int len, encrypt_method method);
int decrypt_aes192(const BYTE *password, const BYTE* data, int len, BYTE* ans, encrypt_method method);
void* decrypt_aes256(const char *password, const void* data, int len, encrypt_method method);
void* decrypt_des(const char *password, const void* data, int len, encrypt_method method);