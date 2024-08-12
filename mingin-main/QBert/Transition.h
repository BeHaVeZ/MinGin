#pragma once
#include <memory>
#include "Component.h"


namespace dae
{
	class Transition final : public Component
	{
	public:
		explicit Transition(const std::shared_ptr<GameObject>& gameObject, float transitionTime);
		~Transition() = default;

		Transition(const Transition& other) = delete;
		Transition(Transition&& other) noexcept = delete;
		Transition& operator=(const Transition& other) = delete;
		Transition& operator=(Transition&& other) noexcept = delete;

		void Update() override;

	private:
		std::shared_ptr<GameObject> m_GameObject{};
		float m_TransitionTime, m_Counter;
	};
}