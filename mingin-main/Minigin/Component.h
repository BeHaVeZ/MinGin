#pragma once
#include <memory>

namespace dae
{
	class GameObject;
	class Subject;

	class Component
	{
	public:
		Component();
		virtual ~Component();
		virtual void Initialize() {}
		virtual void Update() = 0;
		virtual void Render() const = 0;

		Component(const Component& other) = default;                
		Component(Component&& other) noexcept = default;            
		Component& operator=(const Component& other) = default;
		Component& operator=(Component&& other) noexcept = default; 
		//hoe weet een de parent dat er een nieuwe component is toegevoegd
		std::shared_ptr<GameObject> GetGameObject() const
		{
			return m_pGameObject.lock();
		}

		void SetGameObject(std::shared_ptr<GameObject> gameObject)
		{
			m_pGameObject = gameObject;
		}

		Subject* GetSubject() const { return m_Subject; }

	protected:
		Subject* m_Subject;
		std::weak_ptr<GameObject> m_pGameObject;
	};
}