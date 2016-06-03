# crypto-tpe

To compile file for mac run the following command inside the folder of src:

  gcc -o stegowav stegowav.c encryptLib.c -L/usr/local/openssl-1.0.2h/lib -I/usr/local/openssl-1.0.2h/include -lssl -lcrypto
