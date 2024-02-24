#pragma once
#include <memory>

namespace dae
{
	class GameObject;

	class Component
	{
	public:
		virtual void Update() = 0;
		virtual void Render() const = 0;
		virtual ~Component() = default;

		std::shared_ptr<GameObject> GetGameObject() const
		{
			return m_pGameObject.lock();
		}

		void SetGameObject(std::shared_ptr<GameObject> gameObject)
		{
			m_pGameObject = gameObject;
		}

	private:
		std::weak_ptr<GameObject> m_pGameObject;
	};
}