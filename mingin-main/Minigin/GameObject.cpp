#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Transform.h"
#include <iostream>


namespace dae 
{

    GameObject::~GameObject() = default;

    void GameObject::Update()
    {
        for (auto& component : m_Components)
        {
            component->Update();
        }
        for (const auto& child : m_Children)
        {
            child->Update();
        }
    }

    void GameObject::Render() const
    {
        for (auto& component : m_Components)
        {
            component->Render();
        }
    }



    void GameObject::AddChild(std::shared_ptr<GameObject> child)
    {
        if (child and child.get() != this and !child->IsChildOf(shared_from_this()))
        {
            child->UnsetParent();
            child->SetParent(shared_from_this());
            
            auto parentWorldPos = shared_from_this()->GetWorldPosition();
            auto childWorldPos = child->GetWorldPosition();
            child->SetPosition(childWorldPos.x, childWorldPos.y);

            m_Children.emplace_back(child);
        }
    }

    void GameObject::RemoveChild(std::shared_ptr<GameObject> child)
    {
        auto it = std::remove(m_Children.begin(), m_Children.end(), child);
        m_Children.erase(it, m_Children.end());

        child->UnsetParent();
    }

    void GameObject::SetParent(std::shared_ptr<GameObject> parent)
    {
        m_Parent = parent;
    }

    void GameObject::UnsetParent()
    {
        m_Parent.reset();
    }

    bool GameObject::IsChildOf(std::shared_ptr<GameObject> potentialParent) const
    {
        if (!potentialParent)
        {
            return false;
        }

        auto currentParent = m_Parent.lock();

        while (currentParent)
        {
            if (currentParent == potentialParent)
            {
                return true;
            }

            currentParent = currentParent->m_Parent.lock();
        }

        return false;
    }

    void GameObject::SetPosition(float x, float y)
    {
        m_Transform.SetPosition(x, y, 0.0f);
    }

    Transform& GameObject::GetTransform()
    {
        return m_Transform;
    }

    glm::vec3 GameObject::GetWorldPosition() const
    {
        if (auto parent = m_Parent.lock())
        {
            return parent->GetWorldPosition() + m_Transform.GetPosition();
        }
        else
        {
            return m_Transform.GetPosition();
        }
    }
}
