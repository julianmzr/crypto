Dieses Projekt ist im Rahmen einer Projektarbeit im Fach Grundlagenpraktikum: Rechnerarchitektur entstanden. Es wurde auf wesentliche Funktionalität reduziert.  

Dieses Programm erlaubt ver- und entschlüsseln von Dateien mittels XTEA-Blockchiffre, betrieben mit verschiedenen Betriebsmodi, wählbar über -V.  
Version 0 ist hierbei der Counter-Betriebsmodus (CTR) und Version 1 das Cipher Block Chaining (CBC).  
```
Usage: ./main [OPTION] ... [FILE] ...    
```
Performs cryptographic encryption and decryption using XTEA (Extended Tiny Encryption Algorithm).  

With FILE as input file, additional files will be ignored.  

Options:  
-V NUM implementation version that should be used; default: main implementation: -V 0  
-B (NUM) if set, outputs run time of implementation with NUM repetitions  
NUM is optional, (default: 10_000)  
-k NUM selects NUM as key, should be 128 bit  
-i NUM selects NUM as initialization vector, should be 64 bit  
-d if set, decrypt, otherwise encrypt  
-o FILE set output file as FILE  
-h, --help display this help text and exit  
EXAMPLES  
./main -V 0 -B 100000 -o output input  
./main -V 1 -i 1234567890ABCDEF input  
./main -V 1 -B -k 1234567890ABCDEF0123456789ABCDEF input  
./main -V 0 -B 10000 -k 1234567890ABCDEF0123456789ABCDEF -i 1234567890ABCDEF -d -o output input  
./main  
