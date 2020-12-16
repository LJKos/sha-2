
#include <stdio.h>
#include "sha256.h"
#include "sha512.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("%s <bits> <input string>\n", argv[0]);
        return 0;
    }
    
    int bits = atoi(argv[1]);
    
    if (bits == 256) {
        uint32_t hash[8];
        sha256(hash, argv[2], strlen(argv[2]));
        printf("%08x%08x%08x%08x%08x%08x%08x%08x\n",
               hash[0], hash[1], hash[2], hash[3], hash[4], hash[5], hash[6], hash[7]);
    } else if (bits == 512) {
        uint64_t hash[8];
        sha512(hash, argv[2], strlen(argv[2]));
        printf("%016llx%016llx%016llx%016llx%016llx%016llx%016llx%016llx\n",
               hash[0], hash[1], hash[2], hash[3], hash[4], hash[5], hash[6], hash[7]);
    } else {
        printf("Invalid bit count:\n%s <256/512> ...\n", argv[0]);
    }
    
    return 0;
}
