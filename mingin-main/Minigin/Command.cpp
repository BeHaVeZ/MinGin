#include "pch.h"
#include "Command.h"


namespace dae 
{
	void MoveCommand::Execute()
	{
		auto movement = m_GameObject->GetTransform().GetPosition() +
			(m_Direction * m_Speed * Timer::GetInstance().GetDeltaTime());
		m_GameObject->GetTransform().SetPosition(movement.x, movement.y, 0.0f);

		LOG_TRACE("Moving");
	}
}