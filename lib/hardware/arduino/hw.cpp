#include "hw.hpp"

#include <Arduino.h>

// user defined program entry
void entry();

// must be done in Arduino
void setup() {}
void loop() {
    entry();
    while (1);
}