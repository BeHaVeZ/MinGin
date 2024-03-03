#include "pch.h"

namespace dae
{
    Timer::Timer()
        : m_LastTime(std::chrono::high_resolution_clock::now()), m_DeltaTime(0.0f), m_FrameCount(0), m_FPS(0)
    {}

    void Timer::Start()
    {
        m_LastTime = std::chrono::high_resolution_clock::now();
        m_DeltaTime = 0.0f;
        m_FrameCount = 0;
        m_FPS = 0;
    }

    void Timer::Update()
    {
        const auto currentTime = std::chrono::high_resolution_clock::now();
        const auto duration = std::chrono::duration<float>(currentTime - m_LastTime);
        m_DeltaTime = duration.count();
        m_LastTime = currentTime;

        ++m_FrameCount;

        if (m_DeltaTime >= 1.0f)
        {
            m_FPS = m_FrameCount;
            m_FrameCount = 0;
        }
    }

    float Timer::GetDeltaTime() const
    {
        return m_DeltaTime;
    }

    float Timer::GetFPS() const
    {
        return 1.f / m_DeltaTime;
    }
}