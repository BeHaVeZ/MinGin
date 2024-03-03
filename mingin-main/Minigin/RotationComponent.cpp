#include "RotationComponent.h"
#include "Timer.h"
#include "GameObject.h"
#include <glm/glm.hpp>
#include <iostream>

namespace dae
{
	RotationComponent::RotationComponent(float distanceToParent, float rotationSpeed)
		: m_DistanceToParent(distanceToParent), m_RotationSpeed(rotationSpeed), m_CurrentAngle{ 0.f }
	{
	}

	void RotationComponent::Update()
	{
		if (auto parent = GetGameObject()->GetParent())
		{
			float dt = Timer::GetInstance().GetDeltaTime();
			m_CurrentAngle += m_RotationSpeed * dt;

			const float newX = parent->GetTransform().GetPosition().x + cosf(m_CurrentAngle) * m_DistanceToParent;
			const float newY = parent->GetTransform().GetPosition().y + sinf(m_CurrentAngle) * m_DistanceToParent;

			GetGameObject()->SetPosition(newX, newY);
		}
	}
	void RotationComponent::Render() const {}
}