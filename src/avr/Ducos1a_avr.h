#ifndef Ducos1a_avr_h
#define Ducos1a_avr_h

#include "sha1.h"
/*
class Ducos1a_avr : public Ducos1a_t
{
    public:
        uint32_t work(String lastblockhash, String newblockhash, int difficulty);
};
*/
//extern Ducos1a_avr Ducos1a;

class Ducos1a_avr
{
    public:
    // DUCO-S1A hasher
    uint32_t work(String lastblockhash, String newblockhash, int difficulty)
    {
      char buffer[44];
      // DUCO-S1 algorithm implementation for AVR boards (DUCO-S1A)
      // Difficulty loop
      int ducos1res = 0;
      for (int ducos1res = 0; ducos1res < difficulty * 100 + 1; ducos1res++) {
        Sha1.init();
        Sha1.print(lastblockhash + ducos1res);
        // Get SHA1 result
        uint8_t * hash_bytes = Sha1.result();
        // Cast result to array
        for (int i = 0; i < 10; i++) {
          for (int i = 0; i < 20; i++) {
            // MSB to LSB (Depending on the address in hash_bytes)
            buffer[2 * i] = "0123456789abcdef"[hash_bytes[i] >> 4];
            // Choose that from the given array of characters
            // and retreve the value from address next spot over
            buffer[2 * i + 1] = "0123456789abcdef"[hash_bytes[i] & 0xf];
          }
        }
        if (String(buffer) == String(newblockhash)) {
          // If expected hash is equal to the found hash, return the result
          return ducos1res;
        }
      }
      return 0;
    }   
};

#define Ducos1a_t Ducos1a_avr

#endif