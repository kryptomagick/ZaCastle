int mod(int a, int m) {
    a = a % m;
    if (a < 0) {
        a += m;
    }
    return a;
}

int modadd(int a, int b, int m) {
    int x = (a + b) % m;
    if (x < 0) {
        x += m;
    }
    return x;
}

int modsub(int a, int b, int m) {
    int x = (a - b) % m;
    if (x < 0) {
        x += m;
    }
    return x;
}

int modmul(int a, int b, int m) {
    int x = (a * b) % m;
    if (x < 0) {
        x += m;
    }
    return x;
}

void modaddArray(int *a, int *b, int aLen) {
    for (int x = 0; x < aLen; x++) {
        a[x] = modadd(a[x], b[x], 26);
    }
}

void modsubArray(int *a, int *b, int aLen) {
    for (int x = 0; x < aLen; x++) {
        a[x] = modsub(a[x], b[x], 26);
    }
}

void rotateBlockLeft(int *block, int blocklen, int v) {
    int tmp, z, r;
    int rotateOne = (blocklen - 1);
    for (int x = 0; x < v; x++) {
        for (int y = 0; y < rotateOne; y++) {
            z = (y + 1) % blocklen;
            tmp = block[y];
            block[y] = block[z];
            block[z] = tmp;
        }
    }
}

void rotateBlockRight(int *block, int blocklen, int v) {
    int tmp, z;
    int rotateOne = blocklen - 1;
    for (int x = 0; x < v; x++) {
        for (int y = rotateOne; y != 0; y--) {
            z = (y + 1) % blocklen;
            tmp = block[y];
            block[y] = block[z];
            block[z] = tmp;
        }
    }
}

void convertU8toInts(uint8_t *binString, int *nums, int binStringLen) {
    for (int x = 0; x < binStringLen; x++) {
        nums[x] = (binString[x] - 65);
    }
}

void convertIntstoU8(int *nums, uint8_t *binString, int numsLen) {
    for (int x = 0; x < numsLen; x++) {
        binString[x] = (nums[x] + 65);
    }
}

void convertPassphrasetoInts(char *passphrase, int *pass, int passphraseLen) {
    for (int x = 0; x < passphraseLen; x++) {
        pass[x] = passphrase[x] - 65;
    }
}

long getFileSize(char *filename) {
    FILE *infile;
    infile = fopen(filename, "r");
    fseek(infile, 0, SEEK_END);
    long fsize = ftell(infile);
    fclose(infile);
    return fsize;
}

int isFile(char *filename) {
    if (access(filename, F_OK) == -1 ) {
        printf("%s not found\n", filename);
        exit(1);
    }
}

int collectPassphrase(unsigned char *passphrase) {
/* Disable Terminal Echo */
    struct termios tp, save;
    tcgetattr(STDIN_FILENO, &tp);
    save = tp;
    tp.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &tp);
    printf("ihit\n");

/* Collect passphrase */
    printf("Enter secret key passphrase: ");
    scanf("%s", passphrase);

/* Enable Terminal Echo */
    tcsetattr(STDIN_FILENO, TCSANOW, &save);
    return sizeof(passphrase) / sizeof(unsigned char*);
}

void getRandomAZ(uint8_t *randBlock, int blockLen) {
    FILE *randfile;
    int chk = 0;
    uint8_t tmp;
    randfile = fopen("/dev/urandom", "rb");
    while (chk != blockLen) {
        fread(&tmp, 1, 1, randfile);
        if ((tmp >= 0) && (tmp <= 25)) {
            randBlock[chk] = tmp + 65;
            chk += 1;
        }
    }
    fclose(randfile);
}
