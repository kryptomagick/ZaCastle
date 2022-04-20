#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <stdint.h>
#include <string.h>
#include "common/common.c"
#include "ciphers/orias.c"

/* ZaCastle A-Z Cipher System by Karl Zander */
float version = 0.1;

void ZaCastleUsage(float version) {
    printf("ZaCastle v%f - by Karl Zander (zaamunzaamun@gmail.com)\n\n", version);
    printf("Usage:\nzacastle <algorithm> -e <input file> <output file> <public keyfile> <secret keyfile>\n");
    printf("zacastle <algorithm> -d <input file> <output file> <secret keyfile> <public keyfile>\n");
}

int main(int argc, char *argv[]) {
    char *algorithm = argv[1];
    char *mode = argv[2];
    char *infileName = argv[3];
    char *outfileName = argv[4];
    char *passphrase = argv[5];
    char *encryptMode = "-e";
    char *decryptMode = "-d";
    isFile(infileName);
    long fileLen = getFileSize(infileName);
    int passphraseLen = strlen(passphrase);
    //int passphraseLen = collectPassphrase(passphrase);
    if (strcmp(algorithm, "orias-cbc") == 0) {
        if (strcmp(mode, encryptMode) == 0) {
            printf("enc\n");
            oriasEncryptFileCBC(infileName, outfileName, fileLen, passphrase, passphraseLen);
        }
        else if (strcmp(mode, decryptMode) == 0) {
            printf("dec\n");
            oriasDecryptFileCBC(infileName, outfileName, fileLen, passphrase, passphraseLen);
        }
    }
/*
    else if (strcmp(algorithm, "orias-ofb") == 0) {
        if (strcmp(mode, encryptMode) == 0) {
            oriasEncryptFileOFB(filename, fileLen, passphrase);
        }
        else if (strcmp(mode, encryptMode) == 0) {
            oriasDecryptFileOFB(filename, fileLen, passphrase);
        }
    }
*/
}
