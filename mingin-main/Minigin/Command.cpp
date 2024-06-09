#include "pch.h"
#include "ServiceLocator.h"

namespace dae 
{
	void MoveCommand::Execute()
	{
		auto movement = m_GameObject->GetTransform().GetPosition() +
			(m_Direction * m_Speed * Timer::GetInstance().GetDeltaTime());
		m_GameObject->GetTransform().SetPosition(movement.x, movement.y, 0.0f);
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


	void PlaySoundCommand::Execute()
	{
		m_SL.PlaySound((unsigned short)0, 50.f);
		LOG_TRACE("Played sound on butten press U");
	}
	void MuteSoundCommand::Execute()
	{
		m_SL.ToggleMute();
		LOG_TRACE("MUTED");
	}
}