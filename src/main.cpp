#include <array.hpp>
#include <queue.hpp>
#include <vector.hpp>

#include <assert.h>

#ifdef ARDUINO
#include <Arduino.h>
#include <avr-memory.hpp>


msd::array<int, 2> time;

msd::vector<int> vec;

msd::queue<int> q;

void setup() {

    pinMode(13, OUTPUT);

    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);

    q.push_front(1);
    q.push_front(2);
    q.push_front(3);

    q.pop_front();
}

void loop() {
    if (vec.size() != 3) {
        digitalWrite(13, HIGH);
        while (1);
    }
    volatile auto p = new int[11];
    p[5]            = 40;
    p[10]           = 80;


    digitalWrite(13, HIGH);
    delay(p[5]);
    digitalWrite(13, LOW);
    delay(p[10]);
    delete[] p;
}

#endif
