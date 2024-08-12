#include "Transition.h"
#include "Timer.h"

namespace dae
{
	Transition::Transition(const std::shared_ptr<GameObject>& gameObject, float transitionTime)
		: m_GameObject(gameObject)
		, m_TransitionTime(transitionTime)
		, m_Counter(0.f)
	{
	}

	void Transition::Update()
	{
		m_Counter += Timer::GetInstance().GetDeltaTime();

		if (m_Counter >= m_TransitionTime)
		{
			//Change scene
		}
	}
}