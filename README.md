# ZaCastle

A-Z Ciphering system

A ciphering system operating on plaintexts with A-Z characters only (modulo 26) and implementing modern techniques (Block Cipher/Stream Cipher/KDF/HMAC).  The aim of the project is to offer ciphers of 52 factorial in strength or 172 bits of strength.

# Usage

Algorithms

orias-cbc (strength 52)
orias-ofb (strength 52)
violetb (strength 52)

zacastle <algorithm> <-e/-d> <inFile> <outFile> <passphrase>
