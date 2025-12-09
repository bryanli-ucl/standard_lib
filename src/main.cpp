#include <Arduino.h>

// #include <pair>
// #include <vector>

#include <literals>

/*
Memory region         Used Size  Region Size  %age Used
            text:        1586 B       128 KB      1.21%
            data:         344 B      65440 B      0.53%
          eeprom:          0 GB        64 KB      0.00%
            fuse:          0 GB          3 B      0.00%
            lock:          0 GB         1 KB      0.00%
       signature:          0 GB         1 KB      0.00%
 user_signatures:          0 GB         1 KB      0.00%

RAM:   [=         ]   9.0% (used 184 bytes from 2048 bytes)
Flash: [          ]   4.9% (used 1586 bytes from 32256 bytes)
*/
void setup() {
    Serial.begin(9600);
    delay(300);

    // msd::vector<int> vec = { 1, 2, 3 };
    // Serial.println(vec.size());
    // Serial.println(vec.capacity());
    // Serial.println(vec.at(0));
    // Serial.println(vec.at(1));
    // Serial.println(vec.at(2));

    // RAM:   [=         ]  11.1% (used 228 bytes from 2048 bytes)
    // Flash: [=         ]  10.2% (used 3280 bytes from 32256 bytes)
    {
        using namespace msd::literals;

        auto time   = 1_s;
        auto mass   = 1_kg;
        auto length = 1_m;

        auto unknow = 1_s / 1_kg * 1_m * 1_m / 1_s;

        auto newton1   = 1_g * 1_km / (1_s * 1_s);
        auto newton2   = 1_N;
        auto [t, m, l] = newton1.dim();

        Serial.println("===========================");
        Serial.println(time.v);
        Serial.println(mass.v);
        Serial.println(length.v);
        Serial.println(newton1.v, 20);
        Serial.println(newton2.v, 20);
        Serial.print(t);
        Serial.print(m);
        Serial.println(l);
    }

    // RAM:   [=         ]  11.1% (used 228 bytes from 2048 bytes)
    // Flash: [=         ]  10.2% (used 3280 bytes from 32256 bytes)
    {
        double time   = 1;
        double mass   = 1;
        double length = 1;
        double newton = 0.001;
        Serial.println("===========================");
        Serial.println(time);
        Serial.println(mass);
        Serial.println(length);
        Serial.println(newton, 20);
    }
}

void loop() {}