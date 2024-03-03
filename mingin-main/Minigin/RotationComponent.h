#pragma once
#include "Component.h"

namespace dae 
{
	class RotationComponent : public Component
	{
    public:
        RotationComponent(float distanceToParent, float rotationSpeed,bool counterClockWise);
        virtual ~RotationComponent() = default;

        virtual void Update() override;
        virtual void Render() const override;

    private:
        float m_DistanceToParent;
        float m_RotationSpeed;
        float m_CurrentAngle;
        bool m_CounterClockwise;
	};

}