#pragma once
#include "Component.h"

namespace dae
{
    class BoxCollider : public Component
    {
    public:
        BoxCollider(float width, float height, GameObject* owner);

        void Update() override;
        void Render() const override;

        bool IsCollidingWith(const BoxCollider& other) const;
        void ResolveCollision(BoxCollider& other) const;

    private:
        float m_Width;
        float m_Height;
        GameObject* m_Owner;
    };
}
