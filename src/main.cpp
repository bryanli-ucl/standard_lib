#ifdef ARDUINO
#include <Arduino.h>
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "msd/array.hpp"
#include "msd/queue.hpp"

void setup() {
    msd::array<int, 10> arr;
    arr.fill(0);
}

void loop() {}
