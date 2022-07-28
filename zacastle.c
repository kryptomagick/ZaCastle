#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <stdint.h>
#include <string.h>
#include "common/common.c"
#include "ciphers/hex.c"
#include "ciphers/orias.c"
#include "ciphers/violetb.c"

/* ZaCastle A-Z Cipher System by Karl Zander */
float version = 0.5;

void ZaCastleUsage(float version) {
    printf("ZaCastle v%.1f - by Karl Zander\n\n", version);
    printf("Algorithms:\norias-cbc\norias-ofb\nvioletb\n\n");
    printf("Usage:\nzacastle <algorithm> <-e/-d> <input file> <output file> <passphrase>\n");
}

int main(int argc, char *argv[]) {
    if (argc == 1) {
        ZaCastleUsage(version);
        exit(0);
    }
    char *algorithm = argv[1];
    char *mode = argv[2];
    char *infileName = argv[3];
    char *outfileName = argv[4];
    char *passphrase = argv[5];
    char *encryptMode = "-e";
    char *decryptMode = "-d";
    isFile(infileName);
    int fileLen = getFileSize(infileName);
    int passphraseLen = strlen(passphrase);
    if (strcmp(algorithm, "orias-cbc") == 0) {
        if (strcmp(mode, encryptMode) == 0) {
            oriasEncryptFileCBC(infileName, outfileName, fileLen, passphrase, passphraseLen);
        }
        else if (strcmp(mode, decryptMode) == 0) {
            oriasDecryptFileCBC(infileName, outfileName, fileLen, passphrase, passphraseLen);
        }
    }
    else if (strcmp(algorithm, "orias-ofb") == 0) {
        if (strcmp(mode, encryptMode) == 0) {
            oriasEncryptFileOFB(infileName, outfileName, fileLen, passphrase, passphraseLen);
        }
        else if (strcmp(mode, decryptMode) == 0) {
            oriasDecryptFileOFB(infileName, outfileName, fileLen, passphrase, passphraseLen);
        }
    }
    else if (strcmp(algorithm, "violetb") == 0) {
        if (strcmp(mode, encryptMode) == 0) {
            violetbEncryptFile(infileName, outfileName, fileLen, passphrase, passphraseLen);
        }
        else if (strcmp(mode, decryptMode) == 0) {
            violetbDecryptFile(infileName, outfileName, fileLen, passphrase, passphraseLen);
        }
    }
}
