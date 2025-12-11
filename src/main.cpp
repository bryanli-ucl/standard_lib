#include <arduino/hw.hpp>
#include <literals>

using namespace msd::literals;
namespace fw = firmware;

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
    using LED1 = fw::pins<13, fw::PinMode::OUTPUT>;
    using LED2 = fw::pins<12, fw::PinMode::OUTPUT>;

    // using fw::serial;
    // serial.begin(9600);
    // serial.println(sizeof(LED1));

    using fw::logger;
    auto& log = logger::instance();
    log.init();
    log.info("Hi");
    log.info("Hi%d", 123);
    log.info("Hi%s", "abc");
    log.info("7355608");

    LED1::dwrite(true);
    LED2::dwrite(true);
}

/*
void entry() {

    using fw::serial;

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
        serial.printlharacteristic polynomial and factorisationn(length.v);
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

    // using LED = fw::pins<13, firmware::PinMode::OUTPUT>;
    // LED::dwrite(!(LED::dread()));

    while (true) {
        // 3526 -> 3838 = 128 bytes
        // fw::wait(1_s);
        // 3982 -> 4082 = 100 bytes
        // LED::dwrite(!LED ::dread());
        // fw::wait(1_s);
    }
}
*/