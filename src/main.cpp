#include <arduino/hw.hpp>
#include <literals>

using namespace msd::literals;

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
void entry() {

    using firmware::pins;
    using firmware::serial, firmware::wait;

    serial.begin(9600);

    // RAM:   [=         ]  11.1% (used 228 bytes from 2048 bytes)
    // Flash: [=         ]  10.2% (used 3280 bytes from 32256 bytes)
    {
        using namespace msd::literals;

        auto time   = 1_s;
        auto mass   = 1_kg;
        auto length = 1_m;

        auto newton1   = 1_g * 1_km / (1_s * 1_s);
        auto newton2   = 1_N;
        auto [t, m, l] = newton1.dim();

        serial.println("===========================");
        serial.println(time.v);
        serial.println(mass.v);
        serial.println(length.v);
        serial.println(newton1.v, 20);
        serial.println(newton2.v, 20);
        serial.print(t);
        serial.print(m);
        serial.println(l);
    }

    // RAM:   [=         ]  11.1% (used 228 bytes from 2048 bytes)
    // Flash: [=         ]  10.2% (used 3280 bytes from 32256 bytes)
    {
        double time   = 1;
        double mass   = 1;
        double length = 1;
        double newton = 0.001;
        serial.println("===========================");
        serial.println(time);
        serial.println(mass);
        serial.println(length);
        serial.println(newton, 20);
    }

    using LED = pins<13, firmware::PinMode::OUTPUT>;

    while (true) {
        // 3764 -> 3982 = 128 bytes
        LED::dwrite(!LED ::dread());
        wait(1_s);
    }
}
