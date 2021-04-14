// Adapted from https://developer.mbed.org/teams/myDevicesIoT/code/Cayenne-LPP/

// Copyright © 2017 The Things Network
// Use of this source code is governed by the MIT license that can be found in the LICENSE file.

#include "DoinoCoin.h"

DoinoCoin::DoinoCoin(HardwareSerial &stream) : serial(stream)
{
  
}

DoinoCoin::~DoinoCoin(void)
{
  
}

void DoinoCoin::begin(void)
{
  serial.begin(115200); // Open serial port
  serial.setTimeout(5000);
  if (serial.available()) {
    serial.println("ready"); // Send start word to miner program
    return;
  }
  serial.println(DUINOCOIN_VERSION);
}

bool DoinoCoin::loop(void)
{
  String IDstring = "DUCOID";
  String lastblockhash = "";
  String newblockhash = "";
  unsigned int difficulty = 0;
  unsigned int ducos1result = 0;

  if (serial.available() > 0) {
    // Read last block hash
    lastblockhash = serial.readStringUntil(',');
    // Read expected hash
    newblockhash = serial.readStringUntil(',');
    // Read difficulty
    difficulty = serial.readStringUntil(',').toInt();
    // Start time measurement
    unsigned long startTime = micros();
    // Call DUCO-S1A hasher
    ducos1result = ducos1a(lastblockhash, newblockhash, difficulty);
    // End time measurement
    unsigned long endTime = micros();
    // Calculate elapsed time
    unsigned long elapsedTime = endTime - startTime;
    // Send result back to the program with share time
    serial.print(String(ducos1result) + "," + String(elapsedTime) + "," + String(IDstring) + "\n");
    return true;
  }
  return false;
}

bool DoinoCoin::work(void)
{
  String result; // Create globals
  char buffer[64] = "";
  unsigned int iJob = 0;

  String startStr = serial.readStringUntil('\n');
  if (startStr == "start") { // Wait for start word, serial.available caused problems
    serial.flush(); // Clear serial buffer
    String hash = serial.readStringUntil('\n'); // Read hash
    String job = serial.readStringUntil('\n'); // Read job
    unsigned int diff = serial.parseInt() * 100 + 1; // Read difficulty
    unsigned long StartTime = micros(); // Start time measurement
    #ifdef REVERSE_SEARCH
    for (unsigned int iJob = diff; iJob >= 0; iJob--) { // Reversed difficulty loop
    #else
    for (unsigned int iJob = 0; iJob < diff; iJob++) { // Difficulty loop
    #endif
      Sha1.init(); // Create SHA1 hasher
      Sha1.print(String(hash) + String(iJob));
      uint8_t * hash_bytes = Sha1.result(); // Get result
      for (int i = 0; i < 10; i++) { // Cast result to array
        for (int i = 0; i < 32; i++) {
          buffer[2 * i] = "0123456789abcdef"[hash_bytes[i] >> 4]; // MSB to LSB (Depending on the address in hash_bytes)
          // Choose that from the given array of characters
          buffer[2 * i + 1] = "0123456789abcdef"[hash_bytes[i] & 0xf]; // Retreve the value from address next spot over
        }
      }
      result = String(buffer); // Convert and prepare array
      result.remove(40); // First 40 characters are good, rest is garbage
      if (String(result) == String(job)) { // If result is found
        unsigned long EndTime = micros(); // End time measurement
        unsigned long ElapsedTime = EndTime - StartTime; // Calculate elapsed time
        serial.println(String(iJob) + "," + String(ElapsedTime)); // Send result back to the program with share time
        return true; // Stop the loop and wait for more work
      }
    }
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