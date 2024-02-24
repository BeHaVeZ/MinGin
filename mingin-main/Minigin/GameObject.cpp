#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Transform.h"


namespace dae 
{

    GameObject::~GameObject() = default;

    void GameObject::Update()
    {
        for (auto& component : m_Components)
        {
            component->Update();
        }
    }

    void GameObject::Render() const
    {
        for (auto& component : m_Components)
        {
            component->Render();
        }
    }



    void GameObject::SetPosition(float x, float y)
    {
        m_LocalTransform.SetPosition(x, y, 0.0f);
    }

    const Transform& GameObject::GetTransform() const
    {
        return m_LocalTransform;
    }

}
