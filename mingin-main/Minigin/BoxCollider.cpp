#include "pch.h"
#include "BoxCollider.h"
#include "GameObject.h"
#include "Renderer.h"

dae::BoxCollider::BoxCollider(float width, float height, GameObject* owner)
    : m_Width(width), m_Height(height), m_Owner(owner)
{
}

void dae::BoxCollider::Update()
{
}

void dae::BoxCollider::Render() const
{
    //const auto& pos = m_Owner->GetTransform().GetPosition();
    //SDL_Color color = { 255, 0, 0, 255 };
    //dae::Renderer::GetInstance().DrawRect(pos.x, pos.y, m_Width, m_Height, color);
}

bool dae::BoxCollider::IsCollidingWith(const BoxCollider& other) const
{
    const auto& pos1 = m_Owner->GetTransform().GetPosition();
    const auto& pos2 = other.m_Owner->GetTransform().GetPosition();

    bool xOverlap = pos1.x < pos2.x + other.m_Width && pos1.x + m_Width > pos2.x;
    bool yOverlap = pos1.y < pos2.y + other.m_Height && pos1.y + m_Height > pos2.y;

    return xOverlap && yOverlap;
}

void dae::BoxCollider::ResolveCollision(BoxCollider& other) const
{
    if (m_Owner->IsStatic() && other.m_Owner->IsStatic())
    {
        return;
    }
    GameObject* dynamic = m_Owner->IsStatic() ? other.m_Owner : m_Owner;
    GameObject* staticObj = m_Owner->IsStatic() ? m_Owner : other.m_Owner;

    const auto& dynamicPos = dynamic->GetTransform().GetPosition();
    const auto& staticPos = staticObj->GetTransform().GetPosition();

    float overlapLeft = (dynamicPos.x + m_Width) - staticPos.x;
    float overlapRight = (staticPos.x + other.m_Width) - dynamicPos.x;
    float overlapTop = (dynamicPos.y + m_Height) - staticPos.y;
    float overlapBottom = (staticPos.y + other.m_Height) - dynamicPos.y;

    float minOverlap = std::min({ overlapLeft, overlapRight, overlapTop, overlapBottom });

    auto& transform = dynamic->GetTransform();

    if (minOverlap == overlapLeft)
    {
        transform.SetPosition(staticPos.x - other.m_Width, dynamicPos.y, 0.f);
    }
    else if (minOverlap == overlapRight)
    {
        transform.SetPosition(staticPos.x + m_Width, dynamicPos.y, 0.f);
    }
    else if (minOverlap == overlapTop)
    {
        transform.SetPosition(dynamicPos.x, staticPos.y - other.m_Height, 0.f);
    }
    else if (minOverlap == overlapBottom)
    {
        transform.SetPosition(dynamicPos.x, staticPos.y + m_Height, 0.f);
    }

    if (!m_Owner->IsStatic() && !other.m_Owner->IsStatic())
    {
        auto& transform1 = m_Owner->GetTransform();
        auto& transform2 = other.m_Owner->GetTransform();

        if (minOverlap == overlapLeft || minOverlap == overlapRight)
        {
            transform1.SetPosition(dynamicPos.x - minOverlap / 2, dynamicPos.y, 0.f);
            transform2.SetPosition(staticPos.x + minOverlap / 2, staticPos.y, 0.f);
        }
        else if (minOverlap == overlapTop || minOverlap == overlapBottom)
        {
            transform1.SetPosition(dynamicPos.x, dynamicPos.y - minOverlap / 2, 0.f);
            transform2.SetPosition(staticPos.x, staticPos.y + minOverlap / 2, 0.f);
        }
    }
}
