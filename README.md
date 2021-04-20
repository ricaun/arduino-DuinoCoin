# arduino-DuinoCoin

Code for Arduino boards v2.4

© Duino-Coin Community 2019-2021

Distributed under MIT License

* https://github.com/revoxhere/duino-coin

## Library Dependency

The library uses the [`ArduinoUniqueID`][0] library.

# Installation

* Install the library by [Using the Library Manager](https://www.arduino.cc/en/Guide/Libraries#toc3)
* **OR** by [Importing the .zip library](https://www.arduino.cc/en/Guide/Libraries#toc4) using either the [master](https://github.com/ricaun/arduino-DuinoCoin/archive/1.0.0.zip) or one of the [releases](https://github.com/ricaun/arduino-DuinoCoin/releases) ZIP files.

## Examples

The library comes with [examples](examples). After installing the library you need to restart the Arduino IDE before they can be found under **File > Examples > arduino-DuinoCoin**.

---

# Reference

## Include Library

```c
#include <DuinoCoin.h>
```

```c
DuinoCoin DuinoCoin(Serial);
```

## Method:begin

```c
void setup() {
  DuinoCoin.begin();
}
```

## Method:loop

```c
void loop() {
  if (DuinoCoin.loop())
  {
      // Work Done
  }
}
```

---

Do you like this library? Please [star this project on GitHub](https://github.com/ricaun/arduino-DuinoCoin/stargazers)!

[0]: https://github.com/ricaun/ArduinoUniqueID