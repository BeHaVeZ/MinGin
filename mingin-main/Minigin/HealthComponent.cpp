#include "pch.h"
#include "HealthComponent.h"


namespace dae
{
    HealthComponent::HealthComponent(int initialLives)
        : Component(),
        m_Lives(initialLives),
        m_pOnDeath(std::make_unique<Event<int>>())
    {
    }

    void HealthComponent::Update()
    {
        if (m_Lives <= 0)
        {
            m_pOnDeath->Invoke(0);
        }
    }

    void HealthComponent::Render() const
    {
    }

    void HealthComponent::Kill()
    {
        --m_Lives;
        if (m_Lives <= 0)
        {
            m_Lives = 0;
        }

        m_pOnDeath->Invoke(m_Lives);
    }

}

