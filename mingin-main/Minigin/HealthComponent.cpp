#include "pch.h"
#include "HealthComponent.h"


namespace dae
{
    HealthComponent::HealthComponent(std::shared_ptr<GameObject> gameObject, int initialLives)
        : Component(),
        m_Lives(initialLives),
        m_pOnDeath(std::make_unique<Event<int>>())
    {
        SetGameObject(gameObject); // Het gameobject instellen voor deze component
    }

    void HealthComponent::Update()
    {
        // Hier kun je controleren op eventuele voorwaarden die de levens van het game-object beïnvloeden
        if (m_Lives <= 0)
        {
            // Als de levens op zijn, vuur het onDeath event af
            m_pOnDeath->Invoke(0); // 0 kan een specifiek doodsoorzaak zijn, zoals een nulwaarde voor de spelerindex
        }
    }

    void HealthComponent::Render() const
    {
        // Hier kun je eventueel iets renderen dat verband houdt met de gezondheid van het game-object
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

