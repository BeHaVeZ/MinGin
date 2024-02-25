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
        const int fps = Timer::GetInstance().GetFPS();
        TextObject::SetText("FPS: " + std::to_string(fps));
        TextObject::Update();
    }
}