#pragma GCC optimize ("-Ofast")

#ifndef Ducos1a_avr_h
#define Ducos1a_avr_h

#include "sha1.h"

class Ducos1a_avr
{
public:
  Sha1Wrapper Sha1_base;
  // DUCO-S1A hasher
  uint32_t work(String lastblockhash, String newblockhash, int difficulty)
  {
    // DUCO-S1 algorithm implementation for AVR boards (DUCO-S1A)
  newblockhash.toUpperCase();
  const char *c = newblockhash.c_str();
  uint8_t final_len = newblockhash.length() >> 1;
  uint8_t job[final_len+1];
  for (uint8_t i = 0, j = 0; j < final_len; i += 2, j++)
    job[j] = ((((c[i] & 0x1F) + 9) % 25) << 4) + ((c[i + 1] & 0x1F) + 9) % 25;

  if (difficulty > 655)
    return 0;

  Sha1_base.init();
  Sha1.print(lastblockhash);
  for (int ducos1res = 0; ducos1res < difficulty * 100 + 1; ducos1res++)
  {
    Sha1 = Sha1_base;
    Sha1.print(String(ducos1res));
    // Get SHA1 result
    uint8_t *hash_bytes = Sha1.result();
    if (memcmp(hash_bytes, job, SHA1_HASH_LEN*sizeof(char)) == 0)
    {
      // If expected hash is equal to the found hash, return the result
      return ducos1res;
    }
  }
  return 0;
  }
};

#define Ducos1a_t Ducos1a_avr

#endif

