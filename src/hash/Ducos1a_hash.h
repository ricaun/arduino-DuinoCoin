#ifndef Ducos1a_hash_h
#define Ducos1a_hash_h

#include "Hash/src/Hash.h"

class Ducos1a_hash
{
public:
  // DUCO-S1A hasher
  uint32_t work(String lastblockhash, String newblockhash, int difficulty)
  {
    // DUCO-S1 algorithm implementation for AVR boards (DUCO-S1A)
    // Difficulty loop
    int ducos1res = 0;
    for (int ducos1res = 0; ducos1res < difficulty * 100 + 1; ducos1res++)
    {
      String result = sha1(lastblockhash + String(ducos1res));
      if (String(result) == String(newblockhash))
      {
        // If expected hash is equal to the found hash, return the result
        return ducos1res;
      }
    }
    return 0;
  }
};

#define Ducos1a_t Ducos1a_hash

#endif