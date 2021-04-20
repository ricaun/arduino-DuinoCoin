#ifndef Ducos1a_h
#define Ducos1a_h

#include <Arduino.h>

#if defined(ARDUINO_ARCH_AVR)
#include "avr/Ducos1a_avr.h"
#elif defined(ARDUINO_ARCH_ESP8266)
#include "esp/Ducos1a_esp.h"
#else
#endif

extern Ducos1a_t Ducos1a;

#endif