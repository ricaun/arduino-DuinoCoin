//////////////////////////////////////////////////////////
//  _____        _                    _____      _
// |  __ \      (_)                  / ____|    (_)
// | |  | |_   _ _ _ __   ___ ______| |     ___  _ _ __
// | |  | | | | | | '_ \ / _ \______| |    / _ \| | '_ \ 
// | |__| | |_| | | | | | (_) |     | |___| (_) | | | | |
// |_____/ \__,_|_|_| |_|\___/       \_____\___/|_|_| |_|
//  Code for Arduino boards v2.4
//  © Duino-Coin Community 2019-2021
//  Distributed under MIT License
//////////////////////////////////////////////////////////
//  https://github.com/revoxhere/duino-coin - GitHub
//  https://duinocoin.com - Official Website
//  https://discord.gg/k48Ht5y - Discord
//////////////////////////////////////////////////////////
//  If you don't know what to do, visit official website
//  and navigate to Getting Started page. Happy mining!
//////////////////////////////////////////////////////////

#include <DuinoCoin.h>

DuinoCoin DuinoCoin(Serial);

void setup() {
  pinMode(LED_BUILTIN, OUTPUT); // Prepare built-in led pin as output
  DuinoCoin.begin();
}

void loop() {
  if (DuinoCoin.loop())
  {
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    delay(25); // Wait a bit
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  }
}
