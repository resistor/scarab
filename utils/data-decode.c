#include <stdio.h>
#include <stdint.h>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Insufficient arguements\n");
        return -1;
    }

    FILE* DATA = fopen(argv[1], "r");
    FILE* ZERO = fopen(argv[2], "r");

    uint16_t DATAWord = 0;
    while (fread(&DATAWord, 2, 1, DATA) == 1) {
        fwrite(&DATAWord, 2, 1, stdout);

        if (DATAWord == 0) {
            uint16_t ZEROCount = 0;
            if (!fread(&ZEROCount, 2, 1, ZERO)) {
                fprintf(stderr, "Could not read ZERO  file\n");
                return -1;
            }

            // Classic Mac was big endian.
            ZEROCount = __builtin_bswap16(ZEROCount);

            unsigned char NullByte = 0;
            while (ZEROCount--) {
                fwrite(&NullByte, 1, 1, stdout);
            }
        }
    }

    unsigned char ZEROCount = 0;
    if (fread(&ZEROCount, 1, 1, ZERO)) {
        fprintf(stderr, "ZERO file not fully consumed!\n");
        return -1;
    }

    fclose(DATA);
    fclose(ZERO);

    return 0;
}