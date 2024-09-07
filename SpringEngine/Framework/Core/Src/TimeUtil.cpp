#include "Precompiled.h"
#include "TimeUtil.h"

float SpringEngine::Core::TimeUtil::GetTime()
{
    static const auto startTime = std::chrono::high_resolution_clock::now();
    const auto currentTime = std::chrono::high_resolution_clock::now();
    const auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime).count();
    return milliseconds / 1000.0f;
}

float SpringEngine::Core::TimeUtil::GetDeltaTime()
{
    static auto lastCallTime = std::chrono::high_resolution_clock::now();
    const auto currentTime = std::chrono::high_resolution_clock::now();
    const auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastCallTime).count();
    lastCallTime = currentTime;
    return milliseconds / 1000.0f;
}
