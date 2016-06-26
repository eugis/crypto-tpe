all: src/stegowav.c src/encryptLib.c src/fileLibrary.c src/hider.c src/wave.c
	gcc -o stegowav src/stegowav.c src/encryptLib.c src/fileLibrary.c src/hider.c src/wave.c -lcrypto -w

