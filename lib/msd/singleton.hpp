#pragma once

namespace msd {

enum class LiftTime {
    NORMAL,  // normal destory (auto clean)
    LEAK,    // allow memory leaking
    PHOENIX, // allow re-create after destory
};

template <typename T, LiftTime LiftTimeT = LiftTime::NORMAL>
class singleton {

    using data_t   = T;
    using data_ptr = T*;
    using data_ref = T&;

    private:
    protected:
    singleton()          = default;
    virtual ~singleton() = default;

    singleton(const singleton&)            = delete;
    singleton(singleton&&)                 = delete;
    singleton& operator=(const singleton&) = delete;
    singleton& operator=(singleton&&)      = delete;

    public:
    static T& instance() {
        if (LiftTimeT == LiftTime::LEAK) {
            static data_ptr m_instance_ptr = new data_t();
            return *m_instance_ptr;
        } else {
            static data_t instance;
            return instance;
        }
    }

    static void destory() {
        if (LiftTimeT == LiftTime::PHOENIX) {
            return;
        } else if (LiftTimeT == LiftTime::LEAK) {
            return;
        } else {
            return;
        }
    }
};
} // namespace msd
