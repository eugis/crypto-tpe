#include <stdio.h>
#include <stdlib.h>
#include "encryptLib.h"
#include "hider.h"
#include "wave.h"
#include "fileLibrary.h"
#include <string.h>

typedef unsigned char BYTE;

#define UNDEFINED 0

#define EXTRACT 1
#define EMBED 2

#define LSB1 1
#define LSB4 2
#define LSBE 3

void error(char* msg) {
	printf("%s\n", msg);
	exit(1);
}

int main(int argc, char** argv) {
	printf("Bienvenido a stegowav\n");
	printf("Hola mundo les da la bienvenida\n");

	if (argc % 2 == 1) {
		error("Cantidad de argumentos erronea.");
	}

	int function_type = UNDEFINED;
	char host_file[512];
	char out_file[512];
	char in_file[512];
	int steg_method = UNDEFINED;
	encrypt_mode enc_algo = AES128;
	encrypt_method mode = CBC;
	char password[256];

	host_file[0] = '\0';
	out_file[0] = '\0';
	in_file[0] = '\0';
	password[0] = '\0';

	if (strcmp(argv[1], "-extract") == 0) {
		function_type = EXTRACT;
	} else if (strcmp(argv[1], "-embed") == 0) {
		function_type = EMBED;
	}
	if (function_type == UNDEFINED) {
		error("El primer argumento debe ser -extract o -embed.");
	}
	int i;
	for (i = 2; i < argc; i += 2) {
		if (strcmp(argv[i], "-p") == 0) {
			if (host_file[0] != '\0') {
				error("Solo puede haber un archivo portador.");
			}
			strcpy(host_file, argv[i + 1]);
		} else if (strcmp(argv[i], "-out") == 0) {
			if (out_file[0] != '\0') {
				error("Solo puede haber un archivo de salida.");
			}
			strcpy(out_file, argv[i + 1]);
		} else if (strcmp(argv[i], "-in") == 0){
			if (in_file[0] != '\0') {
				error("Solo puede haber un archivo para ocultar.");
			}
			strcpy(in_file, argv[i + 1]);
		} else if (strcmp(argv[i], "-steg") == 0) {
			if (steg_method != UNDEFINED) {
				error("Solo puede haber un metodo de estenografiado.");
			}
			if (strcmp(argv[i + 1], "LSB1") == 0) {
				steg_method = LSB1;
			} else if (strcmp(argv[i + 1], "LSB4") == 0) {
				steg_method = LSB4;
			} else if (strcmp(argv[i + 1], "LSBE") == 0) {
				steg_method = LSBE;
			} else {
				error("El metodo de estenografiado recibido no es soportado. Las opciones son LSB1, LSB4 y LSBE.");
			}
		} else if (strcmp(argv[i], "-a") == 0) {
			if (strcmp(argv[i + 1], "aes128") == 0) {
				enc_algo = AES128;
			} else if (strcmp(argv[i + 1], "aes192") == 0) {
				enc_algo = AES192;
			} else if (strcmp(argv[i + 1], "aes256") == 0) {
				enc_algo = AES256;
			} else if (strcmp(argv[i + 1], "des") == 0) {
				enc_algo = DES;
			} else {
				error("El algoritmo de encripcion recibido no es soportado. Las opciones son aes128, aes192, aes256 y des.");
			}
		} else if (strcmp(argv[i], "-m") == 0) {
			if (strcmp(argv[i + 1], "ecb") == 0) {
				mode = ECB;
			} else if (strcmp(argv[i + 1], "cfb") == 0) {
				mode = CFB;
			} else if (strcmp(argv[i + 1], "ofb") == 0) {
				mode = OFB;
			} else if (strcmp(argv[i + 1], "cbc") == 0) {
				mode = CBC;
			} else {
				error("El modo de encripcion recibido no es soportado. Las opciones son ecb, cfb, ofb y cbc.");
			}
		} else if (strcmp(argv[i], "-pass") == 0) {
			if (password[0] != '\0') {
				error("Solo puede haber una contrasena.");
			}
			strcpy(password, argv[i + 1]);
		} else {
			printf("%s: ", argv[i]);
			error("Argumento desconocido");
		}
	}

	if (host_file[0] == '\0') {
		error("Argumentos invalidos: El archivo portador es obligatorio.");
	}
	if (out_file[0] == '\0') {
		error("Argumentos invalidos: El archivo de salida es obligatorio.");	
	}
	if (in_file[0] == '\0' && function_type == EMBED) {
		error("Argumentos invalidos: El archivo de entrada es obligatorio para ocultar informacion.");	
	}
	if (steg_method == UNDEFINED) {
		error("Argumentos invalidos: El metodo de estenografiado es obligatorio.");	
	}

	FILE * host_ptr;
	FILE * out_ptr = fopen(out_file, "wb");
	struct HEADER file_header;
	int read = 0;

	// get file path
	char full_host_file_path[1024];
	if (getcwd(full_host_file_path, sizeof(full_host_file_path)) != NULL) {
		strcat(full_host_file_path, host_file);
	}

	host_ptr = openFile(full_host_file_path);
	read = read_headers(&file_header, host_ptr, out_ptr);

	// Calculate number of samples
	long size_of_each_sample = (file_header.channels * file_header.bits_per_sample) / 8;

	// Read each sample from data chunk if PCM
	if (file_header.format_type == 1) { // PCM
		char* data_buffer = malloc(file_header.data_size); // this trows segmentation
		read = fread(data_buffer, file_header.data_size, 1, host_ptr);
		if (read == 1) {
			if (function_type == EXTRACT) {
				switch (steg_method) {
					case LSB1:
						if (password[0] == '\0') {
							get_from_LSB1(data_buffer, out_file, size_of_each_sample);	
						} else {
							get_from_LSB1_encrypted(data_buffer, out_file, size_of_each_sample, password, mode, enc_algo);	
						}
						break;
					case LSB4:
						if (password[0] == '\0') {
							get_from_LSB4(data_buffer, out_file, size_of_each_sample);
						} else {
							get_from_LSB4_encrypted(data_buffer, out_file, size_of_each_sample, password, mode, enc_algo);	
						}
						break;
					case LSBE:
						get_from_LSBE(data_buffer, out_file, size_of_each_sample);
						break;
				}
				fwrite(data_buffer, sizeof(data_buffer), 1, out_ptr);	
			} else {
				char full_in_file_path[1024];
				if (getcwd(full_in_file_path, sizeof(full_in_file_path)) != NULL) {
					strcat(full_in_file_path, in_file);
				}
				switch (steg_method) {
					case LSB1:
						apply_LSB1(data_buffer, out_ptr, full_in_file_path, size_of_each_sample, file_header.data_size);
						break;
					case LSB4:
						apply_LSB4(data_buffer, out_ptr, full_in_file_path, size_of_each_sample, file_header.data_size);
						break;
					case LSBE:
						apply_LSBE(data_buffer, out_ptr, full_in_file_path, size_of_each_sample, file_header.data_size);
						break;
				}
			}	
		} else {
			printf("Error al leer el archivo portador. %d bytes leidos\n", read);
		}
	} //  if (header.format_type == 1) { 

	// Cleanup before quitting
	closeFile(host_ptr);
	fclose(out_ptr);

	return 0;
}	
