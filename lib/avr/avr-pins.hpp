#pragma once

#include <Arduino.h>

#include <type_traits.hpp>

#include "avr-def.hpp"
#include "avr-log.hpp"

namespace avr {
enum class PinMode {
    Input       = INPUT,
    InputPullUp = INPUT_PULLUP,
    Output      = OUTPUT,
};
class pins {
    private:
    uint8_t m_pin;
    PinMode m_pinmode;

    public:
    pins(uint8_t pin, PinMode pm) noexcept : m_pin(pin), m_pinmode(pm) {
        pinMode(m_pin, static_cast<uint8_t>(m_pinmode));
        auto& log = logger::instance();
        log.debug("set pin (%d) to %s", m_pin, pinmode_str());
    }
    virtual ~pins() = default;

    pins(const pins& other) : m_pin(other.m_pin), m_pinmode(other.m_pinmode) {}

    const char* pinmode_str() {
        switch (m_pinmode) {
        case PinMode::Input: return "INPUT";
        case PinMode::InputPullUp: return "INPUT_PULLUP";
        case PinMode::Output: return "OUTPUT";
        default: return "UNKNOW";
        }
    }

    // write pwm to pin
    template <typename T>
    typename msd::enable_if<msd::is_integral<T>::value>::type
    writea(T pwm) { analogWrite(m_pin, static_cast<uint8_t>(constrain(pwm, 0, 255))); }

    // write digital to pin
    template <typename T>
    typename msd::enable_if<msd::is_integral<T>::value && !msd::is_bool<T>::value>::type
    writed(T s) { digitalWrite(m_pin, s > 0 ? HIGH : LOW); }
    template <typename T>
    typename msd::enable_if<msd::is_bool<T>::value>::type
    writed(T s) { digitalWrite(m_pin, s ? HIGH : LOW); }

    // Read analog from pin
    uint8_t reada() { return analogRead(m_pin); }

    // Read digital from pin
    bool readd() { return digitalRead(m_pin); }
};
} // namespace avr
