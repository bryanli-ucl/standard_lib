#ifdef ARDUINO
#include <Arduino.h>
#endif

#include <avr-memory.hpp>

#include <array.hpp>
msd::array<int, 2> time;

void setup() {

    pinMode(13, OUTPUT);


    // Serial.begin(115200);


    // time[0] = 1000;
    // time[1] = 500;
    // Q.push_back(1000);
    // Q.push_front(100);
    // msd::queue<int> Q;
    // Q.push_back(1000);
    // Q.push_back(500);
}

void loop() {
    volatile auto p = new int[11];
    p[5]            = 40;
    p[10]           = 80;

    digitalWrite(13, HIGH);
    delay(p[5]);
    digitalWrite(13, LOW);
    delay(p[10]);
}
