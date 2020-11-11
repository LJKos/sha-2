
#ifndef SHA256_H
#define SHA256_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>



int sha256(uint32_t hash[8], void *data, size_t bytes) {
    size_t bits = bytes * 8;
    size_t zeros = (bits % 512 > 447 ? 960 : 448) - (bits % 512 + 1);
    size_t total_bits = bits + 1 + zeros + 64;
    size_t total_bytes = total_bits / 8;
    
    size_t block_count = total_bits / 512;
    
    uint32_t *blocks = malloc(total_bytes);
    if (!blocks)
        return -1;
    
    memset(blocks, 0, total_bytes);
    
    uint8_t *byte = (uint8_t *) data;
    
    for (size_t i = 0; i < bytes; i++) {
        blocks[i / 4] |= (uint32_t) byte[i] << ((3 - (i % 4)) * 8);
    }
    
    blocks[bytes / 4] |= 0x80000000 >> (bits % 32);
    blocks[block_count * 16 - 1] = (uint32_t) bits;
    
    uint32_t ha[8] = {
        0x6a09e667,
        0xbb67ae85,
        0x3c6ef372,
        0xa54ff53a,
        0x510e527f,
        0x9b05688c,
        0x1f83d9ab,
        0x5be0cd19
    };
    
    uint32_t k[64] = {
        0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
        0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
        0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
        0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
        0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
        0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
        0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
        0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
    };
    
    uint32_t w[64];
    uint32_t a, b, c, d, e, f, g, h, t1, t2;
    
    for (size_t i = 0; i < block_count; i++) {
        a = ha[0];
        b = ha[1];
        c = ha[2];
        d = ha[3];
        e = ha[4];
        f = ha[5];
        g = ha[6];
        h = ha[7];
        
        for (size_t w_j = 0; w_j < 16; w_j++) {
            w[w_j] = blocks[16 * i + w_j];
        }
        
        for (size_t w_j = 16; w_j < 64; w_j++) {
            w[w_j] = (((w[w_j - 2] >> 17) | (w[w_j - 2] << 15)) ^ ((w[w_j - 2] >> 19) | (w[w_j - 2] << 13)) ^ (w[w_j - 2] >> 10));
            w[w_j] += w[w_j - 7];
            w[w_j] += (((w[w_j - 15] >> 7) | (w[w_j - 15] << 25)) ^ ((w[w_j - 15] >> 18) | (w[w_j - 15] << 14)) ^ (w[w_j - 15] >> 3));
            w[w_j] += w[w_j - 16];
        }
        
        for (size_t j = 0; j < 64; j++) {
            t1 = h + (((e >> 6) | (e << 26)) ^ ((e >> 11) | (e << 21)) ^ ((e >> 25) | (e << 7)));
            t1 += ((e & f) ^ (~e & g)) + k[j] + w[j];
            
            t2 = (((a >> 2) | (a << 30)) ^ ((a >> 13) | (a << 19)) ^ ((a >> 22) | (a << 10)));
            t2 += ((a & b) ^ (a & c) ^ (b & c));
            
            h = g;
            g = f;
            f = e;
            e = d + t1;
            d = c;
            c = b;
            b = a;
            a = t1 + t2;
        }
        
        ha[0] += a;
        ha[1] += b;
        ha[2] += c;
        ha[3] += d;
        ha[4] += e;
        ha[5] += f;
        ha[6] += g;
        ha[7] += h;
    }
    
    memcpy(hash, ha, 32);
    
    free(blocks);
    
    return 0;
}

#endif
