#pragma once

#include <stddef.h>
#include <stdint.h>

#include <singleton>
#include <type_traits>

namespace firmware {

enum class PinMode {
    INPUT        = 0x0,
    OUTPUT       = 0x1,
    INPUT_PULLUP = 0x2,
};

namespace __details {
void __pin_mode(uint8_t, uint8_t) noexcept;

void __digital_write(uint8_t, bool) noexcept;
bool __digital_read(uint8_t) noexcept;

void __analog_write(uint8_t, uint8_t) noexcept;
uint16_t __analog_read(uint8_t) noexcept;

} // namespace __details


template <uint8_t PIN, PinMode MODE>
class pins : public msd::singleton<pins<PIN, MODE>> {

    friend class msd::singleton<pins<PIN, MODE>>;

    private:
    static constexpr uint8_t m_pin  = PIN;
    static constexpr PinMode m_mode = MODE;

    static bool m_is_initialized;

    pins() noexcept {
        if (!m_is_initialized) {
            __details::__pin_mode(m_pin, static_cast<uint8_t>(m_mode));
            m_is_initialized = true;
        }
    }
    ~pins() noexcept = default;

    pins(const pins&) noexcept            = delete;
    pins& operator=(const pins&) noexcept = delete;
    pins(pins&&) noexcept                 = delete;
    pins& operator=(pins&&) noexcept      = delete;

    public:
    static bool dread() { return __details::__digital_read(m_pin); }
    static uint16_t aread() { return __details::__analog_read(m_pin); }

    static void dwrite(bool val) { return __details::__digital_write(m_pin, val); }
    static void awrite(uint8_t val) { return __details::__analog_write(m_pin, val); }
};


template <uint8_t PIN, PinMode MODE>
bool pins<PIN, MODE>::m_is_initialized = false;

} // namespace firmware
