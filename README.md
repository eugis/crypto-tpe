# crypto-tpe

## Compilation of stegowav

    To be able to compile stegowav in linux, you should have installed the libraries of openssl:
        * openssl
        * libssl-dev
    
    The you just run 'make' command inside crypto-tpe folder

## Running stegowav

    To run stegowav you just need to be in crypto-tpe folder and run stegowav with any combination of the following parameters:
    
        To hide information:
            -embed 
            -in file.wav : path starting with / to the file to be hidden
            -out file.wav : path starting with / to the output file. It should end with .wav
            -steg<LSB1|LSB4|LSBE> : this is the method used to hide information             
            -p file : path starting with / to the wave file which will hide the information
            
            Optional parameters
            -pass password : always needed if you should to encrypt hide information 
            -a <aes128|aes192|aes256|des> : default is aes128    
            -m <ecb|cfb|ofb|cbc> : default is cbc

        To get information:
            -extract
            -out file : the name of the file it would be generated
            -steg<LSB1|LSB4|LSBE> : this is the method used to hide information             
            -p file : path starting with / to the wave file which will hide the information
            
            Optional parameters
            -pass password : always needed if you should to encrypt hide information 
            -a <aes128|aes192|aes256|des> : default is aes128    
            -m <ecb|cfb|ofb|cbc> : default is cbc
