#pragma once

#include <chrono>

namespace BlackBirdBox {

class InstrumentationTimer {
public:
    InstrumentationTimer(const char* name)
        : m_Name(name)
        , m_Stopped(false)
    {
        m_StartTimepoint = std::chrono::steady_clock::now();
    }

    ~InstrumentationTimer()
    {
        if (!m_Stopped)
            Stop();
    }

    void Stop();

private:
    const char* m_Name;
    std::chrono::time_point<std::chrono::steady_clock> m_StartTimepoint;
    bool m_Stopped;
};

}
