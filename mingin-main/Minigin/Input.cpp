#include "pch.h"
#include "Input.h"

namespace dae 
{
	void Input::AddCommand(const ControllerKey& key, std::shared_ptr<Command> command)
	{
		m_ControllerCommands.emplace(std::make_pair(key,std::move(command)));
	}

	void Input::AddCommand(const KeyboardKey key, std::shared_ptr<Command> command)
	{
		m_KeyboardCommands.emplace(std::make_pair(key, std::move(command)));
	}
}