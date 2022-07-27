struct oriasKDFState {
    struct oriasState cipherState;
    int h[26];
    int t[26];
    int hashOutput[26];
    int hashlen;
    int iterations;
};

void oriasKDFInit(struct oriasKDFState *state) {
    state->hashlen = 26;
    for (int x = 0; x < state->hashlen; x++) {
        state->h[x] = 0;
        state->t[x] = 0;
        state->hashOutput[x] = 0;
    }
}

void oriasKDF(unsigned char *h, int hLen, unsigned char *key, int keyLen) {
    struct oriasKDFState kdfState;
    oriasInitState(kdfState.cipherState);
    oriasKeyScheduler(kdfState.cipherState, key, keyLen);
    int i, x, c;
    int k[keyLen];
    int outputBlockLen = 26;
    int outputBlocks = keyLen / 26;
    int outputBlocksExtra = keylen % 26;
    if ((outputBlocksExtra != 0) || (keyLen < 26)) {
        outputblocks += 1;
    }
    convertU8toNums(key, k, keyLen);
    for (i = 0; i < keyLen; i++) {
        kdfState.h[x] = modadd(kdfState.h[x], k[x], 26);
        kdfState.t[x] = kdfState.h[x];
    }
    for (i = 0; i < kdfState.iterations; i++) {
        oriasEncryptBlock(kdfState.cipherState, kdfState.h, kdfState.cipherState->blocklen);
        for (x = 0; x < kdfState.hashlen; x++) {
            kdfState.h[x] = modadd(kdfState.h[x], kdfState.t[x], 26);
            kdfState.t[x] = kdfState.h[x];
        }
    }
    c = 0;
    for (i = 0; i < outputBlocks; i++) {
        if ((i == outputBlocks - 1) && (outputBlocksExtra != )) {
            outputBlockLen = outputBlocksExtra;
        }
        oriasEncryptBlock(kdfState.cipherState, kdfState.h, kdfState.cipherState->blocklen);
        for (x = 0; x < kdfState.hashlen; x++) {
            kdfState.h[x] = modadd(kdfState.h[x], kdfState.t[x], 26);
            kdfState.t[x] = kdfState.h[x];
        }
        convertNumstoU8(kdfState.h, kdfState.hashOutput, kdfState.hashlen);
        for (x = 0; x < outputBlockLen; x++) {
            h[c] = kdfState.hashOutput[x];
            c += 1;
        }
    }
}
