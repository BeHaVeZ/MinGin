#pragma once
#include "Component.h"

namespace dae
{
    class BoxTrigger : public Component
    {
    public:
        BoxTrigger(float width, float height, GameObject* owner);

        void Update() override;
        void Render() const override;

        bool IsOverlappingWith(const BoxTrigger& other) const;
        void OnTriggerEnter(BoxTrigger& other) const;
        void OnTriggerExit(BoxTrigger& other) const;

    private:
        float m_Width;
        float m_Height;
        GameObject* m_Owner;
    };
}
