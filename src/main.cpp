#include <Arduino.h>

#include <pair>

void setup() {

    Serial.begin(9600);
    delay(300);

    auto a      = msd::make_pair(10, 5.5);
    auto [x, y] = a;

    // Serial.println(F("VAR"));
    Serial.println(x);
    Serial.println(y);

    auto b        = msd::make_pair(50, '2');
    auto [x2, y2] = b;
    Serial.println(x2);
    Serial.println(y2);
}

void loop() {}