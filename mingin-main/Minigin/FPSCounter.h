#pragma once

#include "TextObject.h"
#include "Timer.h"

namespace dae
{
    class FPSCounter : public TextObject
    {
    public:
        FPSCounter(const std::shared_ptr<Font>& font);

        void Update() override;
    };
}