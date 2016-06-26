#include "hider.h"

void put_LSB1(BYTE * out, int index, int size_of_each_sample, BYTE data);
void get_LSB1(BYTE * out, int index, BYTE data);
void put_LSB4(BYTE * out, int index, int size_of_each_sample, BYTE data);
void get_LSB4(BYTE * out, int index, BYTE data);

void apply_LSB1(BYTE * data, FILE * file_to_write, const char * hide_filename, int size_of_each_sample, int data_size) {
	int i;
	
	FILE * ptr = openFile(hide_filename);
	int hidden_file_size = getLen(ptr);
	char* extension = get_extension(hide_filename);
	int extension_size = strlen(extension);

	int content_size = 4*sizeof(BYTE) + hidden_file_size + extension_size + 2; // +1 for the '.' and the '\0'
	BYTE * hide_buffer = calloc(content_size, sizeof(BYTE));
	hide_buffer[0] = (hidden_file_size >> 24) & 0xFF;
	hide_buffer[1] = (hidden_file_size >> 16) & 0xFF;
	hide_buffer[2] = (hidden_file_size >> 8) & 0xFF;
	hide_buffer[3] = hidden_file_size & 0xFF;

	print_data("size in buffer", hide_buffer, 4);

	int read = fread(hide_buffer+4, 1, hidden_file_size, ptr);
	if (read == 0) {
		printf("Error: fail to read file %s\n", hide_filename);
		exit(1);
	}

	hide_buffer[hidden_file_size + 4] = '.';
	for (i = 0; i <= extension_size; i++) {
		hide_buffer[hidden_file_size + 5 + i] = extension[i];
	}

	print_data("data a esconder completa:", hide_buffer, content_size);

	if ( content_size > sizeof(BYTE) * data_size/ size_of_each_sample / 8) {
		printf("Error: the message doesn't fit data: %d\n", sizeof(BYTE) * data_size / size_of_each_sample);
		exit(1);
	}
	for (i = 0; i < content_size; i ++) {
		put_LSB1(data, (i*8+1)*size_of_each_sample - 1, size_of_each_sample, hide_buffer[i]);
	}
	i = fwrite(data, 1, data_size, file_to_write);	
	closeFile(ptr);		
	free(hide_buffer);			
}

void apply_LSB4(BYTE * data, FILE * file_to_write, const char * hide_filename, int size_of_each_sample, int data_size) {
	int i;
	
	FILE * ptr = openFile(hide_filename);
	int hidden_file_size = getLen(ptr);
	char* extension = get_extension(hide_filename);
	int extension_size = strlen(extension);

	int content_size = 4*sizeof(BYTE) + hidden_file_size + extension_size + 2; // +1 for the '.' and the '\0'
	BYTE * hide_buffer = calloc(content_size, sizeof(BYTE));
	hide_buffer[0] = (hidden_file_size >> 24) & 0xFF;
	hide_buffer[1] = (hidden_file_size >> 16) & 0xFF;
	hide_buffer[2] = (hidden_file_size >> 8) & 0xFF;
	hide_buffer[3] = hidden_file_size & 0xFF;

	print_data("size in buffer", hide_buffer, 4);

	int read = fread(hide_buffer+4, 1, hidden_file_size, ptr);
	if (read == 0) {
		printf("Error: fail to read file %s\n", hide_filename);
		exit(1);
	}

	hide_buffer[hidden_file_size + 4] = '.';
	for (i = 0; i <= extension_size; i++) {
		hide_buffer[hidden_file_size + 5 + i] = extension[i];
	}

	print_data("data a esconder completa:", hide_buffer, content_size);
	if ( content_size > sizeof(BYTE) * data_size/ size_of_each_sample / 2) {
		printf("Error: the message doesn't fit data: %d\n", sizeof(BYTE) * data_size / size_of_each_sample);
		exit(1);
	}
	for (i = 0; i < content_size; i ++) {
		put_LSB4(data, (i*2+1)*size_of_each_sample - 1, size_of_each_sample, hide_buffer[i]);
	}
	i = fwrite(data, 1, data_size, file_to_write);	
	closeFile(file_to_write);
	free(hide_buffer);			
}

void apply_LSBE(BYTE * data, FILE * file_to_write, const char * hide_filename, int size_of_each_sample, int data_size) {
	int i;
	
	FILE * ptr = openFile(hide_filename);
	int hidden_file_size = getLen(ptr);
	char* extension = get_extension(hide_filename);
	int extension_size = strlen(extension);

	int content_size = 4*sizeof(BYTE) + hidden_file_size + extension_size + 2; // +1 for the '.' and the '\0'
	BYTE * hide_buffer = calloc(content_size, sizeof(BYTE));
	hide_buffer[0] = (hidden_file_size >> 24) & 0xFF;
	hide_buffer[1] = (hidden_file_size >> 16) & 0xFF;
	hide_buffer[2] = (hidden_file_size >> 8) & 0xFF;
	hide_buffer[3] = hidden_file_size & 0xFF;

	print_data("size in buffer", hide_buffer, 4);

	int read = fread(hide_buffer+4, 1, hidden_file_size, ptr);
	if (read == 0) {
		printf("Error: fail to read file %s\n", hide_filename);
		exit(1);
	}

	hide_buffer[hidden_file_size + 4] = '.';
	for (i = 0; i <= extension_size; i++) {
		hide_buffer[hidden_file_size + 5 + i] = extension[i];
	}

	print_data("data a esconder completa:", hide_buffer, content_size);

	i = 0;
	int bits_read = 0;
	int byte_in_hidden_buffer = 0;
	while(bits_read < content_size * 8) {
		if (i == data_size) {
			printf("Error: the message doesn't fit data\n");
			exit(1);	
		}
		if (data[i] == 0xFF || data[i] == 0xFE) {
			data[i] &= 0xFE;
			BYTE byte_to_save = hide_buffer[byte_in_hidden_buffer];
			data[i] |= (byte_to_save >> (7 - bits_read%8))&1;
			bits_read++;
			byte_in_hidden_buffer += (bits_read % 8 == 0);
		}
		i++;
	}
	i = fwrite(data, 1, data_size, file_to_write);	
	closeFile(file_to_write);
	free(hide_buffer);
}

// index: first index to write in out 
void put_LSB1(BYTE * out, int index, int size_of_each_sample, BYTE data) {
		int i;
		for(i=0; i<8; i++){
			BYTE out_byte = out[index+(7-i)*size_of_each_sample];
			BYTE mask = out_byte &0xfe;
			mask |= ((data >> i)&1);
			*(out+index+(7-i)*size_of_each_sample) |=1; 
			*(out+index+(7-i)*size_of_each_sample) &= mask; 
		}
}

// index: first index to write in out 
void put_LSB4(BYTE * out, int index, int size_of_each_sample, BYTE data) {
		int i;
		for(i=0; i<2; i++){
			BYTE out_byte = out[index+(1-i)*size_of_each_sample];
			BYTE mask = out_byte &0xf0;
			mask |= ((data >> i*4)&0x0F);
			*(out+index+(1-i)*size_of_each_sample) |= 0x0F; 
			*(out+index+(1-i)*size_of_each_sample) &= mask; 
		}
}

void get_from_LSB1(const BYTE * data, const char * filename, int size_of_each_sample) {
	int i, j, k, l;
	BYTE* size_buffer = calloc(4, sizeof(BYTE));
	unsigned int size = 0;
	for (i = 0; i < sizeof(unsigned int)*8; i++) {
		get_LSB1(size_buffer, i, data[(i+1)*size_of_each_sample-1]);
	}
	size = size_buffer[0]<<24 | size_buffer[1] << 16 | size_buffer[2] << 8 | size_buffer[3];
	printf("%x\n", size);
	print_data("number size", &size, 4);
	BYTE * message = malloc(size);
	for (j = 0; j < size*8; j++) {
		get_LSB1(message, j, data[(j+i+1)*size_of_each_sample-1]);	
	}

	char * extension = malloc(sizeof(char) * 20);
	l = 0;
	while (l == 0 || l % 8 != 0 || extension[l/8-1] != '\0'){ 
		get_LSB1(extension, l, data[(1+i+j+l)*size_of_each_sample-1]);
		l++;
	}
	char * full_filename = malloc(strlen(filename) + strlen(extension));
	strcat(full_filename, filename);
	strcat(full_filename, extension);
	print_data("message:", message, size);
	printf("mess: %s \n Filename: %s \nExtension: %s\n", message, full_filename, extension);
	FILE * ptr = fopen(full_filename, "wb");
	fwrite(message, size, 1, ptr);	
	closeFile(ptr);
	free(message);			
	free(size_buffer);	
	free(extension);
}

// Change this method
void get_LSB1(BYTE * out, int index, BYTE data) {
	int index_in_array = index/8;
	int index_in_byte = index%8;
	int shift = 7 - index_in_byte;
	out[index_in_array]|=(data&0x01)<<shift;
}

void get_from_LSB4(const BYTE * data, const char * filename, int size_of_each_sample) {
	int i, j, k, l;
	BYTE* size_buffer = calloc(4, sizeof(BYTE));
	unsigned int size = 0;
	for (i = 0; i < 4*sizeof(BYTE)*2; i++) {
		get_LSB4(size_buffer, i, data[(i+1)*size_of_each_sample-1]);
	}
	size = size_buffer[0]<<24 
		   | size_buffer[1] << 16 
		   | size_buffer[2] << 8 
		   | size_buffer[3];
	print_data("number size", &size, 4);
	
	printf("%u\n", size);
	BYTE * message = malloc(size);
	for (j = 0; j < size*2; j++) {
		get_LSB4(message, j, data[(j+i+1)*size_of_each_sample-1]);	
	}

	char * extension = malloc(sizeof(char) * 20);
	l = 0;
	while (l == 0 || l % 2 != 0 || extension[l/2-1] != '\0') { 
		get_LSB4(extension, l, data[(1+i+j+l)*size_of_each_sample-1]);
		l++;
	}
	char * full_filename = malloc(strlen(filename) + strlen(extension));
	strcat(full_filename, filename);
	strcat(full_filename, extension);

	FILE * ptr = fopen(full_filename, "wb");
	fwrite(message, size, 1, ptr);		
	closeFile(ptr);
	free(message);			
	free(size_buffer);
	free(extension);		
}

// TODO: Change this method
void get_LSB4(BYTE * out, int index, BYTE data) {
	int index_in_array = index/(8/4); //4 for lsb4
	int index_in_byte = index%2;
	int shift = 4 - index_in_byte*4;
	out[index_in_array]|=(data&0x0F)<<shift;
}

void get_from_LSBE(const BYTE * data, const char * filename, int size_of_each_sample) {
	int i, l;
	BYTE* size_buffer = calloc(4, sizeof(BYTE));
	unsigned int size = 0;
	int bytes_read = 0;
	i = 0;
	while(bytes_read < 4 * sizeof(BYTE)* 8) {
		if (data[i] == 0xFF || data[i] == 0xFE) {
			get_LSB1(size_buffer, bytes_read, data[i]);
			printf("%d", data[i]-254);
			bytes_read++;
		}
		i++;
	}
	size = size_buffer[0]<<24 | size_buffer[1] << 16 | size_buffer[2] << 8 | size_buffer[3];

	printf("%u\n", size);
	printf("%d\n", bytes_read);
	bytes_read = 0;
	BYTE * message = malloc(size * sizeof(BYTE));
	while(bytes_read < size*8) {
		if (data[i] == 0xFF || data[i] == 0xFE) {
			get_LSB1(message, bytes_read, data[i]);
			bytes_read++;
		}
		i++;
	}

	char * extension_buffer = malloc(sizeof(char) * 20);
	l = 0;
	while (l == 0 || l % 8 != 0 || extension_buffer[l/8-1] != '\0') { 
		if (data[i] == 0xFF || data[i] == 0xFE) {
			get_LSB1(extension_buffer, l, data[i]);
			l++;
		}
		i++;
	}
	char * full_filename = malloc(strlen(filename) + strlen(extension_buffer));
	strcat(full_filename, filename);
	strcat(full_filename, extension_buffer);

	FILE * ptr = fopen(full_filename, "wb");
	fwrite(message, size, 1, ptr);	
	closeFile(ptr);
	free(message);			
	free(size_buffer);
}

void get_from_LSB1_encrypted(const BYTE * data, const char * filename, int size_of_each_sample, char * password, encrypt_mode encrypt_mode, encrypt_method method) {
	int i, j, k, l;
	BYTE* size_buffer = calloc(4, sizeof(BYTE));
	unsigned int size = 0;
	for (i = 0; i < sizeof(unsigned int)*8; i++) {
		get_LSB1(size_buffer, i, data[(i+1)*size_of_each_sample-1]);
	}
	size = size_buffer[0]<<24 | size_buffer[1] << 16 | size_buffer[2] << 8 | size_buffer[3];
	printf("%x\n", size);
	print_data("number size", &size, 4);
	BYTE * message = malloc(size);
	for (j = 0; j < size*8; j++) {
		get_LSB1(message, j, data[(j+i+1)*size_of_each_sample-1]);	
	}
	
	BYTE * decrypted_message = calloc(size, sizeof(BYTE));
	decrypt_with_mode(password, message, size, decrypted_message, encrypt_mode, method);
	printf("%s\n", password);
	printf("%d\n", method);
	printf("%d\n", encrypt_mode);	

	print_data("decrypted_message", decrypted_message, 4);
	// int decrypted_size = (decrypted_message[0]>>24) & 0xFF;
	// decrypted_size = (decrypted_message[1]>>16) & 0xFF;
	// decrypted_size = (decrypted_message[2]>>8) & 0xFF;
	// decrypted_size = (decrypted_message[3]) & 0xFF;
	unsigned int decrypted_size = decrypted_message[0]<<24 
		   | decrypted_message[1] << 16 
		   | decrypted_message[2] << 8 
		   | decrypted_message[3];

	printf("decrypted_size : %d\n", decrypted_size);
	char * extension = malloc(sizeof(char) * 20);
	l = 0;
	while (decrypted_message[decrypted_size + l + 4] != '\0') { 
		extension[l] = decrypted_message[decrypted_size + l + 4];
		l++;
	}
	extension[l] = '\0';

	char * full_filename = malloc(strlen(filename) + strlen(extension));
	strcat(full_filename, filename);
	strcat(full_filename, extension);
	// print_data("message:", message, size);
	// printf("mess: %s \n Filename: %s \nExtension: %s\n", message, full_filename, extension);

	FILE * ptr = fopen(full_filename, "wb");
	fwrite(decrypted_message+4, decrypted_size, 1, ptr);	
	closeFile(ptr);
	free(message);		
	free(full_filename);	
	free(decrypted_message);
	free(size_buffer);	
	free(extension);
}

void get_from_LSB4_encrypted(const BYTE * data, const char * filename, int size_of_each_sample, char * password, encrypt_mode encrypt_mode, encrypt_method method) {
	int i, j, k, l;
	BYTE* size_buffer = calloc(4, sizeof(BYTE));
	unsigned int size = 0;
	for (i = 0; i < 4*sizeof(BYTE)*2; i++) {
		get_LSB4(size_buffer, i, data[(i+1)*size_of_each_sample-1]);
	}
	size = size_buffer[0]<<24 
		   | size_buffer[1] << 16 
		   | size_buffer[2] << 8 
		   | size_buffer[3];
	print_data("number size", &size, 4);
	
	printf("%u\n", size);
	BYTE * message = malloc(size);
	for (j = 0; j < size*2; j++) {
		get_LSB4(message, j, data[(j+i+1)*size_of_each_sample-1]);	
	}
	BYTE * decrypted_message = calloc(size, sizeof(BYTE));
	decrypt_with_mode(password, message, size, decrypted_message, encrypt_mode, method);
	printf("%s\n", password);
	printf("%d\n", method);
	printf("%d\n", encrypt_mode);	

	print_data("decrypted_message", decrypted_message, 4);
	// int decrypted_size = (decrypted_message[0]>>24) & 0xFF;
	// decrypted_size = (decrypted_message[1]>>16) & 0xFF;
	// decrypted_size = (decrypted_message[2]>>8) & 0xFF;
	// decrypted_size = (decrypted_message[3]) & 0xFF;
	unsigned int decrypted_size = decrypted_message[0]<<24 
		   | decrypted_message[1] << 16 
		   | decrypted_message[2] << 8 
		   | decrypted_message[3];

	printf("decrypted_size : %d\n", decrypted_size);
	char * extension = malloc(sizeof(char) * 20);
	l = 0;
	while (decrypted_message[decrypted_size + l + 4] != '\0') { 
		extension[l] = decrypted_message[decrypted_size + l + 4];
		l++;
	}
	extension[l] = '\0';
	char * full_filename = malloc(strlen(filename) + strlen(extension));
	strcat(full_filename, filename);
	strcat(full_filename, extension);

	FILE * ptr = fopen(full_filename, "wb");
	fwrite(message, size, 1, ptr);		
	closeFile(ptr);
	free(message);			
	free(size_buffer);
	free(extension);		
}

void get_from_LSBE_encrypted(const BYTE * data, const char * filename, int size_of_each_sample, char * password, encrypt_mode encrypt_mode, encrypt_method method) {
	int i, l;
	BYTE* size_buffer = calloc(4, sizeof(BYTE));
	unsigned int size = 0;
	int bytes_read = 0;
	i = 0;
	while(bytes_read < 4 * sizeof(BYTE)* 8) {
		if (data[i] == 0xFF || data[i] == 0xFE) {
			get_LSB1(size_buffer, bytes_read, data[i]);
			printf("%d", data[i]-254);
			bytes_read++;
		}
		i++;
	}
	size = size_buffer[0]<<24 | size_buffer[1] << 16 | size_buffer[2] << 8 | size_buffer[3];

	printf("%u\n", size);
	printf("%d\n", bytes_read);
	bytes_read = 0;
	BYTE * message = malloc(size * sizeof(BYTE));
	while(bytes_read < size*8) {
		if (data[i] == 0xFF || data[i] == 0xFE) {
			get_LSB1(message, bytes_read, data[i]);
			bytes_read++;
		}
		i++;
	}

	BYTE * decrypted_message = calloc(size, sizeof(BYTE));
	decrypt_with_mode(password, message, size, decrypted_message, encrypt_mode, method);

	print_data("decrypted_message", decrypted_message, 4);
	// int decrypted_size = (decrypted_message[0]>>24) & 0xFF;
	// decrypted_size = (decrypted_message[1]>>16) & 0xFF;
	// decrypted_size = (decrypted_message[2]>>8) & 0xFF;
	// decrypted_size = (decrypted_message[3]) & 0xFF;
	unsigned int decrypted_size = decrypted_message[0]<<24 
		   | decrypted_message[1] << 16 
		   | decrypted_message[2] << 8 
		   | decrypted_message[3];

	printf("decrypted_size : %d\n", decrypted_size);
	char * extension = malloc(sizeof(char) * 20);
	l = 0;
	while (decrypted_message[decrypted_size + l + 4] != '\0') { 
		extension[l] = decrypted_message[decrypted_size + l + 4];
		l++;
	}
	extension[l] = '\0';
	char * full_filename = malloc(strlen(filename) + strlen(extension));
	strcat(full_filename, filename);
	strcat(full_filename, extension);

	FILE * ptr = fopen(full_filename, "wb");
	fwrite(message, size, 1, ptr);	
	closeFile(ptr);
	free(message);			
	free(size_buffer);
}

void apply_LSB1_encrypted(BYTE * data, FILE * file_to_write, const char * hide_filename, int size_of_each_sample, int data_size, char * password, encrypt_mode encrypt_mode, encrypt_method method) {
	int i;
	FILE * ptr = openFile(hide_filename);
	int hidden_file_size = getLen(ptr);
	char* extension = get_extension(hide_filename);
	int extension_size = strlen(extension);

	int content_size = 4*sizeof(BYTE) + hidden_file_size + extension_size + 2; // +2 for the '.' and the '\0'
	BYTE * hide_buffer = calloc(content_size, sizeof(BYTE));
	hide_buffer[0] = (hidden_file_size >> 24) & 0xFF;
	hide_buffer[1] = (hidden_file_size >> 16) & 0xFF;
	hide_buffer[2] = (hidden_file_size >> 8) & 0xFF;
	hide_buffer[3] = hidden_file_size & 0xFF;

	print_data("size in buffer", hide_buffer, 4);

	int read = fread(hide_buffer+4, 1, hidden_file_size, ptr);
	if (read == 0) {
		printf("Error: fail to read file %s\n", hide_filename);
		exit(1);
	}

	hide_buffer[hidden_file_size + 4] = '.';
	for (i = 0; i <= extension_size; i++) {
		hide_buffer[hidden_file_size + 5 + i] = extension[i];
	}

	print_data("data a esconder completa:", hide_buffer, content_size);

	int encrypted_max_size = content_size + EVP_MAX_BLOCK_LENGTH - 1;
	BYTE* encrypted_message = calloc(4 + encrypted_max_size, sizeof(BYTE));
	int encrypted_size = encrypt_with_mode(password, hide_buffer, content_size, encrypted_message + 4, method, encrypt_mode);
	encrypted_message[0] = (encrypted_size >> 24) & 0xFF;
	encrypted_message[1] = (encrypted_size >> 16) & 0xFF;
	encrypted_message[2] = (encrypted_size >> 8) & 0xFF;
	encrypted_message[3] = encrypted_size & 0xFF;

	if (encrypted_size > sizeof(BYTE) * data_size/ size_of_each_sample / 8) {
		printf("Error: the message doesn't fit data: %d\n", sizeof(BYTE) * data_size / size_of_each_sample);
		exit(1);
	}
	for (i = 0; i < encrypted_size + 4; i ++) {
		put_LSB1(data, (i*8+1)*size_of_each_sample - 1, size_of_each_sample, encrypted_message[i]);
	}
	i = fwrite(data, 1, data_size, file_to_write);	
	closeFile(ptr);		
	free(hide_buffer);	
	free(encrypted_message);		
}

void apply_LSB4_encrypted(BYTE * data, FILE * file_to_write, const char * hide_filename, int size_of_each_sample, int data_size, char * password, encrypt_mode encrypt_mode, encrypt_method method) {
}

void apply_LSBE_encrypted(BYTE * data, FILE * file_to_write, const char * hide_filename, int size_of_each_sample, int data_size, char * password, encrypt_mode encrypt_mode, encrypt_method method) {
}

