/* Violet B Stream Cipher by Karl Zander */

int violetbC0[26] = {9, 12, 4, 7, 1, 22, 14, 25, 8, 10, 0, 6, 21, 24, 11, 18, 5, 16, 5, 13, 23, 20, 2, 19, 17, 3, 15};

int violetbO0[13] = {2, 5, 8, 11, 10, 1, 0, 3, 6, 9, 12, 4, 7};
int violetbO1[13] = {5, 9, 7, 3, 11, 1, 12, 6, 4, 2, 10, 0, 8};
int violetbO2[13] = {12, 4, 6, 7, 2, 3, 8, 9, 11, 5, 1, 10, 0};
int violetbQ0[4] = {1, 3, 0, 2};
int violetbQ1[4] = {0, 2, 1, 3};

int violetbR0[4] = {1, 3, 6, 0};

struct violetbState {
    int CELLS[4][13];
    int rounds;
    int blocklen;
    int keylen;
    int ivlen;
};

void violetbInitState(struct violetbState *state) {
    state->rounds = 20;
    state->blocklen = 26;
    state->keylen = 52;
    state->ivlen = 52;
}

void violetbKeySetup(struct violetbState * state, int * key, int keylen, int *nonce, int noncelen) {
    int i, a;
    int c = 0;
    for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 13; y++) {
            state->CELLS[x][y] = 0;
        }
    }
    for (i = 0; i < 52; i++) {
        a = i % 13;
        state->CELLS[c][a] = modadd(state->CELLS[c][a], violetbC0[mod(i, 26)], 26);
        if (a == 0) {
            c = modadd(c, 1, 4);
        }
    }
    c = 0;
    for (i = 0; i < keylen; i++) {
        a = i % 13;
        state->CELLS[c][a] = modadd(state->CELLS[c][a], key[i], 26);
        if (a == 0) {
            c = modadd(c, 1, 4);
        }
    }
    c = 0;
    for (i = 0; i < noncelen; i++) {
        a = i % 13;
        state->CELLS[c][a] = modadd(state->CELLS[c][a], nonce[i], 26);
        if (a == 0) {
            c = modadd(c, 1, 4);
        }
    }
}

int violetbFuncA(int a, int b, int c) {
    int x = (((a * 11) + b) + c) % 26;
    if (x < 26) {
        x += 26;
    }
    return x;
}

void violetbCrush(struct violetbState *state) {
    int x, y;
    for (x = 0; x < 4; x++) {
        for (y = 0; y < 13; y++) {
            state->CELLS[x][y] = violetbFuncA(state->CELLS[x][violetbO0[y]], state->CELLS[violetbQ0[x]][violetbO1[y]], state->CELLS[violetbQ1[x]][violetbO2[y]]);
        }
        rotateBlockLeft(state->CELLS[x], 13, violetbR0[x]);
    }
    rotateBlockLeft(state->CELLS, 4, 1);
    for (x = 0; x < 4; x++) {
        for (y = 0; y < 13; y++) {
            state->CELLS[x][y] = modadd(state->CELLS[x][y], state->CELLS[modadd(x, 1, 4)][y], 26);
        }
    }
}

void violetbUpdate(struct violetbState *state) {
    for (int r = 0; r < state->rounds; r++) {
        violetbCrush(state);
    }
}

void violetbModAddBlock(struct violetbState *state, int *block, int blocklen) {
    int x = 0;
    int y = 0;
    for (int c = 0; c < blocklen; c++) {
        block[c] = modadd(block[c], state->CELLS[x][y], 26);
        if ((y % 13) == 0) {
            x = modadd(x, 1, 4);
        }
        y = modadd(y, 1, 13);
    }
}

void violetbModSubBlock(struct violetbState *state, int *block, int blocklen) {
    int x = 0;
    int y = 0;
    for (int c = 0; c < blocklen; c++) {
        block[c] = modsub(block[c], state->CELLS[x][y], 26);
        if ((y % 13) == 0) {
            x = modadd(x, 1, 4);
        }
        y = modadd(y, 1, 13);
    }
}

void violetbEncryptFile(char *inFilename, char *outFilename, int filesize, char *passphrase, int passphraseLen) {
    FILE *infile, *outfile;
    infile = fopen(inFilename, "r");
    outfile = fopen(outFilename, "w");
    fseek(infile, filesize - 1, 0);
    int btype;
    fread(&btype, 1, 1, infile);
    if ((btype > 90) || (btype < 65)) {
        filesize -= 1;
    }
    fseek(infile, 0, 0);
    struct violetbState state;
    violetbInitState(&state);
    uint8_t blockU8[state.blocklen];
    int block[state.blocklen];
    int blocks = filesize / state.blocklen;
    int blockExtra = filesize % state.blocklen;
    int blocklen = state.blocklen;
    if ((filesize < state.blocklen) || (blockExtra != 0)) {
        blocks += 1;
    }
    int x, y, z;
    int iv[state.ivlen];
    int k[state.keylen];
    uint8_t ivU8[state.ivlen];
    uint8_t keyU8[state.keylen];
    uint8_t mackeyU8[state.keylen];
    int pass[state.blocklen];
    getRandomAZ(ivU8, state.ivlen);
    convertU8toInts(ivU8, iv, state.ivlen);
    convertPassphrasetoInts(passphrase, pass, passphraseLen);
    hexKDF(keyU8, state.keylen, passphrase, passphraseLen);
    hexKDF(mackeyU8, state.keylen, keyU8, state.keylen);
    convertU8toInts(keyU8, k, state.keylen);
    violetbKeySetup(&state, k, state.keylen, iv, state.ivlen);
    fwrite(ivU8, 1, state.ivlen, outfile);
    struct hexState macState;
    hexHMACIncrementalInit(&macState, mackeyU8, state.keylen);
    uint8_t tag[HEX_TAGLEN];
    for (x = 0; x < blocks; x++) {
        if ((x == (blocks - 1)) && ((blockExtra != 0))) {
            blocklen = blockExtra;
        }
        fread(blockU8, 1, blocklen, infile);
        convertU8toInts(blockU8, block, blocklen);
        violetbUpdate(&state);
        violetbModAddBlock(&state, block, blocklen);
        int macblock[26] = {0};
        memcpy(macblock, block, blocklen * sizeof(int));
        hexHMACIncrementalUpdate(&macState, macblock);
        convertIntstoU8(block, blockU8, blocklen);
        fwrite(blockU8, 1, blocklen, outfile);
    }
    hexHMACOutput(&macState, tag, HEX_TAGLEN);
    fwrite(tag, 1, HEX_TAGLEN, outfile);
    fclose(infile);
    fclose(outfile);
}

void violetbDecryptFile(char *inFilename, char *outFilename, int filesize, char *passphrase, int passphraseLen) {
    FILE *infile, *outfile;
    infile = fopen(inFilename, "r");
    outfile = fopen(outFilename, "w");
    struct violetbState state;
    violetbInitState(&state);
    int block[state.blocklen];
    uint8_t blockU8[state.blocklen];
    filesize = filesize - state.ivlen - HEX_TAGLEN;
    int blocks = filesize / state.blocklen;
    int blockExtra = filesize % state.blocklen;
    int blocklen = state.blocklen;
    if ((filesize < state.blocklen) || (blockExtra != 0)) {
        blocks += 1;
    }
    int x, y, z, pad;
    int pass[passphraseLen];
    convertPassphrasetoInts(passphrase, pass, passphraseLen);
    int iv[state.ivlen];
    int k[state.keylen];
    uint8_t ivU8[state.ivlen];
    uint8_t keyU8[state.keylen];
    uint8_t mackeyU8[state.keylen];
    hexKDF(keyU8, state.keylen, passphrase, passphraseLen);
    hexKDF(mackeyU8, state.keylen, keyU8, state.keylen);
    convertU8toInts(keyU8, k, state.keylen);
    fread(ivU8, 1, state.ivlen, infile);
    convertU8toInts(ivU8, iv, state.ivlen);
    violetbKeySetup(&state, k, state.keylen, iv, state.ivlen);
    struct hexState macState;
    hexHMACIncrementalInit(&macState, mackeyU8, state.keylen);
    uint8_t tag[HEX_TAGLEN];
    for (x = 0; x < blocks; x++) {
        if ((x == blocks - 1) && (blockExtra != 0)) {
            blocklen = blockExtra;
        }
        fread(blockU8, 1, blocklen, infile);
        convertU8toInts(blockU8, block, blocklen);
        int macblock[26] = {0};
        memcpy(macblock, block, blocklen * sizeof(int));
        hexHMACIncrementalUpdate(&macState, macblock);
        violetbUpdate(&state);
        violetbModSubBlock(&state, block, blocklen);
        convertIntstoU8(block, blockU8, blocklen);
        fwrite(blockU8, 1, blocklen, outfile);
    }
    hexHMACOutput(&macState, tag, HEX_TAGLEN);
    uint8_t tag2[HEX_TAGLEN];
    fread(tag2, 1, HEX_TAGLEN, infile);
    fclose(infile);
    fclose(outfile);
    if (hexHMACVerifyAlt(tag, tag2, HEX_TAGLEN) == 1) {
        printf("Error: Message has been tampered with\n");
        exit(1);
    }
}
