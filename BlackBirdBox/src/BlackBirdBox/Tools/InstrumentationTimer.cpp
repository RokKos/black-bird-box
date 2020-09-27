#include "bbbpch.h"
#include "InstrumentationTimer.h"

#include "Instrumentator.h"

namespace BlackBirdBox {

void InstrumentationTimer::Stop()
{
    auto endTimepoint = std::chrono::steady_clock::now();
    auto highResStart = FloatingPointMicroseconds{ m_StartTimepoint.time_since_epoch() };
    auto elapsedTime = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch()
        - std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch();

    Instrumentor::Get().WriteProfile({ m_Name, highResStart, elapsedTime, std::this_thread::get_id() });

    m_Stopped = true;
}

}
