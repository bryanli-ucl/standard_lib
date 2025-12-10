#include "wiring.hpp"

#include <Arduino.h>

namespace firmware {

void wait(msd::literals::Seconds s) {
    using namespace msd::literals;

    if (s < 20_ms) {
        delayMicroseconds(static_cast<unsigned int>(s.v * 1000000));
    } else {
        delay(static_cast<unsigned int>(s.v * 1000.));
    }
}

} // namespace firmware
