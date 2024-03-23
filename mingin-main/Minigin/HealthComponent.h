#pragma once
#include "Component.h"
#include "Event.h"

namespace dae
{
	class HealthComponent final : public Component
	{
    public:
        HealthComponent(std::shared_ptr<GameObject> gameObject, int initialLives);

        void Update() override;
        void Render() const override;

        void Kill();
        int GetLives() const { return m_Lives; }
        int SetLives(int lives) { m_Lives = lives; }

        Event<int>* GetOnDeathEvent() const { return m_pOnDeath.get(); }

    private:
        int m_Lives;
        std::unique_ptr<Event<int>> m_pOnDeath;
	};
}