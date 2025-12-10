#pragma once

#ifdef __AVR__
#include <avr-log.hpp>
#include <avr-pins.hpp>
#endif

namespace firmware {
namespace step_motor {

enum class State {
    AP = 1,
    AN = 3,
    BP = 2,
    BN = 4,
};

class StepMotorStateController {
    private:
    step_motor::State m_state;

    public:
    StepMotorStateController() noexcept : m_state(State::AP) {}
    virtual ~StepMotorStateController() noexcept = default;

    step_motor::State get_state() const { return m_state; }
    void set_state(step_motor::State state) { m_state = state; }

    void forward() {
        switch (m_state) {
        case State::AP: m_state = State::BP; break;
        case State::BP: m_state = State::AN; break;
        case State::AN: m_state = State::BN; break;
        case State::BN: m_state = State::AP; break;
        default: break;
        }
    }

    void backward() {
        switch (m_state) {
        case State::BN: m_state = State::AN; break;
        case State::AN: m_state = State::BP; break;
        case State::BP: m_state = State::AP; break;
        case State::AP: m_state = State::BN; break;
        default: break;
        }
    }
};

class StepMotor {
    private:
    StepMotorStateController m_smsc;

    avr::pins m_ap;
    avr::pins m_an;
    avr::pins m_bp;
    avr::pins m_bn;

    uint8_t m_power;
    bool m_is_enable_power;

    uint16_t m_delay;

    public:
    StepMotor(const avr::pins& ap, const avr::pins& an, const avr::pins& bp, const avr::pins& bn) noexcept
    : m_ap(ap), m_an(an), m_bp(bp), m_bn(bn), m_is_enable_power(false) {}
    virtual ~StepMotor() noexcept = default;

    StepMotor(const StepMotor& other)
    : m_ap(other.m_ap), m_an(other.m_an), m_bp(other.m_bp), m_bn(other.m_bn), m_is_enable_power(other.m_is_enable_power) {}

    void enable_power() { m_is_enable_power = true; }
    void disable_power() { m_is_enable_power = false; }

    void set_power(uint8_t power) { m_power = power; }
    uint8_t get_power() { return m_power; }

    void set_speed(uint32_t spm) { m_delay = 60000 / spm; }

    void step(int16_t s) {
        auto& log = avr::logger::instance();
        log.debug("step motor rotating %d steps begin", s);
        if (s == 0) return;
        for (uint16_t i = 0; i < static_cast<uint16_t>(abs(s)); i++) {
            state_close();

            if (s > 0) {
                m_smsc.forward();
            } else {
                m_smsc.backward();
            }

            state_open();

            delay(m_delay);
        }
        log.debug("step motor rotating %d steps eng", s);
    }

    void state_close() noexcept {
        switch (m_smsc.get_state()) {
        case State::AP: m_is_enable_power ? m_ap.writea(0) : m_ap.writed(false); break;
        case State::AN: m_is_enable_power ? m_an.writea(0) : m_an.writed(false); break;
        case State::BP: m_is_enable_power ? m_bp.writea(0) : m_bp.writed(false); break;
        case State::BN: m_is_enable_power ? m_bn.writea(0) : m_bn.writed(false); break;
        }
    }

    void state_open() noexcept {
        switch (m_smsc.get_state()) {
        case State::AP: m_is_enable_power ? m_ap.writea(m_power) : m_ap.writed(true); break;
        case State::AN: m_is_enable_power ? m_an.writea(m_power) : m_an.writed(true); break;
        case State::BP: m_is_enable_power ? m_bp.writea(m_power) : m_bp.writed(true); break;
        case State::BN: m_is_enable_power ? m_bn.writea(m_power) : m_bn.writed(true); break;
        }
    }
};
} // namespace step_motor
} // namespace firmware
