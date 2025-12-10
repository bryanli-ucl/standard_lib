#include "pins.hpp"

#include <Arduino.h>

namespace firmware {

void __details::__pin_mode(uint8_t p, uint8_t m) noexcept {
    pinMode(p, m);
}

void __details::__digital_write(uint8_t p, bool v) noexcept {
    digitalWrite(p, static_cast<uint8_t>(v));
}

bool __details::__digital_read(uint8_t p) noexcept {
    return static_cast<bool>(digitalRead(p));
}

void __details::__analog_write(uint8_t p, uint8_t v) noexcept {
    analogWrite(p, static_cast<uint16_t>(v));
}

uint16_t __details::__analog_read(uint8_t p) noexcept {
    return static_cast<uint16_t>(analogRead(p));
}


} // namespace firmware
