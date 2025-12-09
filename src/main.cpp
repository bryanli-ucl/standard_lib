#include <Arduino.h>

#include <pair>
#include <vector>


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

    msd::vector<int> vec = { 1, 2, 3 };
    Serial.println(vec.size());
    Serial.println(vec.capacity());
    Serial.println(vec.at(0));
    Serial.println(vec.at(1));
    Serial.println(vec.at(2));

    msd::vector<float> vec2 = { 1, 2, 3 };
    Serial.println(vec2.size());
    Serial.println(vec2.capacity());
    Serial.println(vec2.at(0));
    Serial.println(vec2.at(1));
    Serial.println(vec2.at(2));
}

void loop() {}