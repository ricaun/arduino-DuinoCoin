
#include "DoinoCoin.h"

DoinoCoin::DoinoCoin(HardwareSerial &stream) : serial(stream)
{
  // Grab Arduino chip ID
  ID = "DUCOID";
  for (size_t i = 0; i < 8; i++)
    ID += UniqueID[i];
}

DoinoCoin::~DoinoCoin(void)
{
  
}

void DoinoCoin::begin(void)
{
  serial.begin(115200); // Open serial port
  serial.setTimeout(5000);
}

void DoinoCoin::test(Stream &stream)
{
  stream.print("251 ");
  stream.println(ducos1a("2eebb02e9b4995c15d46727dc0427478e738a505", "73a34a3ca74449c0565961476772714e940c9cf0", 6));
  stream.print("482 ");
  stream.println(ducos1a("68958c4aab39fbfb4453fb4ee0398a580af7abaf", "b430b3b1d76a6b1ae0464b327e786f43801a02cb", 6));
  stream.print("185 ");
  stream.println(ducos1a("f5da2de2ca442868678363e28298bf16f1e3a856", "5a9320c0015d1dd4eee6f90ad1a7d429577abf88", 6));
}

bool DoinoCoin::loop(void)
{
  if (serial.available() > 0) {
    // Read last block hash
    String lastblockhash = serial.readStringUntil(',');
    // Read expected hash
    String newblockhash = serial.readStringUntil(',');
    // Read difficulty
    unsigned int difficulty = serial.readStringUntil(',').toInt();
    // Start time measurement
    unsigned long startTime = micros();
    // Call DUCO-S1A hasher
    unsigned int ducos1result = ducos1a(lastblockhash, newblockhash, difficulty);
    // End time measurement
    unsigned long endTime = micros();
    // Calculate elapsed time
    unsigned long elapsedTime = endTime - startTime;
    // Send result back to the program with share time
    serial.print(String(ducos1result) + "," + String(elapsedTime) + "," + String(ID) + "\n");
    return true;
  }
  return false;
}

// DUCO-S1A hasher
uint32_t DoinoCoin::ducos1a(String lastblockhash, String newblockhash, int difficulty) {
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
