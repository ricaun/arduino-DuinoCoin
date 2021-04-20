#ifndef Ducos1a_esp_h
#define Ducos1a_esp_h

#include <Arduino.h>
#include <Crypto.h> // experimental SHA1 crypto library
using namespace experimental::crypto;

class Ducos1a_esp
{
public:
    // DUCO-S1A hasher
    uint32_t work(String lastblockhash, String newblockhash, int difficulty)
    {
        newblockhash.toUpperCase();
        // DUCO-S1 algorithm implementation for AVR boards (DUCO-S1A)
        // Difficulty loop
        int ducos1res = 0;
        for (int ducos1res = 0; ducos1res < difficulty * 100 + 1; ducos1res++)
        {
            String result = SHA1::hash(lastblockhash + String(ducos1res));
            if (String(result) == String(newblockhash))
            {
                // If expected hash is equal to the found hash, return the result
                return ducos1res;
            }
            ESP.wdtFeed();
        }
        return 0;
    }
};

#define Ducos1a_t Ducos1a_esp

#endif