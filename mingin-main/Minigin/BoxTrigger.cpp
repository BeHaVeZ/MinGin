#include "pch.h"
#include "BoxTrigger.h"
#include "GameObject.h"
#include "Renderer.h"

dae::BoxTrigger::BoxTrigger(float width, float height, GameObject* owner)
    : m_Width(width), m_Height(height), m_Owner(owner)
{
}

void dae::BoxTrigger::Update()
{
}

void dae::BoxTrigger::Render() const
{
    //const auto& pos = m_Owner->GetTransform().GetPosition();
    //SDL_Color color = { 0, 255, 0, 255 };
    //dae::Renderer::GetInstance().DrawRect(pos.x, pos.y, m_Width, m_Height, color);
}

bool dae::BoxTrigger::IsOverlappingWith(const BoxTrigger& other) const
{
    const auto& pos1 = m_Owner->GetTransform().GetPosition();
    const auto& pos2 = other.m_Owner->GetTransform().GetPosition();

    bool xOverlap = pos1.x < pos2.x + other.m_Width && pos1.x + m_Width > pos2.x;
    bool yOverlap = pos1.y < pos2.y + other.m_Height && pos1.y + m_Height > pos2.y;

    return xOverlap && yOverlap;
}

void dae::BoxTrigger::OnTriggerEnter(BoxTrigger& other) const
{
    other;
}

void dae::BoxTrigger::OnTriggerExit(BoxTrigger& other) const
{
    other;
}
