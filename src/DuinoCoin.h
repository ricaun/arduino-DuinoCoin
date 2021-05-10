#ifndef _DuinoCoin_H_
#define _DuinoCoin_H_

#include <Arduino.h>
#include <ArduinoUniqueID.h>
#include "Ducos1a.h"

class DuinoCoin
{
public:
  DuinoCoin(HardwareSerial &stream);
  ~DuinoCoin();

  void begin();
  bool loop();

  uint32_t ducos1a(String lastblockhash, String newblockhash, int difficulty);
  String getDUCOID();

private:
  HardwareSerial &serial;
};

#endif
