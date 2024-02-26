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
		//TODO:
		//aan en af zetten IsActive functies bool die bepaald of iets gerenderd moet worden of niet
		//AddChild / SetParent
		//RemoveChild
		void SetPosition(float x, float y);
		const Transform& GetTransform() const;

	private:
		Transform m_LocalTransform{};
		std::vector<std::shared_ptr<Component>> m_Components;
	};
}