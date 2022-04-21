/* Orias SPN Block Cipher by Karl Zander */

/* Poly Generated Substitution Box */
int oriasS0[26] = {11, 0, 15, 4, 19, 8, 23, 12, 1, 16, 5, 20, 9, 24, 13, 2, 17, 6, 21, 10, 25, 14, 3, 18, 7, 22};
/* Inverse Poly Generated Substitution Box */
int oriasS0i[26] = {1, 8, 15, 22, 3, 10, 17, 24, 5, 12, 19, 0, 7, 14, 21, 2, 9, 16, 23, 4, 11, 18, 25, 6, 13, 20};
/* Poly Generated Affine Box */
int oriasA0[12] = {21, 9, 3, 7};
/* Inverse Poly Generated Affine Box */
int oriasA0i[12] = {5, 3, 9, 15};
/* Poly Generated Mix Selection Box */
int oriasM0[26] = {16, 18, 20, 22, 24, 14, 13, 15, 17, 19, 21, 23, 25, 3, 5, 7, 9, 11, 1, 0, 2, 4, 6, 8, 10, 12};
/* Round key Vectors */
int oriasPoly26[3] = {4, 23, 11};
/* Number of Rounds */
int oriasRounds = 13;
/* Block Length */
int oriasBlockLen = 26;
/* Half Block Length */
int oriasHalfLen = 13;
/* Affine Box Length */
int oriasALen = 4;
/* Key Length */
int oriasKeyLen = 26;
/* Initalization Vector Length */
int oriasIVLen = 26;
/* Block Rotation Vector */
int oriasROT = 2;
/* Key Scheduler Constants */
int oriasC0[26] = {7, 9, 11, 4, 6, 8, 10, 12, 14, 2, 0, 1, 3, 5, 20, 21, 22, 25, 23, 24, 19, 18, 17, 16, 15, 13};

/* Orias Encryption State */
struct oriasState {
    int rk[13][26];
    int block[26];
    int last[26];
    int next[26];
    int rounds;
    int blocklen;
    int halflen;
    int alen;
    int keylen;
    int ivlen;
    int rot;
};

void oriasInitState(struct oriasState *state) {
    state->rounds = oriasRounds;
    state->blocklen = oriasBlockLen;
    state->halflen = oriasHalfLen;
    state->alen = oriasALen;
    state->keylen = oriasKeyLen;
    state->ivlen = oriasIVLen;
    state->rot = oriasROT;
    for (int r = 0; r < state->rounds; r++) {
        for (int x = 0; x < state->blocklen; x++) {
            state->rk[r][x] = 0;
        }
    }
}

int oriasPolyFuncA(int v, int m) {
    int p = ((v * oriasPoly26[1]) * oriasPoly26[2] + oriasPoly26[1]) % m;
    if (p < 0) {
        p += m;
    }
    return p;
}

int oriasPolyFuncB(int a, int b, int c, int m) {
    int p = (a + b + c) % m;
    if (p < 0) {
        p += m;
    }
    return p;

}

void oriasKeyScheduler(struct oriasState *state, uint8_t *key) {
    int k[26];
    memcpy(k, oriasC0, (26 * sizeof(int)));
    int p;
    int sk[26];
    convertU8toInts(key, k, state->keylen);
    modaddArray(sk, k, state->keylen);
    for (int r = 0; r < state->rounds; r++) {
        for (int x = 0; x < state->blocklen; x++) {
            p = oriasPolyFuncA(k[x], 26);
            state->rk[r][x] = oriasPolyFuncB(state->rk[r][x], p, state->rk[r][modsub(x, 1, 26)], 26);
            k[x] = modadd(k[x], state->rk[r][x], 26);
        }
    }
}

void oriasSubBlock(struct oriasState *state) {
    for (int x = 0; x < state->blocklen; x++) {
        state->block[x] = oriasS0[state->block[x]];
    }
}

void oriasInvSubBlock(struct oriasState *state) {
    for (int x = 0; x < state->blocklen; x++) {
        state->block[x] = oriasS0i[state->block[x]];
    }
}

void oriasAffineMulDiffuse(struct oriasState *state) {
    for (int x = 0; x < state->blocklen; x++) {
        state->block[x] = modmul(state->block[x], oriasA0[mod(x, oriasALen)], 26);
    }
}

void oriasAffineInvMulDiffuse(struct oriasState *state) {
    for (int x = 0; x < state->blocklen; x++) {
        state->block[x] = modmul(state->block[x], oriasA0i[mod(x, oriasALen)], 26);
    }
}

void oriasModAddMix(struct oriasState *state) {
    for (int x = 0; x < state->blocklen; x++) {
        state->block[x] = modadd(state->block[x], state->block[oriasM0[x]], 26);
    }
}

void oriasModInvMix(struct oriasState *state) {
    for (int x = state->blocklen - 1; x != -1; x--) {
        state->block[x] = modsub(state->block[x], state->block[oriasM0[x]], 26);
    }
}

void oriasAddRoundKeys(struct oriasState *state, int r) {
   for (int x = 0; x < state->blocklen; x++) {
       state->block[x] = modadd(state->block[x], state->rk[r][x], 26);
   }
}

void oriasInvRoundKeys(struct oriasState *state, int r) {
   for (int x = 0; x < state->blocklen; x++) {
       state->block[x] = modsub(state->block[x], state->rk[r][x], 26);
   }
}

void oriasEncryptBlock(struct oriasState *state) {
   for (int r = 0; r < state->rounds; r++) {
       oriasSubBlock(state);
       rotateBlockLeft(state->block, state->blocklen, state->rot);
       oriasAffineMulDiffuse(state);
       oriasModAddMix(state);
       oriasAddRoundKeys(state, r);
   }
}

void oriasDecryptBlock(struct oriasState *state) {
   for (int r = (state->rounds - 1); r != -1; r--) {
       oriasInvRoundKeys(state, r);
       oriasModInvMix(state);
       oriasAffineInvMulDiffuse(state);
       rotateBlockRight(state->block, state->blocklen, state->rot);
       oriasInvSubBlock(state);
   }
}

struct oriasKDFState {
    int h[26];
    int t[26];
    uint8_t hashOutput[26];
    int hashlen;
    int iterations;
};

void oriasKDFInit(struct oriasKDFState *state) {
    state->hashlen = 26;
    state->iterations = (26 * 26) + 1;
    for (int x = 0; x < state->hashlen; x++) {
        state->h[x] = 0;
        state->t[x] = 0;
        state->hashOutput[x] = 0;
    }
}

void oriasKDF(struct oriasKDFState *state, char *src, int srcLen) {
    struct oriasState cipherState;
    oriasKDFInit(state);
    oriasInitState(&cipherState);
    uint8_t key[26] = {0};
    int i, x;
    int k[26] = {1};
    for (i =0; i < srcLen; i++) {
        k[i] = src[i] - 65;
    }
    for (i = 0; i < srcLen; i++) {
        state->h[i] = modadd(state->h[i], k[i], 26);
        state->t[i] = state->h[x];
    }
    convertIntstoU8(state->h, key, state->hashlen);
    oriasKeyScheduler(&cipherState, key);
    for (i = 0; i < state->iterations; i++) {
        for (x = 0; x < state->hashlen; x++) {
            state->t[x] = state->h[x];
        }
        memcpy(cipherState.block, state->h, (cipherState.blocklen * sizeof(int)));
        oriasEncryptBlock(&cipherState);
        for (x = 0; x < state->hashlen; x++) {
            state->h[x] = modadd(state->h[x], cipherState.block[x], 26);
            state->h[x] = modadd(state->h[x], state->t[x], 26);
        }
    }
    for (int t = 0; t < state->hashlen; t++) {
        state->hashOutput[t] = state->h[t] + 65;
    }
}

void oriasEncryptFileCBC(char *inFilename, char *outFilename, int filesize, char *passphrase, int passphraseLen) {
    FILE *infile, *outfile;
    infile = fopen(inFilename, "r");
    outfile = fopen(outFilename, "w");
    fseek(infile, (filesize - 1), 0);
    uint8_t btmp;
    fread(&btmp, 1, 1, infile);
    if ((btmp > 90) || (btmp < 65)) {
        filesize -= 1;
    }
    fseek(infile, 0, 0);
    struct oriasState state;
    oriasInitState(&state);
    uint8_t blockU8[state.blocklen];
    int blocks = filesize / state.blocklen;
    int blockExtra = filesize % state.blocklen;
    int blocklen = state.blocklen;
    int padding = state.blocklen - blockExtra;
    if (filesize < state.blocklen) {
        blocks += 1;
    }
    if (blockExtra != 0) {
        blocks += 1;
    }
    int x, y, z, pad;
    int iv[state.blocklen];
    uint8_t ivU8[state.blocklen];
    int pass[state.blocklen];
    getRandomAZ(ivU8, state.blocklen);
    convertU8toInts(ivU8, iv, state.blocklen);
    struct oriasKDFState kdfState;
    convertPassphrasetoInts(passphrase, pass, passphraseLen);
    oriasKDF(&kdfState, passphrase, passphraseLen);
    oriasKeyScheduler(&state, kdfState.hashOutput);
    fwrite(ivU8, 1, state.blocklen, outfile);
    memcpy(state.last, iv, (state.blocklen * sizeof(int)));
    for (x = 0; x < blocks; x++) {
        if ((x == (blocks - 1)) && ((blockExtra != 0))) {
            for (y = (state.blocklen - 1); y != (blockExtra - 1); y--) {
                state.block[y] = padding;
            }
            blocklen = blockExtra;
        }
        fread(blockU8, 1, blocklen, infile);
        convertU8toInts(blockU8, state.block, blocklen);
        modaddArray(state.block, state.last, state.blocklen);
        oriasEncryptBlock(&state);
        convertIntstoU8(state.block, blockU8, state.blocklen);
        memcpy(state.last, state.block, (state.blocklen * sizeof(int)));
        fwrite(blockU8, 1, state.blocklen, outfile);
    }
    fclose(infile);
    fclose(outfile);
}

void oriasDecryptFileCBC(char *inFilename, char *outFilename, int filesize, char *passphrase, int passphraseLen) {
    FILE *infile, *outfile;
    infile = fopen(inFilename, "r");
    outfile = fopen(outFilename, "w");
    struct oriasState state;
    oriasInitState(&state);
    uint8_t blockU8[state.blocklen];
    filesize = filesize - state.ivlen;
    int blocks = filesize / state.blocklen;
    int blockExtra = filesize % state.blocklen;
    int blocklen = state.blocklen;
    int x, y, z, pad;
    int pass[passphraseLen];
    convertPassphrasetoInts(passphrase, pass, passphraseLen);
    int iv[state.blocklen];
    uint8_t ivU8[state.blocklen];
    struct oriasKDFState kdfState;
    oriasKDF(&kdfState, passphrase, passphraseLen);
    oriasKeyScheduler(&state, kdfState.hashOutput);
    fread(ivU8, 1, state.ivlen, infile);
    convertU8toInts(ivU8, iv, state.blocklen);
    memcpy(state.last, iv, (state.blocklen * sizeof(int)));
    for (x = 0; x < blocks; x++) {
        fread(blockU8, 1, state.blocklen, infile);
        convertU8toInts(blockU8, state.block, state.blocklen);
        memcpy(state.next, state.block, (state.blocklen * sizeof(int)));
        oriasDecryptBlock(&state);
        modsubArray(state.block, state.last, state.blocklen);
        memcpy(state.last, state.next, (state.blocklen * sizeof(int)));
        convertIntstoU8(state.block, blockU8, state.blocklen);
        if (x == blocks - 1) {
            int l = state.blocklen - 1;
            int pad = state.block[l];
            int padchk = 1;
            z = l - 1;
            int padding = state.blocklen - pad - 1;
            while (z != padding) {
               if (state.block[z] == pad) {
                   padchk += 1;
               }
               z -= 1;
            }
            if (padchk == pad) {
                blocklen = (state.blocklen - pad);
            }
        }
        fwrite(blockU8, 1, blocklen, outfile);
    }
    fclose(infile);
    fclose(outfile);
}

void oriasEncryptFileOFB(char *inFilename, char *outFilename, int filesize, char *passphrase, int passphraseLen) {
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
    struct oriasState state;
    oriasInitState(&state);
    uint8_t blockU8[state.blocklen];
    int block[state.blocklen];
    int blocks = filesize / state.blocklen;
    int blockExtra = filesize % state.blocklen;
    int blocklen = state.blocklen;
    int padding = state.blocklen - blockExtra;
    int x, y, z, pad;
    int iv[state.blocklen];
    uint8_t ivU8[state.blocklen];
    int pass[state.blocklen];
    getRandomAZ(ivU8, state.blocklen);
    convertU8toInts(ivU8, iv, state.blocklen);
    struct oriasKDFState kdfState;
    convertPassphrasetoInts(passphrase, pass, passphraseLen);
    oriasKDF(&kdfState, passphrase, passphraseLen);
    oriasKeyScheduler(&state, kdfState.hashOutput);
    fwrite(ivU8, 1, state.blocklen, outfile);
    memcpy(state.block, iv, (state.blocklen * sizeof(int)));
    for (x = 0; x < blocks; x++) {
        if ((x == (blocks - 1)) && ((blockExtra != 0))) {
            blocklen = blockExtra;
        }
        fread(blockU8, 1, blocklen, infile);
        convertU8toInts(blockU8, block, blocklen);
        oriasEncryptBlock(&state);
        modaddArray(block, state.block, blocklen);
        convertIntstoU8(block, blockU8, blocklen);
        fwrite(blockU8, 1, blocklen, outfile);
    }
    fclose(infile);
    fclose(outfile);
}

void oriasDecryptFileOFB(char *inFilename, char *outFilename, int filesize, char *passphrase, int passphraseLen) {
    FILE *infile, *outfile;
    infile = fopen(inFilename, "r");
    outfile = fopen(outFilename, "w");
    struct oriasState state;
    oriasInitState(&state);
    int block[state.blocklen];
    uint8_t blockU8[state.blocklen];
    filesize = filesize - state.ivlen;
    int blocks = filesize / state.blocklen;
    int blockExtra = filesize % state.blocklen;
    int blocklen = state.blocklen;
    if (filesize < state.blocklen) {
        blocks += 1;
    }
    if (blockExtra != 0) {
        blocks += 1;
    }
    int x, y, z, pad;
    int pass[passphraseLen];
    convertPassphrasetoInts(passphrase, pass, passphraseLen);
    int iv[state.blocklen];
    uint8_t ivU8[state.blocklen];
    struct oriasKDFState kdfState;
    oriasKDF(&kdfState, passphrase, passphraseLen);
    oriasKeyScheduler(&state, kdfState.hashOutput);
    fread(ivU8, 1, state.ivlen, infile);
    convertU8toInts(ivU8, iv, state.blocklen);
    memcpy(state.block, iv, (state.blocklen * sizeof(int)));
    for (x = 0; x < blocks; x++) {
        if ((x == blocks - 1) && (blockExtra != 0)) {
            blocklen = blockExtra;
        }
        fread(blockU8, 1, blocklen, infile);
        convertU8toInts(blockU8, block, blocklen);
        oriasEncryptBlock(&state);
        modsubArray(block, state.block, blocklen);
        convertIntstoU8(block, blockU8, blocklen);
        fwrite(blockU8, 1, blocklen, outfile);
    }
    fclose(infile);
    fclose(outfile);
}
