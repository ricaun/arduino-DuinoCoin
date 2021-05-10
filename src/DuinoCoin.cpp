
#include "DuinoCoin.h"

DuinoCoin::DuinoCoin(HardwareSerial &stream) : serial(stream)
{

}

DuinoCoin::~DuinoCoin(void)
{
  
}

void DuinoCoin::begin(void)
{
  serial.begin(115200); // Open serial port
  while (!serial);
  serial.setTimeout(5000);
}

bool DuinoCoin::loop(void)
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
    serial.print(String(ducos1result) + "," + String(elapsedTime) + "," + String(getDUCOID()) + "\n");
    return true;
  }
  return false;
}

// DUCO-S1A hasher
uint32_t DuinoCoin::ducos1a(String lastblockhash, String newblockhash, int difficulty) {
  return Ducos1a.work(lastblockhash, newblockhash, difficulty);
}

// Grab Arduino chip DUCOID
String DuinoCoin::getDUCOID() {
  String ID = "DUCOID";
  char buff[4];
  for (size_t i = 0; i < 8; i++)
  {
    sprintf(buff, "%02X", (uint8_t) UniqueID8[i]);
    ID += buff;
  }
  return ID;
}