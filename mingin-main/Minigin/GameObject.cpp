#include "pch.h"


namespace dae
{
	GameObject::GameObject()
		: m_IsDirty(false),
		m_WorldPosition{},
		m_Transform{}
	{
	}

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

			MarkDirty();
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

	void GameObject::MarkDirty()
	{
		m_IsDirty = true;

		for (const auto& child : m_Children)
		{
			child->MarkDirty();
		}
	}

	void GameObject::SetPosition(float x, float y, float z)
	{
		m_Transform.SetPosition(x, y, z);
		m_IsDirty = true;
	}

	void GameObject::SetPosition(float x, float y)
	{
		m_Transform.SetPosition(x, y, 0.0f);
		m_IsDirty = true;
	}

	Transform& GameObject::GetTransform()
	{
		return m_Transform;
	}

	std::shared_ptr<GameObject> GameObject::GetParent() const
	{
		auto parent = m_Parent.lock();
		if (!parent)
		{
			LOG_CRITICAL("Attempted to access parent of GameObject, but the parent is expired. (CHECK IF YOU ADDED/REMOVED IT TO/FROM SCENE!!)");
		}

		return parent;
	}

	glm::vec3 GameObject::GetWorldPosition() const
	{
		if (m_IsDirty)
		{
			if (auto parent = m_Parent.lock())
			{
				m_WorldPosition = parent->GetWorldPosition() + m_Transform.GetPosition();
			}
			else
			{
				m_WorldPosition = m_Transform.GetPosition();
			}
			//reset
			m_IsDirty = false;
		}

		return m_WorldPosition;
	}
}
