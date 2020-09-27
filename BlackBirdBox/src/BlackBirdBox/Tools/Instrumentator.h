#pragma once

#include "rapidjson/document.h"
#include <chrono>
#include <string>
#include <thread>

namespace BlackBirdBox {

using FloatingPointMicroseconds = std::chrono::duration<double, std::micro>;

struct ProfileResult {
    std::string Name;

    FloatingPointMicroseconds Start;
    std::chrono::microseconds ElapsedTime;
    std::thread::id ThreadID;
};

struct InstrumentationSession {
    std::string FilePath;
};

class Instrumentor {
public:
    Instrumentor();

    void BeginSession(const std::string& filepath = "results.json");
    void EndSession();

    void WriteProfile(const ProfileResult& result);

    static Instrumentor& Get()
    {
        static Instrumentor instance;
        return instance;
    }

private:
    void WriteHeader();

    // Note: you must already own lock on m_Mutex before
    // calling InternalEndSession()
    void InternalEndSession();

private:
    InstrumentationSession* current_session_;
    rapidjson::Document output_json_;
};
}
