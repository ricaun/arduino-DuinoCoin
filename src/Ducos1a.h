#ifndef Ducos1a_h
#define Ducos1a_h

#include <Arduino.h>

#if defined(ARDUINO_ARCH_AVR)
//#include "avr/Ducos1a_avr.h"
//#include "hash/Ducos1a_hash.h"
#include "sha1/Ducos1a_sha1.h"
#elif defined(ARDUINO_ARCH_ESP8266)
#include "esp/Ducos1a_esp.h"
#elif defined(ARDUINO_ARCH_ESP32)
#include "esp32/Ducos1a_esp32.h"
#else
#include "hash/Ducos1a_hash.h"
#endif

extern Ducos1a_t Ducos1a;

#endif