#include "pch.h"


namespace dae 
{
	void MoveCommand::Execute()
	{
		auto movement = m_GameObject->GetTransform().GetPosition() +
			(m_Direction * m_Speed * Timer::GetInstance().GetDeltaTime());
		m_GameObject->GetTransform().SetPosition(movement.x, movement.y, 0.0f);

		LOG_TRACE("Moving");
	}

	void KillCommand::Execute()
	{
		if (!m_pHealthComponent)
		{
			LOG_CRITICAL("HealthComponent is not assigned!");
			return;
		}
		m_pHealthComponent->Kill();
	}



	
	void AddScoreCommand::Execute()
	{
		if (!m_pScoreComponent)
		{
			LOG_CRITICAL("ScoreComponent is not assigned!");
			return;
		}
		m_pScoreComponent->AddScore(50);
	}

}