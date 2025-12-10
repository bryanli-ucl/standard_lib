#include "serial.hpp"

#include <Arduino.h>
#include <stdint.h>

namespace firmware {

void SerialPort::begin(unsigned long baud) { Serial.begin(baud); }
void SerialPort::end() { Serial.end(); }
int SerialPort::available() { return Serial.available(); }

int SerialPort::read() { return Serial.read(); }
int SerialPort::peek() { return Serial.peek(); }
void SerialPort::flush() { Serial.flush(); }

size_t SerialPort::write(uint8_t byte) { return Serial.write(byte); }
size_t SerialPort::write(const uint8_t* buffer, size_t size) { return Serial.write(buffer, size); }

size_t SerialPort::print(const char* str) { return Serial.print(str); }
size_t SerialPort::print(int n) { return Serial.print(n); }
size_t SerialPort::print(unsigned int n) { return Serial.print(n); }
size_t SerialPort::print(long n) { return Serial.print(n); }
size_t SerialPort::print(unsigned long n) { return Serial.print(n); }
size_t SerialPort::print(double n, int digits) { return Serial.print(n, digits); }
size_t SerialPort::println(const char* str) { return Serial.println(str); }
size_t SerialPort::println(int n) { return Serial.println(n); }
size_t SerialPort::println(unsigned int n) { return Serial.println(n); }
size_t SerialPort::println(long n) { return Serial.println(n); }
size_t SerialPort::println(unsigned long n) { return Serial.println(n); }
size_t SerialPort::println(double n, int digits) { return Serial.println(n, digits); }
size_t SerialPort::println() { return Serial.println(); }

SerialPort serial;
} // namespace firmware
