#pragma once
#include "Component.h"

namespace dae
{
	class HealthComponent final : public Component
	{
    public:
        HealthComponent(int initialLives);

        void Update() override;
        void Render() const override;

        void GetDamage(int incomingDamage);
        int GetLives() const { return m_Lives; }
        int SetLives(int lives) { m_Lives = lives; }


    private:
        int m_Lives;
        bool m_IsAlive;
	};
}