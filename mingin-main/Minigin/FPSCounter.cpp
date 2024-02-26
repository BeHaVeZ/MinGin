#include "FPSCounter.h"
#include <iostream>
#include "Timer.h"


namespace dae
{
    FPSCounter::FPSCounter(const std::shared_ptr<Font>& font)
        : TextObject("FPS: 0", font)
    {
    }

    void FPSCounter::Update()
    {
        const float fps = Timer::GetInstance().GetFPS();
        //std::format("{:.1f}",m_count / m_delay));
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(1) << fps;

        TextObject::SetText("FPS: " + oss.str());
        TextObject::Update();
    }
}