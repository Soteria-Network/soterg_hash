#include "soterg.h"
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "sha3/sph_blake.h"
#include "sha3/sph_groestl.h"
#include "sha3/sph_jh.h"
#include "sha3/sph_keccak.h"
#include "sha3/sph_skein.h"
#include "sha3/sph_luffa.h"
#include "sha3/sph_cubehash.h"
#include "sha3/sph_simd.h"
#include "sha3/sph_echo.h"
#include "sha3/sph_hamsi.h"
#include "sha3/sph_shabal.h"
#include "sha3/sph_sha2.h"

// Keep exactly 12 algorithms
enum Algo {
    BLAKE = 0,
    GROESTL,
    JH,
    KECCAK,
    SKEIN,
    LUFFA,
    CUBEHASH,
    SIMD,
    ECHO,
    HAMSI,
    SHABAL,
    SHA512,
    HASH_FUNC_COUNT
};

static void getAlgoString(const uint32_t* timeHash, char *output)
{
    // Produce exactly HASH_FUNC_COUNT (12) digits from timeHash nibbles.
    // We read 12 hex nibbles from the end, similar to the old reversed scheme.
    char *sptr = output;
    const uint8_t* data = (const uint8_t*)timeHash;

    for (int j = 0; j < HASH_FUNC_COUNT; j++) {
        // 12 ascii hex chars, reversed
        uint8_t b = (11 - j) >> 1; // was 15 for 16; now 11 for 12
        uint8_t hi = data[b] >> 4;
        uint8_t lo = data[b] & 0xF;
        uint8_t nibble = (j & 1) ? lo : hi;

        // Collapse nibble range 0..15 to 0..11 deterministically
        uint8_t algoDigit = (uint8_t)(nibble % HASH_FUNC_COUNT);

        // Write as single hex-like char (0..9 then A..B for 10..11)
        if (algoDigit >= 10)
            sprintf(sptr, "%c", 'A' + (algoDigit - 10)); // A, B
        else
            sprintf(sptr, "%u", (unsigned)algoDigit);
        sptr++;
    }
    *sptr = '\0';
}

static void getTimeHash(const uint32_t timeStamp, void* timeHash)
{
    // Mask timestamp and double-SHA256 into 32 bytes
    int32_t maskedTime = timeStamp & TIME_MASK;
    sha256d((unsigned char*)timeHash, (const unsigned char*)&maskedTime, sizeof(maskedTime));
}

// Optionally rename to x12rt_hash
void soterg_hash(const char* input, char* output)
{
    uint32_t hash[64/4];
    char hashOrder[HASH_FUNC_COUNT + 1] = { 0 };

    sph_blake512_context     ctx_blake;
    sph_groestl512_context   ctx_groestl;
    sph_skein512_context     ctx_skein;
    sph_jh512_context        ctx_jh;
    sph_keccak512_context    ctx_keccak;
    sph_luffa512_context     ctx_luffa;
    sph_cubehash512_context  ctx_cubehash;
    sph_simd512_context      ctx_simd;
    sph_echo512_context      ctx_echo;
    sph_hamsi512_context     ctx_hamsi;
    sph_shabal512_context    ctx_shabal;
    sph_sha512_context       ctx_sha512;

    void *in = (void*) input;
    int size = 80;

    const uint32_t *in32 = (const uint32_t*) input;
    uint32_t ntime = in32[17]; // as-is
    uint32_t timeHash[8];      // 32 bytes

    getTimeHash(ntime, &timeHash[0]);
    getAlgoString(&timeHash[0], hashOrder);

    for (int i = 0; i < HASH_FUNC_COUNT; i++) {
        const char elem = hashOrder[i];
        // Map char back to digit 0..11
        const uint8_t algo = (elem >= 'A') ? (uint8_t)(elem - 'A' + 10) : (uint8_t)(elem - '0');

        switch (algo) {
            case BLAKE:
            sph_blake512_init(&ctx_blake);
            sph_blake512(&ctx_blake, in, size);
            sph_blake512_close(&ctx_blake, hash);
            break;
            case GROESTL:
            sph_groestl512_init(&ctx_groestl);
            sph_groestl512(&ctx_groestl, in, size);
            sph_groestl512_close(&ctx_groestl, hash);
            break;
            case SKEIN:
            sph_skein512_init(&ctx_skein);
            sph_skein512(&ctx_skein, in, size);
            sph_skein512_close(&ctx_skein, hash);
            break;
            case JH:
            sph_jh512_init(&ctx_jh);
            sph_jh512(&ctx_jh, in, size);
            sph_jh512_close(&ctx_jh, hash);
            break;
            case KECCAK:
            sph_keccak512_init(&ctx_keccak);
            sph_keccak512(&ctx_keccak, in, size);
            sph_keccak512_close(&ctx_keccak, hash);
            break;
            case LUFFA:
            sph_luffa512_init(&ctx_luffa);
            sph_luffa512(&ctx_luffa, in, size);
            sph_luffa512_close(&ctx_luffa, hash);
            break;
            case CUBEHASH:
            sph_cubehash512_init(&ctx_cubehash);
            sph_cubehash512(&ctx_cubehash, in, size);
            sph_cubehash512_close(&ctx_cubehash, hash);
            break;
            case SIMD:
            sph_simd512_init(&ctx_simd);
            sph_simd512(&ctx_simd, in, size);
            sph_simd512_close(&ctx_simd, hash);
            break;
            case ECHO:
            sph_echo512_init(&ctx_echo);
            sph_echo512(&ctx_echo, in, size);
            sph_echo512_close(&ctx_echo, hash);
            break;
            case HAMSI:
            sph_hamsi512_init(&ctx_hamsi);
            sph_hamsi512(&ctx_hamsi, in, size);
            sph_hamsi512_close(&ctx_hamsi, hash);
            break;
            case SHABAL:
            sph_shabal512_init(&ctx_shabal);
            sph_shabal512(&ctx_shabal, in, size);
            sph_shabal512_close(&ctx_shabal, hash);
            break;
            case SHA512:
            sph_sha512_init(&ctx_sha512);
            sph_sha512(&ctx_sha512,(const void*) in, size);
            sph_sha512_close(&ctx_sha512,(void*) hash);
            break;
        }

        in = (void*) hash;
        size = 64;
    }

    // Truncate to 256-bit output (first 32 bytes of the last 512-bit state)
    memcpy(output, hash, 32);
}
