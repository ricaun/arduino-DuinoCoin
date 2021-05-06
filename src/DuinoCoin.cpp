
#include "DuinoCoin.h"

DuinoCoin::DuinoCoin(HardwareSerial &stream) : serial(stream)
{
  // Grab Arduino chip ID
  ID = "DUCOID";
    char buff[4];
  for (size_t i = 0; i < 8; i++)
  {
    sprintf(buff, "%02X", (uint8_t) UniqueID8[i]);
    ID += buff;
  }
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

void DuinoCoin::test(Stream &stream)
{
  unsigned long time = 0;

  time = millis();
  stream.print("251 = ");
  stream.print(ducos1a("2eebb02e9b4995c15d46727dc0427478e738a505", "73a34a3ca74449c0565961476772714e940c9cf0", 6));
  stream.print(" | Time: ");
  stream.println(millis() - time);

  time = millis();
  stream.print("482 = ");
  stream.print(ducos1a("68958c4aab39fbfb4453fb4ee0398a580af7abaf", "b430b3b1d76a6b1ae0464b327e786f43801a02cb", 6));
  stream.print(" | Time: ");
  stream.println(millis() - time);

  time = millis();
  stream.print("185 = ");
  stream.print(ducos1a("f5da2de2ca442868678363e28298bf16f1e3a856", "5a9320c0015d1dd4eee6f90ad1a7d429577abf88", 6));
  stream.print(" | Time: ");
  stream.println(millis() - time);
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
    serial.print(String(ducos1result) + "," + String(elapsedTime) + "," + String(ID) + "\n");
    return true;
  }
  return false;
}

// DUCO-S1A hasher
uint32_t DuinoCoin::ducos1a(String lastblockhash, String newblockhash, int difficulty) {
  return Ducos1a.work(lastblockhash, newblockhash, difficulty);
}
