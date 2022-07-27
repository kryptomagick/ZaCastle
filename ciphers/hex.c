int hexC0[26] = {11, 19, 2, 0, 3, 22, 14, 7, 6, 9, 1, 13, 17, 23, 25, 18, 16, 12, 5, 4, 10, 8, 15, 20, 21, 24};

struct hexState {
    int hashlen;
    int blocklen;
    int inputlen;
    int rounds;
    int iterations;
    int T[4][13];
    int H[4][13];
    int block[52];
};

void hexInit(struct hexState *state) {
    state->blocklen = 52;
    state->inputlen = 26;
    state->rounds = 20;
    state->iterations = (26 * 26) + 1;
    memset(state->H, 0, 4 * 13 * (sizeof(int)));
    memcpy(state->H, hexC0, 26 * (sizeof(int)));
}

void hexMix0(struct hexState *state) {
    state->H[0][0] = modadd(state->H[0][0], state->H[1][1], 26);
    state->H[0][1] = modadd(state->H[0][1], state->H[2][2], 26);
    state->H[0][2] = modadd(state->H[0][2], state->H[3][3], 26);
    state->H[0][3] = modadd(state->H[0][3], state->H[0][0], 26);
    state->H[0][4] = modadd(state->H[0][4], state->H[1][2], 26);
    state->H[0][5] = modadd(state->H[0][5], state->H[2][3], 26);
    state->H[0][6] = modadd(state->H[0][6], state->H[3][4], 26);
    state->H[0][7] = modadd(state->H[0][7], state->H[0][1], 26);
    state->H[0][8] = modadd(state->H[0][8], state->H[1][3], 26);
    state->H[0][9] = modadd(state->H[0][9], state->H[2][4], 26);
    state->H[0][10] = modadd(state->H[0][10], state->H[3][5], 26);
    state->H[0][11] = modadd(state->H[0][11], state->H[0][2], 26);
    state->H[0][12] = modadd(state->H[0][12], state->H[1][4], 26);

    state->H[1][0] = modadd(state->H[1][0], state->H[2][5], 26);
    state->H[1][1] = modadd(state->H[1][1], state->H[3][6], 26);
    state->H[1][2] = modadd(state->H[1][2], state->H[0][3], 26);
    state->H[1][3] = modadd(state->H[1][3], state->H[1][5], 26);
    state->H[1][4] = modadd(state->H[1][4], state->H[2][6], 26);
    state->H[1][5] = modadd(state->H[1][5], state->H[3][7], 26);
    state->H[1][6] = modadd(state->H[1][6], state->H[0][4], 26);
    state->H[1][7] = modadd(state->H[1][7], state->H[1][6], 26);
    state->H[1][8] = modadd(state->H[1][8], state->H[2][7], 26);
    state->H[1][9] = modadd(state->H[1][9], state->H[3][8], 26);
    state->H[1][10] = modadd(state->H[1][10], state->H[0][5], 26);
    state->H[1][11] = modadd(state->H[1][11], state->H[1][7], 26);
    state->H[1][12] = modadd(state->H[1][12], state->H[2][8], 26);

    state->H[2][0] = modadd(state->H[2][0], state->H[3][9], 26);
    state->H[2][1] = modadd(state->H[2][1], state->H[0][6], 26);
    state->H[2][2] = modadd(state->H[2][2], state->H[1][8], 26);
    state->H[2][3] = modadd(state->H[2][3], state->H[2][9], 26);
    state->H[2][4] = modadd(state->H[2][4], state->H[3][10], 26);
    state->H[2][5] = modadd(state->H[2][5], state->H[0][7], 26);
    state->H[2][6] = modadd(state->H[2][6], state->H[1][8], 26);
    state->H[2][7] = modadd(state->H[2][7], state->H[2][10], 26);
    state->H[2][8] = modadd(state->H[2][8], state->H[3][11], 26);
    state->H[2][9] = modadd(state->H[2][9], state->H[0][8], 26);
    state->H[2][10] = modadd(state->H[2][10], state->H[1][9], 26);
    state->H[2][11] = modadd(state->H[2][11], state->H[2][11], 26);
    state->H[2][12] = modadd(state->H[2][12], state->H[3][12], 26);

    state->H[3][0] = modadd(state->H[3][0], state->H[0][9], 26);
    state->H[3][1] = modadd(state->H[3][1], state->H[1][10], 26);
    state->H[3][2] = modadd(state->H[3][2], state->H[2][12], 26);
    state->H[3][3] = modadd(state->H[3][3], state->H[3][0], 26);
    state->H[3][4] = modadd(state->H[3][4], state->H[0][10], 26);
    state->H[3][5] = modadd(state->H[3][5], state->H[1][11], 26);
    state->H[3][6] = modadd(state->H[3][6], state->H[2][0], 26);
    state->H[3][7] = modadd(state->H[3][7], state->H[3][1], 26);
    state->H[3][8] = modadd(state->H[3][8], state->H[0][11], 26);
    state->H[3][9] = modadd(state->H[3][9], state->H[1][12], 26);
    state->H[3][10] = modadd(state->H[3][10], state->H[2][1], 26);
    state->H[3][11] = modadd(state->H[3][11], state->H[3][2], 26);
    state->H[3][12] = modadd(state->H[3][12], state->H[0][12], 26);
}

void hexMix1(struct hexState * state) {
    state->H[3][0] = modadd(state->H[3][0], state->H[1][1], 26);
    state->H[2][1] = modadd(state->H[2][1], state->H[2][2], 26);
    state->H[1][2] = modadd(state->H[1][2], state->H[3][3], 26);
    state->H[0][3] = modadd(state->H[0][3], state->H[0][0], 26);
    state->H[3][4] = modadd(state->H[3][4], state->H[1][2], 26);
    state->H[2][5] = modadd(state->H[2][5], state->H[2][3], 26);
    state->H[1][6] = modadd(state->H[1][6], state->H[3][4], 26);
    state->H[0][7] = modadd(state->H[0][7], state->H[0][1], 26);
    state->H[3][8] = modadd(state->H[3][8], state->H[1][3], 26);
    state->H[2][9] = modadd(state->H[2][9], state->H[2][4], 26);
    state->H[1][10] = modadd(state->H[1][10], state->H[3][5], 26);
    state->H[0][11] = modadd(state->H[0][11], state->H[0][2], 26);
    state->H[3][12] = modadd(state->H[3][12], state->H[1][4], 26);

    state->H[2][0] = modadd(state->H[2][0], state->H[2][5], 26);
    state->H[1][1] = modadd(state->H[1][1], state->H[3][6], 26);
    state->H[0][2] = modadd(state->H[0][2], state->H[0][3], 26);
    state->H[3][3] = modadd(state->H[3][3], state->H[1][5], 26);
    state->H[2][4] = modadd(state->H[2][4], state->H[2][6], 26);
    state->H[1][5] = modadd(state->H[1][5], state->H[3][7], 26);
    state->H[0][6] = modadd(state->H[0][6], state->H[0][4], 26);
    state->H[3][7] = modadd(state->H[3][7], state->H[1][6], 26);
    state->H[2][8] = modadd(state->H[2][8], state->H[2][7], 26);
    state->H[1][9] = modadd(state->H[1][9], state->H[3][8], 26);
    state->H[0][10] = modadd(state->H[0][10], state->H[0][5], 26);
    state->H[3][11] = modadd(state->H[3][11], state->H[1][7], 26);
    state->H[2][12] = modadd(state->H[2][12], state->H[2][8], 26);

    state->H[1][0] = modadd(state->H[1][0], state->H[3][9], 26);
    state->H[0][1] = modadd(state->H[0][1], state->H[0][6], 26);
    state->H[3][2] = modadd(state->H[3][2], state->H[1][8], 26);
    state->H[2][3] = modadd(state->H[2][3], state->H[2][9], 26);
    state->H[1][4] = modadd(state->H[1][4], state->H[3][10], 26);
    state->H[0][5] = modadd(state->H[0][5], state->H[0][7], 26);
    state->H[3][6] = modadd(state->H[3][6], state->H[1][8], 26);
    state->H[2][7] = modadd(state->H[2][7], state->H[2][10], 26);
    state->H[1][8] = modadd(state->H[1][8], state->H[3][11], 26);
    state->H[0][9] = modadd(state->H[0][9], state->H[0][8], 26);
    state->H[3][10] = modadd(state->H[3][10], state->H[1][9], 26);
    state->H[2][11] = modadd(state->H[2][11], state->H[2][11], 26);
    state->H[1][12] = modadd(state->H[1][12], state->H[3][12], 26);

    state->H[0][0] = modadd(state->H[0][0], state->H[0][9], 26);
    state->H[3][1] = modadd(state->H[3][1], state->H[1][10], 26);
    state->H[2][2] = modadd(state->H[2][2], state->H[2][12], 26);
    state->H[1][3] = modadd(state->H[1][3], state->H[3][0], 26);
    state->H[0][4] = modadd(state->H[0][4], state->H[0][10], 26);
    state->H[3][5] = modadd(state->H[3][5], state->H[1][11], 26);
    state->H[2][6] = modadd(state->H[2][6], state->H[2][0], 26);
    state->H[1][7] = modadd(state->H[1][7], state->H[3][1], 26);
    state->H[0][8] = modadd(state->H[0][8], state->H[0][11], 26);
    state->H[3][9] = modadd(state->H[3][9], state->H[1][12], 26);
    state->H[2][10] = modadd(state->H[2][10], state->H[2][1], 26);
    state->H[1][11] = modadd(state->H[1][11], state->H[3][2], 26);
    state->H[0][12] = modadd(state->H[0][12], state->H[0][12], 26);
}

void hexSoak(struct hexState *state, int *block) {
    state->H[0][0] = modadd(state->H[0][0], block[0], 26);
    state->H[1][0] = modadd(state->H[1][0], block[1], 26);
    state->H[2][0] = modadd(state->H[2][0], block[2], 26);
    state->H[3][0] = modadd(state->H[3][0], block[3], 26);
    state->H[0][1] = modadd(state->H[0][1], block[4], 26);
    state->H[1][1] = modadd(state->H[1][1], block[5], 26);
    state->H[2][1] = modadd(state->H[2][1], block[6], 26);
    state->H[3][1] = modadd(state->H[3][1], block[7], 26);
    state->H[0][2] = modadd(state->H[0][2], block[8], 26);
    state->H[1][2] = modadd(state->H[1][2], block[9], 26);
    state->H[2][2] = modadd(state->H[2][2], block[10], 26);
    state->H[3][2] = modadd(state->H[3][2], block[11], 26);
    state->H[0][3] = modadd(state->H[0][3], block[12], 26);
    state->H[1][3] = modadd(state->H[1][3], block[13], 26);
    state->H[2][3] = modadd(state->H[2][3], block[14], 26);
    state->H[3][3] = modadd(state->H[3][3], block[15], 26);
    state->H[0][4] = modadd(state->H[0][4], block[16], 26);
    state->H[1][4] = modadd(state->H[1][4], block[17], 26);
    state->H[2][4] = modadd(state->H[2][4], block[18], 26);
    state->H[3][4] = modadd(state->H[3][4], block[19], 26);
    state->H[0][5] = modadd(state->H[0][5], block[20], 26);
    state->H[1][5] = modadd(state->H[1][5], block[21], 26);
    state->H[2][5] = modadd(state->H[2][5], block[22], 26);
    state->H[3][5] = modadd(state->H[3][5], block[23], 26);
    state->H[0][6] = modadd(state->H[0][6], block[24], 26);
    state->H[1][6] = modadd(state->H[1][6], block[25], 26);
}

void hexAddLast(struct hexState *state) {
    for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 13; y++) {
            state->H[x][y] = modadd(state->H[x][y], state->T[x][y], 26);
        }
    }
}

void hexUpdate(struct hexState *state, int *block) {
    memcpy(state->H, state->T, 52 * sizeof(int));
    hexSoak(state, block);
    for (int r = 0; r < state->rounds; r++) {
        hexMix0(state);
        rotateBlockLeft(state->H, 4, 1);
        hexMix1(state);
    }
    hexAddLast(state);
}

void hexRounds(struct hexState *state) {
    memcpy(state->T, state->H, 52 * sizeof(int));
    for (int r = 0; r < state->rounds; r++) {
        hexMix0(state);
        rotateBlockLeft(state->H, 4, 1);
        hexMix1(state);
    }
    hexAddLast(state);
}

void hexOutput(struct hexState *state, uint8_t *output, int outputLen) {
    int c = 0;
    int blocks = outputLen / 26;
    int blockExtra = outputLen % 26;
    int h[26];
    int o[outputLen];
    memset(o, 0, outputLen * sizeof(int));
    int blocklen = 26;
    int x = 0;
    int y = 0;
    for (int b = 0; b < blocks; b++) {
        if ((b == (blocks - 1)) && (blockExtra != 0)) {
            blocklen = blockExtra;
        }
        memcpy(h, state->H, 26 * sizeof(int));
        hexUpdate(state, h);
        for (int i = 0; i < blocklen; i++) {
            o[c] = state->H[x][y];
            if ((c % 4) == 0) {
                x = modadd(x, 1, 4);
            }
            y = modadd(y, 1, 13);
            c += 1;
        }
    }
    convertIntstoU8(o, output, outputLen);
}

void hexKDF(uint8_t *hashOutput, int outLen, uint8_t *src, int srcLen) {
    struct hexState state;
    hexInit(&state);
    int i;
    int x = 0;
    int y = 0;
    int s[srcLen];
    int o[outLen];
    memset(s, 0, srcLen * (sizeof(int)));
    int outputBlocks = outLen / state.blocklen;
    int outputBlocksExtra = outLen % state.blocklen;
    int outputBlockLen = state.blocklen;
    convertU8toInts(src, s, srcLen);
    for (i = 0; i < srcLen; i++) {
        state.H[x][y] = modadd(state.H[x][y], s[i], 26);
        if ((i % 4) == 0) {
            x = modadd(x, 1, 4);
        }
        y = modadd(y, 1, 13);
    }
    for (i = 0; i < state.iterations; i++) {
        hexRounds(&state);
    }
    hexOutput(&state, hashOutput, outLen);
}
