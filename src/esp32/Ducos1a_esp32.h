#ifndef Ducos1a_esp32_h
#define Ducos1a_esp32_h

#include <Arduino.h>
#include "mbedtls/md.h"   // Include software hashing library
#include "hwcrypto/sha.h" // Include hardware accelerated hashing library

class Ducos1a_esp32
{
public:
    // DUCO-S1A hasher
    uint32_t work(String lastblockhash, String newblockhash, int difficulty)
    {
        // DUCO-S1 algorithm implementation for ESP32 boards (DUCO-S1A)
        newblockhash.toUpperCase();
        const char *c = newblockhash.c_str();
        size_t len = strlen(c);
        size_t final_len = len / 2;
        uint8_t job[final_len+1];
        for (size_t i = 0, j = 0; j < final_len; i += 2, j++)
            job[j] = (c[i] % 32 + 9) % 25 * 16 + (c[i + 1] % 32 + 9) % 25;

        for (int ducos1res = 0; ducos1res < difficulty * 100 + 1; ducos1res++)
        {
            String hash11 = String(lastblockhash) + String(ducos1res);
            const unsigned char *payload1 = (const unsigned char *)hash11.c_str();
            unsigned int payloadLenght1 = hash11.length();

            byte shaResult1[20];
            esp_sha(SHA1, payload1, payloadLenght1, shaResult1);

            if (memcmp(shaResult1, job, sizeof(shaResult1)) == 0)
            {
                // If expected hash is equal to the found hash, return the result
                return ducos1res;
            }
            yield(); // uncomment if ESP watchdog triggers
        }
        return 0;
    }

    uint32_t work2(String lastblockhash, String newblockhash, int difficulty)
    {
        // DUCO-S1 algorithm implementation for ESP32 boards (DUCO-S1A)
        newblockhash.toUpperCase();
        const char *c = newblockhash.c_str();
        size_t len = strlen(c);
        size_t final_len = len / 2;
        uint8_t job[final_len+1];
        for (size_t i = 0, j = 0; j < final_len; i += 2, j++)
            job[j] = (c[i] % 32 + 9) % 25 * 16 + (c[i + 1] % 32 + 9) % 25;

        for (int ducos1res = 0; ducos1res < difficulty * 100 + 1; ducos1res++)
        {
            String hash1 = String(lastblockhash) + String(ducos1res);
            const unsigned char *payload = (const unsigned char *)hash1.c_str();
            unsigned int payloadLength = hash1.length();

            byte shaResult[20];
            mbedtls_md_context_t ctx;
            mbedtls_md_type_t md_type = MBEDTLS_MD_SHA1;
            mbedtls_md_init(&ctx);
            mbedtls_md_setup(&ctx, mbedtls_md_info_from_type(md_type), 0);
            mbedtls_md_starts(&ctx);
            mbedtls_md_update(&ctx, (const unsigned char *)payload, payloadLength);
            mbedtls_md_finish(&ctx, shaResult);
            mbedtls_md_free(&ctx);

            if (memcmp(shaResult, job, sizeof(shaResult)) == 0)
            {
                // If expected hash is equal to the found hash, return the result
                return ducos1res;
            }
            yield(); // uncomment if ESP watchdog triggers
        }
        return 0;
    }
};

#define Ducos1a_t Ducos1a_esp32

#endif