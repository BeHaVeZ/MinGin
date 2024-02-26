#pragma once
#include <memory>
#include <optional>
#include <vector>
#include "Transform.h"
#include "Component.h"

namespace dae
{
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


		template <typename T, typename... Args>
		std::shared_ptr<T> AddComponent(Args&&... args)
		{
			static_assert(std::is_base_of<Component, T>::value, "T must be a subclass of Component");

			auto newComponent = std::make_shared<T>(std::forward<Args>(args)...);
			m_Components.emplace_back(newComponent);

			newComponent->SetGameObject(shared_from_this());

			return newComponent;
		}

		template <typename T>
		std::shared_ptr<T> GetComponent() const
		{
			static_assert(std::is_base_of<Component, T>::value, "T must be a subclass of Component");

			for (const auto& component : m_Components)
			{
				auto derivedComponent = std::dynamic_pointer_cast<T>(component);
				if (derivedComponent)
					return derivedComponent;
			}

			return nullptr;
		}
//aan en af zetten IsActive functies
		void SetPosition(float x, float y);
		const Transform& GetTransform() const;

	private:
		Transform m_LocalTransform{};
		std::vector<std::shared_ptr<Component>> m_Components;
	};
}
