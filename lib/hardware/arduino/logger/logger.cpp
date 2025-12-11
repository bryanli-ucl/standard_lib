#include "logger.hpp"

#include <Arduino.h>

namespace firmware {

namespace __details {
template <> struct is_flash_string<const __FlashStringHelper*> : msd::true_type {};
} // namespace __details

logger::logger() noexcept
: m_baud_rate(BaudRate::BAUD_9600),
  m_level(Level::INFO),
  m_is_initialized(false) {}

void logger::init(uint32_t br, Level level) {
    if (m_is_initialized == true) return;

    m_baud_rate = br;
    m_level     = level;

    Serial.begin(br);
    delay(50);

    m_is_initialized = true;

    info(F("========================="));
    info(F("Serial Logger initialized"));
    info("Baud Rate: %lu", (m_baud_rate));
    info("Level: %s", level_to_string(level));
    info(F("========================="));
}

void logger::log_impl(Level level, const char* str) {
    log_str(level, str);
}

void logger::log_impl(Level level, const void* flash_str) {
    const __FlashStringHelper* fsh = reinterpret_cast<const __FlashStringHelper*>(flash_str);
    char buf[64];
    strncpy_P(buf, (const char*)fsh, sizeof(buf) - 1);
    buf[sizeof(buf) - 1] = '\0';
    log_str(level, buf);
}

void logger::log_str(Level level, const char* str) {
    if (m_is_initialized == false) return;
    if (m_level < level) return;

    // print time stamp
    print_time_stamp();
    // print level
    print_level(level);
    // print message
    Serial.print(str);
    Serial.print('\n');
}

void logger::print_level(Level level) {
    Serial.print('[');
    Serial.print(level_to_string(level));
    Serial.print(']');
}

void logger::print_time_stamp() {
    // [s.ms]
    unsigned long time = millis();
    unsigned long ms   = time % 1000;
    unsigned long s    = time / 1000;
    Serial.print('[');
    Serial.print(s);
    Serial.print('.');
    if (ms < 100) Serial.print("0");
    if (ms < 10) Serial.print("0");
    Serial.print(ms);
    Serial.print(']');
}

const char* logger::level_to_string(Level level) {
    switch (level) {
    case Level::DEBUG: return "DEBUG";
    case Level::INFO: return " INFO";
    case Level::WARN: return " WARN";
    case Level::ERROR: return "ERROR";
    case Level::FATAL: return "FATAL";
    default: return "UNKNOW";
    }
}

} // namespace firmware