#pragma once

#include <Arduino.h>

#include <singleton.hpp>
#include <string.hpp>

namespace msd {
class logger : public msd::singleton<logger> {

    friend class msd::singleton<logger>;

    enum class Level {
        DEBUG   = 4,
        INFO    = 3,
        WARN    = 2,
        ERROR   = 1,
        FATAL   = 0,
        DISABLE = -1,
    };

    struct BaudRate {
        static constexpr uint32_t BAUD_300    = 300;
        static constexpr uint32_t BAUD_1200   = 1200;
        static constexpr uint32_t BAUD_2400   = 2400;
        static constexpr uint32_t BAUD_4800   = 4800;
        static constexpr uint32_t BAUD_9600   = 9600;
        static constexpr uint32_t BAUD_19200  = 19200;
        static constexpr uint32_t BAUD_38400  = 38400;
        static constexpr uint32_t BAUD_57600  = 57600;
        static constexpr uint32_t BAUD_115200 = 115200;
    };

    private:
    uint32_t m_baud_rate;
    Level m_level;
    bool m_is_initialized;

    virtual ~logger() noexcept = default;
    logger() noexcept
    : m_baud_rate(BaudRate::BAUD_9600),
      m_level(Level::INFO),
      m_is_initialized(false) {}

    logger(const logger&) = delete;
    logger(logger&&)      = delete;

    logger& operator=(const logger&) = delete;
    logger& operator=(logger&&)      = delete;

    public:
    void init(uint32_t br = BaudRate::BAUD_9600, Level level = Level::INFO) {
        if (m_is_initialized == true) return;

        m_baud_rate = br;
        m_level     = level;

        Serial.begin(br);
        if (Serial == true) delay(10);

        m_is_initialized = true;

        info(F("========================="));
        info(F("Serial Logger initialized"));
        info("Baud Rate: %d", m_baud_rate);
        info("Level: %s", level_to_string(level));
        info(F("========================="));
    }

    // stats
    bool is_initialized() const noexcept { return m_is_initialized; }
    Level get_level() const noexcept { return m_level; }

    // control
    void set_level(Level level) noexcept { m_level = level; }

    // log
    void log(Level level, const char* fmt) { log_str(level, fmt); }
    void log(Level level, const __FlashStringHelper* fmt) {
        char buf[64];
        strncpy_P(buf, (const char*)fmt, sizeof(buf) - 1);
        buf[sizeof(buf) - 1] = '\0';
        log_str(level, buf);
    }
    template <typename... Args>
    void log(Level level, const char* fmt, Args... args) {
        char buf[64] = {};
        snprintf(buf, sizeof(buf), fmt, args...);
        log_str(level, buf);
    }

    template <typename... Args>
    void debug(const char* fmt, Args... args) { log(Level::DEBUG, fmt, args...); }
    template <typename... Args>
    void info(const char* fmt, Args... args) { log(Level::INFO, fmt, args...); }
    template <typename... Args>
    void warn(const char* fmt, Args... args) { log(Level::WARN, fmt, args...); }
    template <typename... Args>
    void error(const char* fmt, Args... args) { log(Level::ERROR, fmt, args...); }
    template <typename... Args>
    void fatal(const char* fmt, Args... args) { log(Level::FATAL, fmt, args...); }

    void debug(const __FlashStringHelper* fmt) { log(Level::DEBUG, fmt); }
    void info(const __FlashStringHelper* fmt) { log(Level::INFO, fmt); }
    void warn(const __FlashStringHelper* fmt) { log(Level::WARN, fmt); }
    void error(const __FlashStringHelper* fmt) { log(Level::ERROR, fmt); }
    void fatal(const __FlashStringHelper* fmt) { log(Level::FATAL, fmt); }

    private:
    void log_str(Level level, const char* str) {
        if (m_is_initialized == false) return;
        if (m_level < level) return;
        // print time stamp
        print_time_stamp();
        // print level
        print_level(level);
        // print message
        Serial.println(str);
    }
    static void print_level(Level level) {
        Serial.print('[');
        Serial.print(level_to_string(level));
        Serial.print(']');
    }
    static void print_time_stamp() {
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
    static const char* level_to_string(Level level) {
        switch (level) {
        case Level::DEBUG: return "DEBUG";
        case Level::INFO: return " INFO";
        case Level::WARN: return " WARN";
        case Level::ERROR: return "ERROR";
        case Level::FATAL: return "FATAL";
        default: return "UNKNOW";
        }
    }
};
} // namespace msd
