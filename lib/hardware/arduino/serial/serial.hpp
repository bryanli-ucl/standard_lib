#pragma once

#include <stddef.h>
#include <stdint.h>

namespace firmware {

class SerialPort {
    public:
    void begin(unsigned long baud);
    void end();

    int available();
    int read();
    int peek();
    void flush();

    size_t write(uint8_t byte);
    size_t write(const uint8_t* buffer, size_t size);

    size_t print(const char* str);
    size_t print(int n);
    size_t print(unsigned int n);
    size_t print(long n);
    size_t print(unsigned long n);
    size_t print(double n, int digits = 2);

    size_t println(const char* str);
    size_t println(int n);
    size_t println(unsigned int n);
    size_t println(long n);
    size_t println(unsigned long n);
    size_t println(double n, int digits = 2);
    size_t println();
};

extern SerialPort serial;

} // namespace firmware
