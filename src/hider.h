#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "fileLibrary.h"
#include "encryptLib.h"
#include <stdint.h>

typedef uint8_t BYTE;

void apply_LSB1(BYTE * data, FILE * file_to_write, const char * hide_filename, uint32_t size_of_each_sample, uint32_t data_size);
void apply_LSB4(BYTE * data, FILE * file_to_write, const char * hide_filename, uint32_t size_of_each_sample, uint32_t data_size);
void apply_LSBE(BYTE * data, FILE * file_to_write, const char * hide_filename, uint32_t size_of_each_sample, uint32_t data_size);
void get_from_LSB1(const BYTE * data, const char * filename, uint32_t size_of_each_sample);
void get_from_LSB4(const BYTE * data, const char * filename, uint32_t size_of_each_sample);
void get_from_LSBE(const BYTE * data, const char * filename, uint32_t size_of_each_sample);
void get_from_LSB1_encrypted(const BYTE * data, const char * filename, uint32_t size_of_each_sample, char * password, encrypt_mode encrypt_mode, encrypt_method method);
void get_from_LSB4_encrypted(const BYTE * data, const char * filename, uint32_t size_of_each_sample, char * password, encrypt_mode encrypt_mode, encrypt_method method);
void apply_LSB1_encrypted(BYTE * data, FILE * file_to_write, const char * hide_filename, uint32_t size_of_each_sample, uint32_t data_size, char * password, encrypt_mode encrypt_mode, encrypt_method method);
void apply_LSB4_encrypted(BYTE * data, FILE * file_to_write, const char * hide_filename, uint32_t size_of_each_sample, uint32_t data_size, char * password, encrypt_mode encrypt_mode, encrypt_method method);
void apply_LSBE_encrypted(BYTE * data, FILE * file_to_write, const char * hide_filename, uint32_t size_of_each_sample, uint32_t data_size, char * password, encrypt_mode encrypt_mode, encrypt_method method);
