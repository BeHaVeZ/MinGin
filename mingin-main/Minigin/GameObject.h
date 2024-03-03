#pragma once
#include <concepts>
#include <memory>
#include <optional>
#include <vector>
#include "Transform.h"
#include "Component.h"

namespace dae
{
	//Gewoon kort: concept die checkt of het afgeleid is van component voor mijn templates ipv static_assert(component?)
	template <typename T>
	concept DerivedFromComponent = std::derived_from<T, Component>;

	class GameObject final : public std::enable_shared_from_this<GameObject>
	{
	public:
		void Update();
		void Render() const;

		GameObject() = default;
		~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		template <DerivedFromComponent T, typename... Args>
		std::shared_ptr<T> AddComponent(Args&&... args)
		{
			auto newComponent = std::make_shared<T>(std::forward<Args>(args)...);
			m_Components.emplace_back(newComponent);

			newComponent->SetGameObject(shared_from_this());

			return newComponent;
		}

		template <DerivedFromComponent T>
		std::shared_ptr<T> GetComponent() const
		{
			for (const auto& component : m_Components)
			{
				auto derivedComponent = std::dynamic_pointer_cast<T>(component);
				if (derivedComponent)
					return derivedComponent;
			}

			return nullptr;
		}

		void AddChild(std::shared_ptr<GameObject> child);
		void RemoveChild(std::shared_ptr<GameObject> child);

		void SetPosition(float x, float y);
		void SetPosition(float x, float y, float z);


		std::shared_ptr<GameObject> GetParent() const;
		glm::vec3 GetWorldPosition() const;
		Transform& GetTransform();

	private:
		void SetParent(std::shared_ptr<GameObject> parent);
		void UnsetParent();
		bool IsChildOf(std::shared_ptr<GameObject> potentialParent) const;


		Transform m_Transform{ };
		std::weak_ptr<GameObject> m_Parent;
		std::vector<std::shared_ptr<Component>> m_Components;
		std::vector<std::shared_ptr<GameObject>> m_Children;
	};
}