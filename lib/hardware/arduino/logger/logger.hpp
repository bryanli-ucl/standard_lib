#pragma once

#include <stddef.h>

#include <singleton>
#include <type_traits>

#include "../serial/serial.hpp"

namespace firmware {


namespace __details {
extern "C" int snprintf(char* str, size_t size, const char* format, ...);

template <typename T> struct is_flash_string : msd::false_type {};
} // namespace __details

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

class logger : public msd::singleton<logger> {
    friend class msd::singleton<logger>;


    private:
    uint32_t m_baud_rate;
    Level m_level;
    bool m_is_initialized;

    logger() noexcept;
    ~logger() noexcept               = default;
    logger(const logger&)            = delete;
    logger(logger&&)                 = delete;
    logger& operator=(const logger&) = delete;
    logger& operator=(logger&&)      = delete;

    public:
    void init(uint32_t br = BaudRate::BAUD_115200, Level level = Level::INFO);

    // stats
    bool is_initialized() const noexcept { return m_is_initialized; }
    Level get_level() const noexcept { return m_level; }

    // control
    void set_level(Level level) noexcept { m_level = level; }

    // log
    template <typename T>
    void log(Level level, T str) {
        log_impl(level, str);
    }

    template <typename... Args>
    void log(Level level, const char* fmt, Args... args) {
        char buf[64] = {};
        __details::snprintf(buf, sizeof(buf), fmt, args...);
        log_str(level, buf);
    }

    template <typename T>
    void debug(T str) { log(Level::DEBUG, str); }
    template <typename T>
    void info(T str) { log(Level::INFO, str); }
    template <typename T>
    void warn(T str) { log(Level::WARN, str); }
    template <typename T>
    void error(T str) { log(Level::ERROR, str); }
    template <typename T>
    void fatal(T str) { log(Level::FATAL, str); }

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


    private:
    void log_str(Level level, const char* str);

    void log_impl(Level level, const char* str);
    void log_impl(Level level, const void* flash_str);

    static void print_level(Level level);
    static void print_time_stamp();
    static const char* level_to_string(Level level);
};
} // namespace firmware
