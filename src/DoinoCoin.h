#ifndef _DOINOCOIN_LPP_H_
#define _DOINOCOIN_LPP_H_

#include <Arduino.h>
#include <ArduinoUniqueID.h>

#define DUINOCOIN_VERSION "DuinoCoin Arduino Library Version 1.0.0"

#if defined(ARDUINO_ARCH_AVR)
#include "avr/sha1.h"
#endif

class DoinoCoin
{
public:
  DoinoCoin(HardwareSerial &stream);
  ~DoinoCoin();

  void begin();
  bool loop();

  void test(Stream &stream);
  
  uint32_t ducos1a(String lastblockhash, String newblockhash, int difficulty);

private:
  
  HardwareSerial &serial;
  String ID;
};

#endif
