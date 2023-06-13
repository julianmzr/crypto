Dieses Projekt ist im Rahmen einer Projektarbeit im Fach Grundlagenpraktikum: Rechnerarchitektur entstanden. Es wurde auf wesentliche Funktionalität reduziert.  

Dieses Programm erlaubt ver- und entschlüsseln von Dateien mittels XTEA-Blockchiffre, betrieben mit verschiedenen Betriebsmodi, wählbar über -V.  
Version 0 ist hierbei der Counter-Betriebsmodus (CTR) und Version 1 das Cipher Block Chaining (CBC).  
```
Usage: ./main [OPTION] ... [FILE] ...    
```
Performs cryptographic encryption and decryption using XTEA (Extended Tiny Encryption Algorithm).  

With FILE as input file, additional files will be ignored.  

Run ./main --help for options  
