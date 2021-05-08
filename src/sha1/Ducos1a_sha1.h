#ifndef Ducos1a_sha1_h
#define Ducos1a_sha1_h

#include "sha1/sha1.h"

class Ducos1a_sha1
{
public:
  // DUCO-S1A hasher
  uint32_t work(String lastblockhash, String newblockhash, int difficulty)
  {
    newblockhash.toUpperCase();
    const char *c = newblockhash.c_str();

    size_t len = strlen(c);
    size_t final_len = len / 2;
    unsigned char *job = (unsigned char *)malloc((final_len + 1) * sizeof(unsigned char));
    for (size_t i = 0, j = 0; j < final_len; i += 2, j++)
      job[j] = (c[i] % 32 + 9) % 25 * 16 + (c[i + 1] % 32 + 9) % 25;

    // DUCO-S1 algorithm implementation for AVR boards (DUCO-S1A)
    // Difficulty loop
    int ducos1res = 0;
    for (int ducos = 0; ducos < difficulty * 100 + 1; ducos++)
    {
      Sha1.init();
      Sha1.print(lastblockhash + ducos);
      // Get SHA1 result
      uint8_t *hash_bytes = Sha1.result();

      if (memcmp(hash_bytes, job, sizeof(hash_bytes)) == 0)
      {
        // If expected hash is equal to the found hash, return the result
        ducos1res = ducos;
        break;
      }
    }
    free( job );
    return ducos1res;
  }
};

#define Ducos1a_t Ducos1a_sha1

#endif