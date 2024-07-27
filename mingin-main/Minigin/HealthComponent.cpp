#include "pch.h"
#include "HealthComponent.h"


namespace dae
{
    HealthComponent::HealthComponent(int initialLives)
        : Component(),
        m_Lives(initialLives),
        m_IsAlive(true)
    {
    }

    void HealthComponent::Update()
    {
    }

    void HealthComponent::Render() const
    {
    }

    void HealthComponent::GetDamage(int incomingDamage)
    {
        if (incomingDamage < 0)
        {
            LOG_WARNING("Can't inflict negative damage value.");
            return;
        }
        if (m_Lives - incomingDamage > 0)
        {
            m_Lives -= incomingDamage;
            m_Subject->Notify(Event::ActorHealthChange);
        }
        else
        {
            m_Lives = 0;
            m_IsAlive = false;
            m_Subject->Notify(Event::ActorDeath);
        }
    }

}

